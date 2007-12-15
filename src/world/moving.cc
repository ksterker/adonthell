/*
 $Id: moving.cc,v 1.7 2007/12/15 23:15:10 ksterker Exp $
 
 Copyright (C) 2002 Alexandre Courbot <alexandrecourbot@linuxgames.com>
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
 * @file   world/moving.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * @author Kai Sterker <kai.sterker@linuxgames.com>
 * 
 * @brief  Defines the moving class.
 * 
 * 
 */

#include <cmath>
#include "world/moving.h"
#include "world/area.h"
#include "world/plane3.h"

#ifdef DEBUG_COLLISION
#include "gfx/screen.h"
#endif

using world::moving;
using world::area;
using world::plane3;
using world::vector3;

// ctor
moving::moving (world::area & mymap)
    : placeable (mymap), coordinates ()
{
    Lx = mymap.length();
    Ly = mymap.height();
}

// movement over ground
void moving::set_velocity (const float & vx, const float & vy) 
{
    Velocity.set_x (vx);
    Velocity.set_y (vy);
}

// indicate falling or jumping
void moving::set_vertical_velocity (const float & vz)
{
    Velocity.set_z (vz);
}

// set pixel position on tile
void moving::set_offset (const u_int16 & ox, const u_int16 & oy)
{
    coordinates::set_offset (ox, oy);
    Position.set_x (ox);
    Position.set_y (oy);
}

// set z position
void moving::set_altitude (const s_int32 & z)
{
    coordinates::set_altitude (z);
    Position.set_z (z);
}

// boundary for movement
void moving::set_limits (const u_int16 & mx, const u_int16 & my)
{
    Lx = mx;
    Ly = my;
}

// check objects on map for collision
bool moving::collide_with_objects (collision *collisionData)
{
    GroundPos = -10000;
    
    const placeable_shape *shape = current_shape ();
    collisionData->set_radius (shape->length() / 2.0, shape->width() / 2.0, shape->height() / 2.0);

    // calculate start point of squares we need to check for possible collisions
    s_int16 start_x = X + ((s_int16) Velocity.x () + Ox) / SQUARE_SIZE;
    s_int16 start_y = Y + ((s_int16) Velocity.y () + Oy) / SQUARE_SIZE;
    
    // don't exceed boundary of map!
    if (start_x < 0) start_x = 0;
    if (start_y < 0) start_y = 0;

    // calculate end point of squares we need to check for possible collisions
    s_int16 end_x = start_x + 1 + (Ox + shape->length () + (s_int16) Velocity.x ()) / SQUARE_SIZE;
    s_int16 end_y = start_y + 1 + (Oy + shape->width () + (s_int16) Velocity.y ()) / SQUARE_SIZE;

    // don't exceed boundary of map!
    if (end_x > Lx) end_x = Lx;
    if (end_y > Ly) end_y = Ly;
    
    for (s_int32 i = start_x; i < end_x; i++)
    {
        for (s_int32 j = start_y; j < end_y; j++)
        {
            // iterate over mapsquares in range
            square *msqr = Mymap.get (i, j);
            
            // iterate over all objects on square ...
            for (square::iterator it = msqr->begin(); it != msqr->end(); it++)
            {
                // ... get offset of shape from current tile ...                
                const vector3<s_int16> offset (i - it->x() + it->ox(), j - it->y() + it->oy(), it->z());
                
                // ... and check if collision occurs
                shape = it->obj->current_shape ();
                shape->collide (collisionData, offset);
                
                // calculate z position of ground
                s_int32 objz = it->z () + shape->height ();
                if (GroundPos < objz) GroundPos = objz;
            }
        }
    }
    
    return collisionData->collision_found ();
}

// try to move from given position with given velocity
vector3<float> moving::execute_move (const vector3<float> & pos, const vector3<float> & vel, u_int16 depth) 
{
    static float veryCloseDistance = 0.005f;
    
    // do we need to worry? 
    if (depth > 5) 
    {
        return pos; 
    }
    
    // ok, we need to worry
    collision collisionData (pos, vel);
    
    // check for collision
    if (!collide_with_objects (&collisionData))
    { 
        // if no collision occured, we just move along the velocity
        return pos + vel;
    }
    
    // the desired destination point 
    vector3<float> destinationPoint = pos + vel;
    vector3<float> newBasePoint = pos;
    
    // only update if we are not already very close and if so we only 
    // move very close to intersection, not to the exact spot. 
    if (collisionData.distance () >= veryCloseDistance) 
    {
        vector3<float> v = vel;
        v.set_length (collisionData.distance () - veryCloseDistance);
        newBasePoint = pos + v; 
        
        // Adjust polygon intersection point (so sliding 
        // plane will be unaffected by the fact that we 
        // move slightly less than collision tells us) 
        collisionData.update_intersection (v.normalize () * veryCloseDistance); 
    }
    
    // determine the sliding plane 
    vector3<float> slidePlaneOrigin = collisionData.intersection (); 
    vector3<float> slidePlaneNormal = (newBasePoint - slidePlaneOrigin).normalize (); 
    plane3 slidingPlane (slidePlaneOrigin, slidePlaneNormal);
    
    vector3<float> newDestinationPoint = destinationPoint - slidePlaneNormal * 
        slidingPlane.signed_distance (destinationPoint);
    
    // Generate the slide vector, which will become our new velocity vector for the next iteration 
    vector3<float> newVelocityVector = newDestinationPoint - slidePlaneOrigin; 
    
    // don't recurse if the new velocity is very small 
    if (newVelocityVector.length() < veryCloseDistance) 
    { 
        return newBasePoint; 
    }
    
    return execute_move (newBasePoint, newVelocityVector, ++depth); 
}

// calculate new position
void moving::update_position ()
{
    // assuming that 1 meter is about 35px ...
    static float gravity = -9.81 * 35;
    
    // calculate radius of ellipsoid
    placeable_shape * shape = current_shape();
    if (shape != NULL)
    {
        const vector3<float> eRadius (shape->length() / 2.0, shape->width() / 2.0, shape->height() / 2.0);
        
        // calculate position (= center of ellipse --> + 1) and velocity in eSpace 
        vector3<float> eSpacePosition (Position.x() / eRadius.x() + 1, Position.y() / eRadius.y() + 1, Position.z() / eRadius.z() + 1);
        vector3<float> eSpaceVelocity (Velocity.x() / eRadius.x(), Velocity.y() / eRadius.y(), Velocity.z() / eRadius.z());
        vector3<float> finalPosition = execute_move (eSpacePosition, eSpaceVelocity); 
        
        // apply gravity effect
        eSpaceVelocity = vector3<float> (0.0, 0.0, gravity / eRadius.z());
        finalPosition = execute_move (finalPosition, eSpaceVelocity); 

        // convert final result back to R3
        float x = (finalPosition.x() - 1) * eRadius.x();
        float y = (finalPosition.y() - 1) * eRadius.y();
        float z = (finalPosition.z() - 1) * eRadius.z();
        
        // update position on map, which must be in whole pixels 
        if (x >= SQUARE_SIZE) 
        {
            x -= SQUARE_SIZE;
            X++;
        }
        else if (x < 0)
        {
            x += SQUARE_SIZE;
            X--;
        }
        if (y >= SQUARE_SIZE) 
        {
            y -= SQUARE_SIZE;
            Y++;
        }
        else if (y < 0)
        {
            y += SQUARE_SIZE;
            Y--;
        }
        
        Ox = (u_int16) x;
        Oy = (u_int16) y;
        Z = (s_int32) z;
                
#ifdef DEBUG_COLLISION
        printf ("Position = [%.2f, %.2f, %.2f]\n", x, y, z);
#endif

        // update precise location for next iteration
        Position.set (x, y, z);
    }
}

// update movable position
bool moving::update ()
{
    Mymap.remove (this);
    update_position ();
    Mymap.put (this);
    
    return true; 
}
