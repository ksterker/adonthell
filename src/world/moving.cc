/*
 $Id: moving.cc,v 1.18 2008/09/14 14:25:25 ksterker Exp $
 
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
#include <functional>

#include "world/moving.h"
#include "world/area.h"
#include "world/plane3.h"

#if DEBUG_COLLISION
#include "gfx/gfx.h"
#endif

using world::moving;
using world::area;
using world::chunk_info;
using world::plane3;
using world::vector3;

/// sort chunk_info objects according to the z-position of their top
struct z_order : public std::binary_function<const chunk_info &, const chunk_info &, bool> 
{
	bool operator() (const chunk_info & a, const chunk_info & b) 
    {
        return a.Max.z() > b.Max.z();
    }
};

// ctor
moving::moving (world::area & mymap)
    : placeable (mymap), coordinates ()
{
#if DEBUG_COLLISION
    Image = gfx::create_surface();
    Image->set_alpha (255, true);
    Image->resize (gfx::screen::length(), gfx::screen::height());
#endif
}

// dtor
moving::~moving ()
{
#if DEBUG_COLLISION
    delete Image;
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

// set x,y coordinates
void moving::set_position (const s_int32 & x, const s_int32 & y)
{
    // pixel-based location
    coordinates::set_x (x);
    coordinates::set_y (y);
    
    // precise location
    Position.set_x (x);
    Position.set_y (y);
}

// set z position
void moving::set_altitude (const s_int32 & z)
{
    coordinates::set_z (z);
    Position.set_z (z);
}

// check objects on map for collision
bool moving::collide_with_objects (collision *collisionData)
{
    // bbox around character and projected movement
    vector3<s_int32> min (
        x() + (Velocity.x() < 0 ? floor (Velocity.x()) : 0), 
        y() + (Velocity.y() < 0 ? floor (Velocity.y()) : 0), 
        z() + (Velocity.z() < 0 ? floor (Velocity.z()) : 0));
                          
    vector3<s_int32> max (
        min.x() + placeable::length() + (Velocity.x () > 0 ? ceil (Velocity.x()) : 0),
        min.y() + placeable::width() + (Velocity.y () > 0 ? ceil (Velocity.y()) : 0),
        min.z() + placeable::height() + (Velocity.z () > 0 ? ceil (Velocity.z()) : 0));

#if DEBUG_COLLISION
    gfx::surface *area = gfx::create_surface();
    area->set_alpha (128);
    area->set_mask (true);
    area->resize (gfx::screen::length(), gfx::screen::height());
    area->fillrect (0, 0, area->length(), area->height(), gfx::screen::trans_color());
    
    // show area used for collision detection    
    area->fillrect (min.x(), min.y() - max.z(),
                     max.x() - min.x(), (max.y() - min.y()) + (max.z() - min.z()),
                     Image->map_color (0, 0, 128, 128));
    cube3 box (max.x() - min.x(), max.y() - min.y(), max.z() - min.z());
    box.draw (min.x(), min.y() - min.z(), NULL, area);
    
    area->draw (0, 0, NULL, Image);
    delete area;
#endif

    // get all objects in our path
    std::list<chunk_info> objects = Mymap.objects_in_bbox (min, max);
    
    // check all placeables in our path
    for (std::list<chunk_info>::const_iterator i = objects.begin(); i != objects.end(); i++)
    {
        // check all models the placeable consists of
        for (placeable::iterator model = i->Object->begin(); model != i->Object->end(); model++)
        {
            // get the model's current shape, ...
            const placeable_shape * shape = (*model)->current_shape ();
            
            // ... get offset of shape from current position ...                
            const vector3<s_int16> offset = i->Min - *this;
                                           
            // ... and check if collision occurs
            shape->collide (collisionData, offset);
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

#if DEBUG_COLLISION
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
    
#if DEBUG_COLLISION
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
    
    // calculate radius of ellipsoid
    const vector3<float> eRadius (placeable::length() / 2.0, width() / 2.0, height() / 2.0);
    
    // calculate position (= center of ellipse --> + 1) and velocity in eSpace 
    vector3<float> eSpacePosition ((Position.x() /* + shape->x()*/) / eRadius.x() + 1, (Position.y() /*+ shape->y()*/) / eRadius.y() + 1, Position.z() / eRadius.z() + 1);
    vector3<float> eSpaceVelocity (Velocity.x() / eRadius.x(), Velocity.y() / eRadius.y(), Velocity.z() / eRadius.z());
    
    collision collisionData (eSpacePosition, eSpaceVelocity, eRadius);
    vector3<float> finalPosition = execute_move (&collisionData);         
        
#if DEBUG_COLLISION
    gfx::drawing_area da (0, 0, Image->length() - 1, Image->height() - 1);

    u_int16 pos_x = X/*+shape->x()*/;
    u_int16 pos_y = Y/*+shape->y()*/-Z;
    
    // draw character bounding box
    cube3 bbox (placeable::length(), width(), height());
    bbox.draw (pos_x, pos_y, &da, Image);
    
    /* pos_x += shape->length()/2;
    pos_y += shape->width()/2; */

    // draw position of character
    Image->draw_line (pos_x - 5, pos_y, pos_x + 5, pos_y, Image->map_color (255, 255, 0), &da);                           
    Image->draw_line (pos_x, pos_y - 5, pos_x, pos_y + 5, Image->map_color (255, 255, 0), &da);                           
                      
    /* pos_y -= shape->height()/2; */
    
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
        tri->draw (X, Y, Image);
    }
#endif

    // apply gravity effect
    eSpaceVelocity = vector3<float> (0.0f, 0.0f, gravity / eRadius.z());
    collisionData.update_movement (finalPosition, eSpaceVelocity);
    finalPosition = execute_move (&collisionData); 
            
    // convert final result back to R3
    float x = (finalPosition.x() - 1) * eRadius.x() /*- shape->x()*/;
    float y = (finalPosition.y() - 1) * eRadius.y() /*- shape->y()*/;
    float z = (finalPosition.z() - 1) * eRadius.z();
        
#if DEBUG_COLLISION
    if (tri != NULL)
    {
        // draw actual movement along x,y axis
        Image->draw_line (pos_x, pos_y, pos_x + (x - Position.x()) * 20, pos_y + (y - Position.y()) * 20, Image->map_color (0, 255, 0), &da);
    
        // draw actual movement along z axis
        Image->draw_line (pos_x, pos_y, pos_x, pos_y - (z - Z) * 20, Image->map_color (0, 255, 0), &da);
    }
#endif

    // update position on map, which must be in whole pixels     
    X = (s_int32) x;
    Y = (s_int32) y;
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

// calculate z position of ground
void moving::calculate_ground_pos ()
{
    // bbox below our center
    vector3<s_int32> min (X + placeable::length()/2, Y + width()/2, -10000);
    vector3<s_int32> max (X + placeable::length()/2, Y + width()/2, Z);
    
    // get objects below us
    std::list<chunk_info> result = Mymap.objects_in_bbox (min, max);

    // sort according to their z-Order
    result.sort (z_order());
    
    // the topmost object will be our ground pos
    GroundPos = result.front().Max.z();
}

// update movable position
bool moving::update ()
{
#if DEBUG_COLLISION
    // clear image
    Image->fillrect (0, 0, Image->length() - 1, Image->height() - 1, Image->map_color (0, 0, 0, 0));
#endif
    
    // we can skip the whole collision stuff if we're not moving
    if (vx() != 0.0f || vy() != 0.0f || vz() != 0.0f || GroundPos != Z)
    {
        Mymap.remove (this, *this);
        update_position ();
        Mymap.add (this, *this);
    }
    
    return true; 
}

// debugging
void moving::debug_collision ()
{
#if DEBUG_COLLISION
    Image->draw (0, 0);
#endif
}

