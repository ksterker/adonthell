/*
 $Id: collision.cc,v 1.6 2008/01/13 18:36:01 ksterker Exp $
 
 Copyright (C) 2007 Kai Sterker <kaisterker@linuxgames.com>
 Part of the Adonthell Project http://adonthell.linuxgames.com
 
 Adonthell is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
 
 Adonthell is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with Adonthell; if not, write to the Free Software 
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/**
 * @file   world/collision.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Implements the 3D collision class.
 * 
 */

#include <cmath>
#include "world/collision.h"
#include "world/plane3.h"

using world::collision;
using world::plane3;

// ctor
collision::collision (const vector3<float> & position, const vector3<float> & velocity, const vector3<float> & radius)
{
    update_movement (position, velocity);
    
    Radius = radius;
    NearestDistance = 0.0;
#ifdef DEBUG_COLLISION
    Triangle = NULL;
#endif
}

// ctor
collision::~collision ()
{
#ifdef DEBUG_COLLISION
    delete Triangle;
#endif
}

// change movement information
void collision::update_movement (const vector3<float> & position, const vector3<float> & velocity)
{
    BasePoint = position;
    Velocity = velocity;
    NormalizedVelocity = velocity.normalize ();
    CollisionFound = false;
}

// test collision against given triangle
void collision::check_triangle (const triangle3<s_int16> & triangle, const vector3<s_int16> & offset) 
{
    // translate triangle into espace
    triangle3<float> eTriangle = triangle.translate (Radius, offset);
    
    // Make the plane containing this triangle. 
    plane3 trianglePlane (eTriangle);
    
    // Is triangle front-facing to the velocity vector? 
    if (trianglePlane.is_facing (NormalizedVelocity)) 
    { 
        // Get interval of plane intersection: 
        double t0, t1;
        
        bool embeddedInPlane = false; 
        
        // Calculate the signed distance from sphere position to triangle plane 
        double signedDistToTrianglePlane = trianglePlane.signed_distance (BasePoint);
        
        // cache this as we’re going to use it a few times below: 
        float normalDotVelocity = trianglePlane.normal ().dot (Velocity);
        
        // if sphere is travelling parallel to the plane: 
        if (fabsf (normalDotVelocity) < 0.005f) 
        {
            // sphere is not embedded in plane --> no collision possible: 
            if (fabs (signedDistToTrianglePlane) >= 1.0f) 
            { 
                return;
            }
            // sphere is embedded in plane --> it intersects in the whole range [0..1]
            else 
            { 
                embeddedInPlane = true; 
                t0 = 0.0; 
                t1 = 1.0; 
            }
        } 
        else 
        { 
            // N dot D is not 0. Calculate intersection interval: 
            t0 = (-1.0 - signedDistToTrianglePlane) / normalDotVelocity; 
            t1 = ( 1.0 - signedDistToTrianglePlane) / normalDotVelocity; 
            
            // Swap so t0 < t1 
            if (t0 > t1) 
            { 
                double temp = t1; 
                t1 = t0; 
                t0 = temp; 
            }
            
            // Check that at least one result is within range: 
            if (t0 > 1.0f || t1 < 0.0f) 
            { 
                return; 
            } 

            // Clamp to [0..1] 
            if (t0 < 0.0) t0 = 0.0;
            if (t1 > 1.0) t1 = 1.0; 
        }
        
        // OK, at this point we have two time values t0 and t1 between which the swept 
        // sphere intersects with the triangle plane. If any collision is to occur it 
        // must happen within this interval. 
        vector3<float> collisionPoint; 
        bool foundCollison = false; 
        float t = 1.0; 
        
        // First we check for the easy case - collision inside the triangle. If this 
        // happens it must be at time t0 as this is when the sphere rests on the front 
        // side of the triangle plane. Note, this can only happen if the sphere is not 
        // embedded in the triangle plane. 
        if (!embeddedInPlane) 
        { 
            vector3<float> planeIntersectionPoint = (BasePoint - trianglePlane.normal ()) + Velocity * t0; 
            if (eTriangle.contains (planeIntersectionPoint)) 
            { 
                collisionPoint = planeIntersectionPoint; 
                foundCollison = true;
                t = t0; 
            }
        }
        
        // if we haven’t found a collision already we’ll have to sweep sphere against 
        // points and edges of the triangle. 
        // Note: A collision inside the triangle (the check above) will always happen 
        // before a vertex or edge collision! This is why we can skip the swept test 
        // if the above gives a collision! 
        if (foundCollison == false) 
        { 
            // some commonly used terms: 
            float velocitySquaredLength = Velocity.squared_length (); 
            float a, b, c, newT;
            
            // For each vertex or edge a quadratic equation has to 
            // be solved. We parameterize this equation as 
            // a*t^2 + b*t + c = 0 and below we calculate the 
            // parameters a, b and c for each test. 
            
            // check against points:
            for (int i = 0; i < 3; i++)
            {
                const vector3<float> p = eTriangle.get_point (i);
                b = 2.0 * Velocity.dot (BasePoint - p); 
                c = (p - BasePoint).squared_length () - 1.0;
                if (solve_quadric_equation (velocitySquaredLength, b, c, t, &newT)) 
                {
                    t = newT;
                    foundCollison = true;
                    collisionPoint = p;
                }
            }

            // check agains edges:
            for (int i = 0; i < 3; i++)
            {
                const vector3<float> edge = eTriangle.get_edge (i);
                const vector3<float> baseToVertex = eTriangle.get_point (i) - BasePoint;

                float edgeSquaredLength = edge.squared_length(); 
                float edgeDotVelocity = edge.dot(Velocity); 
                float edgeDotBaseToVertex = edge.dot(baseToVertex); 

                // Calculate parameters for equation 
                a = edgeSquaredLength * -velocitySquaredLength + 
                    edgeDotVelocity * edgeDotVelocity; 
                b = edgeSquaredLength * (2 * Velocity.dot (baseToVertex)) - 
                    2.0 * edgeDotVelocity * edgeDotBaseToVertex; 
                c = edgeSquaredLength * (1 - baseToVertex.squared_length ()) + 
                    edgeDotBaseToVertex * edgeDotBaseToVertex; 
                
                // Does the swept sphere collide against infinite edge? 
                if (solve_quadric_equation (a, b, c, t, &newT)) 
                {
                    // Check if intersection is within line segment: 
                    float f = (edgeDotVelocity * newT - edgeDotBaseToVertex) / edgeSquaredLength; 
                    if (f >= 0.0 && f <= 1.0) 
                    { 
                        // intersection took place within segment. 
                        t = newT;
                        foundCollison = true; 
                        collisionPoint = eTriangle.get_point (i) + edge * f; 
                    } 
                } 
            }
        }
            
        // set result 
        if (foundCollison) 
        { 
            // distance to collision: ’t’ is time of collision 
            float distToCollision = t * Velocity.length ();
            
            // Does this triangle qualify for the closest hit? 
            // It does if it’s the first hit or the closest 
            if (CollisionFound == false || distToCollision < NearestDistance) 
            { 
                // Collision information nessesary for sliding 
                NearestDistance = distToCollision;
                IntersectionPoint = collisionPoint;
                CollisionFound = true;
                
                // debugging
#ifdef DEBUG_COLLISION
                printf ("Collision @ [%.2f, %.2f, %.2f] Dist %.4f\n", collisionPoint.x(), collisionPoint.y(), collisionPoint.z(), distToCollision);
                if (Triangle) delete Triangle; 
                Triangle = new triangle3<float> (triangle.translate (vector3<float> (1.0f, 1.0f, 1.0f), offset));
#endif
            } 
        } 
    } // if not backface 
}
            
// solve quadric equation
bool collision::solve_quadric_equation (const float & a, const float & b, const float & c, const float & threshold, float* result) const
{
    // check if a solution exists
    float determinant = b * b - 4.0f * a * c;
    
    // if determinant is negative it means no solutions.
    if (determinant < 0.0f) return false;
    
    // calculate the two results
    float sqrtD = sqrt (determinant);
    float div2a = 1.0f / (a + a);
    float r1 = (-b - sqrtD) * div2a;
    float r2 = (-b + sqrtD) * div2a;
    
    // sort so r1 <= r2 
    if (r1 > r2) 
    { 
        float temp = r2;
        r2 = r1; 
        r1 = temp; 
    }
    
    // get lowest result: 
    if (r1 > 0 && r1 < threshold) 
    { 
        *result = r1; 
        return true; 
    }
    
    // it is possible that we want r2 - this can happen if r1 < 0 
    if (r2 > 0 && r2 < threshold) 
    { 
        *result = r2;
        return true;
    } 
    
    // no (valid) solutions 
    return false; 
}
