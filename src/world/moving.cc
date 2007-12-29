/*
 $Id: moving.cc,v 1.10 2007/12/29 22:21:37 ksterker Exp $
 
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
    Image->resize (gfx::screen::length(), gfx::screen::height());
    Image->set_mask (true);
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
    GroundPos = -10000;
    
    const placeable_shape *shape = current_shape ();
    float fox = shape->length() / 2.0;
    float foy = shape->width() / 2.0;
    
    // calculate start point of squares we need to check for possible collisions
    float fsx = X * SQUARE_SIZE + (Velocity.x () < 0 ? Velocity.x() : 0) + fox + Ox;
    float fsy = Y * SQUARE_SIZE + (Velocity.y () < 0 ? Velocity.y() : 0) + foy + Oy;
    float fex = fsx + (Velocity.x () > 0 ? Velocity.x() : 0) + fox + Ox + SQUARE_SIZE;
    float fey = fsy + (Velocity.y () > 0 ? Velocity.y() : 0) + foy + Oy + SQUARE_SIZE;

    // calculate vertical range of moving
    float fsz = Z + (Velocity.z() < 0 ? Velocity.z() : 0);
    float fez = fsz + (Velocity.z() > 0 ? Velocity.z() : 0) + shape->height();
    
    // don't exceed boundary of map!
    u_int16 start_x = fsx < 0 ? 0 : fsx / SQUARE_SIZE;
    u_int16 start_y = fsy < 0 ? 0 : fsy / SQUARE_SIZE;
    u_int16 end_x   = fex > Lx * SQUARE_SIZE ? Lx : fex / SQUARE_SIZE;
    u_int16 end_y   = fey > Ly * SQUARE_SIZE ? Ly : fey / SQUARE_SIZE;
    
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
                    const vector3<s_int16> offset ((it->x() - start_x) * SQUARE_SIZE + it->ox(), (it->y() - start_y) * SQUARE_SIZE + it->oy(), it->z());
                    
                    // ... and check if collision occurs
                    shape->collide (collisionData, offset);
                    
                    // calculate z position of ground
                    s_int32 objz = it->z () + shape->height ();
                    if (GroundPos < objz) GroundPos = objz;
                }
            }
        }
    }
    
    return collisionData->collision_found ();
}

// try to move from given position with given velocity
vector3<float> moving::execute_move (collision *collisionData, u_int16 depth) 
{
    static float veryCloseDistance = 0.005f;
    
    const vector3<float> & vel = collisionData->velocity ();
    const vector3<float> & pos = collisionData->position ();
    
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
    const vector3<float> intersection = collisionData->intersection ();
    printf ("Pos [%.2f, %.2f, %.2f] ", pos.x(), pos.y(), pos.z());
    printf ("Vel [%.2f, %.2f, %.2f] ", vel.x(), vel.y(), vel.z());
    printf ("Col [%.2f, %.2f, %.2f] ", intersection.x(), intersection.y(), intersection.z());
    printf ("Dst %.3f\n", collisionData->distance());
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
        return newBasePoint; 
    }
    
    collisionData->update_movement (newBasePoint, newVelocityVector);
    return execute_move (collisionData, ++depth); 
}

// calculate new position
void moving::update_position ()
{
    static float gravity = -0.981;
    
    placeable_shape * shape = current_shape();
    if (shape != NULL)
    {
#ifdef DEBUG_COLLISION
        // clear image
        Image->fillrect (0, 0, Image->length() - 1, Image->height() - 1, gfx::screen::trans_color ());
        gfx::drawing_area da (0, 0, Image->length() - 1, Image->height() - 1);
            
        u_int16 pos_x = X*SQUARE_SIZE+Ox;
        u_int16 pos_y = Y*SQUARE_SIZE+Oy-Z-shape->height();
        
        // draw character bounding box
        cube3 bbox (shape->length(), shape->width(), shape->height());
        bbox.draw (pos_x, pos_y, &da, Image);
        
        pos_x += shape->length()/2;
        pos_y += (shape->width()+shape->height())/2; 
        
        // draw velocity along x,y axis
        Image->draw_line (pos_x, pos_y, pos_x + Velocity.x()*20, pos_y + Velocity.y() * 20, Image->map_color (0, 0, 255), &da);

        // draw gravity
        Image->draw_line (pos_x, pos_y, pos_x, pos_y - gravity * 20, Image->map_color (255, 0, 0), &da);        

        // draw velocity along z axis
        Image->draw_line (pos_x, pos_y, pos_x, pos_y - Velocity.z() * 20, Image->map_color (0, 0, 255), &da);        
#endif
        
        // calculate radius of ellipsoid
        const vector3<float> eRadius (shape->length() / 2.0, shape->width() / 2.0, shape->height() / 2.0);
        
        // calculate position (= center of ellipse --> + 1) and velocity in eSpace 
        vector3<float> eSpacePosition (Position.x() / eRadius.x() + 1, Position.y() / eRadius.y() + 1, Position.z() / eRadius.z() + 1);
        vector3<float> eSpaceVelocity (Velocity.x() / eRadius.x(), Velocity.y() / eRadius.y(), Velocity.z() / eRadius.z());
        
        collision collisionData (eSpacePosition, eSpaceVelocity, eRadius);
        vector3<float> finalPosition = execute_move (&collisionData); 
        
        /*
        // apply gravity effect
        eSpaceVelocity = vector3<float> (0.0f, 0.0f, gravity / eRadius.z());
        collisionData.update_movement (finalPosition, eSpaceVelocity);
        finalPosition = execute_move (&collisionData); 
         */
        
        // convert final result back to R3
        float x = (finalPosition.x() - 1) * eRadius.x();
        float y = (finalPosition.y() - 1) * eRadius.y();
        float z = (finalPosition.z() - 1) * eRadius.z();

#ifdef DEBUG_COLLISION
        // did we collide at all?
        const triangle3<float> *tri = collisionData.triangle ();
        if (tri != NULL)
        {
            // draw triangle we collided with
            tri->draw (X*SQUARE_SIZE, Y*SQUARE_SIZE, Image);
            
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
