/*
 $Id: moving.cc,v 1.14 2008/02/16 21:13:25 ksterker Exp $
 
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

#ifdef DEBUG_COLLISION
    Image = gfx::create_surface();
    Image->set_alpha (255, true);
    Image->resize (gfx::screen::length(), gfx::screen::height());
#endif
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
    const placeable_shape *shape = current_shape ();
    
    // calculate start point of squares we need to check for possible collisions
    float fsx = X * SQUARE_SIZE + Ox + (Velocity.x () < 0 ? Velocity.x() : 0) + shape->x();
    float fsy = Y * SQUARE_SIZE + Oy + (Velocity.y () < 0 ? Velocity.y() : 0) + shape->y();
    float fex = fsx + (Velocity.x () > 0 ? Velocity.x() : 0) + shape->length() + SQUARE_SIZE;
    float fey = fsy + (Velocity.y () > 0 ? Velocity.y() : 0) + shape->width() + SQUARE_SIZE;

    // calculate vertical range of moving
    float fsz = Z + (Velocity.z() < 0 ? Velocity.z() : 0);
    float fez = fsz + (Velocity.z() > 0 ? Velocity.z() : 0) + shape->height();
    
    // don't exceed boundary of map!
    u_int16 start_x = fsx < 0 ? 0 : fsx / SQUARE_SIZE;
    u_int16 start_y = fsy < 0 ? 0 : fsy / SQUARE_SIZE;
    u_int16 end_x   = fex > Lx * SQUARE_SIZE ? Lx : fex / SQUARE_SIZE;
    u_int16 end_y   = fey > Ly * SQUARE_SIZE ? Ly : fey / SQUARE_SIZE;

/*
#ifdef DEBUG_COLLISION
    // show squares used for collision detection
    Image->fillrect (start_x * SQUARE_SIZE, start_y * SQUARE_SIZE, 
                     (end_x - start_x) * SQUARE_SIZE, (end_y - start_y) * SQUARE_SIZE,
                     Image->map_color (0, 0, 128, 64));
#endif
*/
 
    for (u_int16 i = start_x; i < end_x; i++)
    {
        for (u_int16 j = start_y; j < end_y; j++)
        {
            // iterate over mapsquares in range
            square *msqr = Mymap.get (i, j);
            
            // iterate over all objects on square ...
            for (square::iterator it = msqr->begin(); it != msqr->end(); it++)
            {
                shape = it->obj->current_shape ();

                // is object inside vertical range of moving
                if ((fsz <= it->z() && it->z() < fez) ||
                    (fsz >= it->z() && it->z() + shape->height() < fez) ||
                    (fsz <= it->z() + shape->height() && it->z() + shape->height() < fez)) 
                {
                    // ... get offset of shape from current tile ...                
                    const vector3<s_int16> offset ((it->x() - X) * SQUARE_SIZE + it->ox(), (it->y() - Y) * SQUARE_SIZE + it->oy(), it->z());
                    
                    // ... and check if collision occurs
                    shape->collide (collisionData, offset);
                }
            }
        }
    }
    
    return collisionData->collision_found ();
}

// try to move from given position with given velocity
vector3<float> moving::execute_move (collision *collisionData, u_int16 depth) 
{
    static float veryCloseDistance = 0.00005f;
    
    const vector3<float> & vel = collisionData->velocity ();
    const vector3<float> & pos = collisionData->position ();

#ifdef DEBUG_COLLISION
    if (vel.x() != 0.0f || vel.y() != 0.0f)
    {
        printf ("[%i] ", depth);
        printf ("Pos [%.4f, %.4f, %.4f] ", pos.x(), pos.y(), pos.z());
        printf ("Vel [%.4f, %.4f, %.4f]\n", vel.x(), vel.y(), vel.z());
    }
#endif
    
    // do we need to worry? 
    if (depth > 5) 
    {
        return pos;
    }
    
    // check for collision
    if (!collide_with_objects (collisionData))
    { 
        // if no collision occured, we just move along the velocity
        return pos + vel;
    }
    
#ifdef DEBUG_COLLISION
    if (vel.x() != 0.0f || vel.y() != 0.0f)
    {
        const vector3<float> intersection = collisionData->intersection ();
        printf ("    Col [%.4f, %.4f, %.4f] ", intersection.x(), intersection.y(), intersection.z());
        printf ("Dst %.5f\n", collisionData->distance());
    }
#endif
    
    // the desired destination point 
    vector3<float> destinationPoint = pos + vel;
    vector3<float> newBasePoint = pos;
    
    // only update if we are not already very close and if so we only 
    // move very close to intersection, not to the exact spot. 
    if (collisionData->distance () >= veryCloseDistance) 
    {
        vector3<float> v = vel;
        v.set_length (collisionData->distance () - veryCloseDistance);
        newBasePoint = pos + v; 
        
        // Adjust polygon intersection point (so sliding 
        // plane will be unaffected by the fact that we 
        // move slightly less than collision tells us) 
        collisionData->update_intersection (v.normalize () * veryCloseDistance); 
    }
    
    // determine the sliding plane 
    vector3<float> slidePlaneOrigin = collisionData->intersection (); 
    vector3<float> slidePlaneNormal = (newBasePoint - slidePlaneOrigin).normalize (); 
    plane3 slidingPlane (slidePlaneOrigin, slidePlaneNormal);
    
    vector3<float> newDestinationPoint = destinationPoint - slidePlaneNormal * 
        slidingPlane.signed_distance (destinationPoint);
    
    // generate the slide vector, which will become our new velocity vector for the next iteration 
    vector3<float> newVelocityVector = newDestinationPoint - slidePlaneOrigin; 
    
    // don't recurse if the new velocity is very small 
    if (newVelocityVector.length() < veryCloseDistance) 
    { 
        collisionData->set_falling (false);
        return newBasePoint; 
    }
    
    collisionData->update_movement (newBasePoint, newVelocityVector);
    return execute_move (collisionData, ++depth); 
}

// calculate new position
void moving::update_position ()
{
    static float gravity = -4.905;
    
    placeable_shape * shape = current_shape();
    if (shape != NULL)
    {
        // calculate radius of ellipsoid
        const vector3<float> eRadius (shape->length() / 2.0, shape->width() / 2.0, shape->height() / 2.0);
        
        // calculate position (= center of ellipse --> + 1) and velocity in eSpace 
        vector3<float> eSpacePosition ((Position.x() + shape->x()) / eRadius.x() + 1, (Position.y() + shape->y()) / eRadius.y() + 1, Position.z() / eRadius.z() + 1);
        vector3<float> eSpaceVelocity (Velocity.x() / eRadius.x(), Velocity.y() / eRadius.y(), Velocity.z() / eRadius.z());
        
        collision collisionData (eSpacePosition, eSpaceVelocity, eRadius);
        vector3<float> finalPosition = execute_move (&collisionData);         
        
#ifdef DEBUG_COLLISION
        gfx::drawing_area da (0, 0, Image->length() - 1, Image->height() - 1);

        u_int16 pos_x = X*SQUARE_SIZE+Ox+shape->x();
        u_int16 pos_y = Y*SQUARE_SIZE+Oy+shape->y()-Z;
        
        // draw character bounding box
        cube3 bbox (shape->length(), shape->width(), shape->height());
        bbox.draw (pos_x, pos_y, &da, Image);
        
        pos_x += shape->length()/2;
        pos_y += shape->width()/2;

        // draw position of character
        Image->draw_line (pos_x - 5, pos_y, pos_x + 5, pos_y, Image->map_color (255, 255, 0), &da);                           
        Image->draw_line (pos_x, pos_y - 5, pos_x, pos_y + 5, Image->map_color (255, 255, 0), &da);                           
                          
        pos_y -= shape->height()/2; 
        
        // draw velocity along x,y axis
        Image->draw_line (pos_x, pos_y, pos_x + Velocity.x()*20, pos_y + Velocity.y() * 20, Image->map_color (0, 0, 255), &da);

        // draw gravity
        Image->draw_line (pos_x, pos_y, pos_x, pos_y - gravity * 20, Image->map_color (255, 0, 0), &da);        

        // draw velocity along z axis
        Image->draw_line (pos_x, pos_y, pos_x, pos_y - Velocity.z() * 20, Image->map_color (0, 0, 255), &da);        

        // did we collide at all?
        const triangle3<float> *tri = collisionData.triangle ();
        if (tri != NULL)
        {
            printf ("\n");

            // draw triangle we collided with
            tri->draw (X*SQUARE_SIZE, Y*SQUARE_SIZE, Image);
        }
#endif

        // apply gravity effect
        eSpaceVelocity = vector3<float> (0.0f, 0.0f, gravity / eRadius.z());
        collisionData.update_movement (finalPosition, eSpaceVelocity);
        finalPosition = execute_move (&collisionData); 
                
        // convert final result back to R3
        float x = (finalPosition.x() - 1) * eRadius.x() - shape->x();
        float y = (finalPosition.y() - 1) * eRadius.y() - shape->y();
        float z = (finalPosition.z() - 1) * eRadius.z();
        
#ifdef DEBUG_COLLISION
        if (tri != NULL)
        {
            // draw actual movement along x,y axis
            Image->draw_line (pos_x, pos_y, pos_x + (x - Position.x()) * 20, pos_y + (y - Position.y()) * 20, Image->map_color (0, 255, 0), &da);
        
            // draw actual movement along z axis
            Image->draw_line (pos_x, pos_y, pos_x, pos_y - (z - Z) * 20, Image->map_color (0, 255, 0), &da);
        }
#endif
    
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

        // calculate ground position if different from z-pos
        if (collisionData.is_falling ())
        {
            calculate_ground_pos ();
        }
        else
        {
            GroundPos = Z;
        }
                
        // update precise location for next iteration
        Position.set (x, y, z);
    }
}

// calculate z position of ground
void moving::calculate_ground_pos ()
{
    GroundPos = -10000;
    
    // get square we occupy
    square *msqr = Mymap.get (X, Y);
    
    // iterate over all objects on square ...
    for (square::iterator it = msqr->begin(); it != msqr->end(); it++)
    {
        placeable_shape *shape = it->obj->current_shape ();
        
        s_int32 objz = it->z () + shape->z() + shape->height ();
        printf ("ObjZ = %i (%i + %i + %i) MyZ = %i\n", objz, it->z(), shape->z(), shape->height (), Z);
        
        // only check objects below our character
        if (objz > Z) return;
        
        if (GroundPos < objz) GroundPos = objz;
    }
}

// update movable position
bool moving::update ()
{
#ifdef DEBUG_COLLISION
    // clear image
    Image->fillrect (0, 0, Image->length() - 1, Image->height() - 1, Image->map_color (0, 0, 0, 0));
#endif
    
    // we can skip the whole collision stuff if we're not moving
    if (vx() != 0.0f || vy() != 0.0f || vz() != 0.0f || GroundPos != Z)
    {
        Mymap.remove (this);
        update_position ();
        Mymap.put (this);
    }
    
    return true; 
}
