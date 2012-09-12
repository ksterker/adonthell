/*
 Copyright (C) 2002 Alexandre Courbot <alexandrecourbot@linuxgames.com>
 Copyright (C) 2007/2009 Kai Sterker <kaisterker@linuxgames.com>
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

#include <functional>

#include "moving.h"
#include "area.h"
#include "plane3.h"
#include "shadow.h"
#include "move_event.h"
#include <adonthell/event/manager.h>
#include <adonthell/base/logging.h>

#if DEBUG_COLLISION
#include <adonthell/gfx/gfx.h>
#endif

using world::moving;
using world::area;
using world::chunk_info;
using world::plane3;
using world::vector3;

/// sort chunk_info objects according to the z-position of their surface
struct z_order : public std::binary_function<const chunk_info *, const chunk_info *, bool> 
{
	bool operator() (const chunk_info * a, const chunk_info * b) 
    {
        //Here we want to sort by the top of each solid.  However, if they have the same top,
        //then we want the one with the highest bottom (which is the smallest height).  The reason
        //for this is we have tiles and walls with the same height, but we want to prefer to draw on
        //the tile if the player is standing on it.
        s_int32 atop = a->center_min().z() + a->get_object()->get_surface_pos ();
        s_int32 btop = b->center_min().z() + b->get_object()->get_surface_pos ();
        return (atop > btop) || (atop == btop && a->get_object()->solid_height () < b->get_object()->solid_height ());
    }
};

// ctor
moving::moving (world::area & mymap, const std::string & hash)
    : placeable (mymap, hash), coordinates (), Position(), Velocity()
{
    GroundPos = -10000;
    MyShadow = NULL;
    Terrain = NULL;

#if DEBUG_COLLISION
    Image = gfx::create_surface();
    Image->set_alpha (255, true);
    Image->resize (160, 240);
#endif
}

// dtor
moving::~moving ()
{
    delete MyShadow;
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
    GroundPos = z;
}

// check objects on map for collision
bool moving::collide_with_objects (collision *collisionData)
{
    // bbox around character and projected movement
    const vector3<s_int32> min (
        x() + (Velocity.x() < 0 ? static_cast<s_int32>(floor (Velocity.x())) : 0), 
        y() + (Velocity.y() < 0 ? static_cast<s_int32>(floor (Velocity.y())) : 0), 
        z() + (Velocity.z() < 0 ? static_cast<s_int32>(floor (Velocity.z())) : 0));
                          
    const vector3<s_int32> max (
        min.x() + placeable::length() + (Velocity.x () > 0 ? static_cast<s_int32>(ceil (Velocity.x())) : 0),
        min.y() + placeable::width()  + (Velocity.y () > 0 ? static_cast<s_int32>(ceil (Velocity.y())) : 0),
        min.z() + placeable::height() + (Velocity.z () > 0 ? static_cast<s_int32>(ceil (Velocity.z())) : 0) - 1);

    // get all objects in our path
    const std::list<chunk_info*> & objects = Mymap.objects_in_bbox (min, max);
    
    // check all placeables in our path
    for (std::list<chunk_info*>::const_iterator i = objects.begin(); i != objects.end(); i++)
    {
        const placeable *object = (*i)->get_object();

        // check all models the placeable consists of
        for (placeable::iterator model = object->begin(); model != object->end(); model++)
        {
            // get the model's current shape, ...
            const placeable_shape * shape = (*model)->current_shape ();
            
            // ... and check if collision occurs
            shape->collide (collisionData, (*i)->center_min());
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
    
    // do we need to worry? 
    if (depth > 5) 
    {
        return pos;
    }
    
    // check for collision
    if (!collide_with_objects (collisionData))
    { 
        // if no collision occurred, we just move along the velocity
        return pos + vel;
    }
    
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
    
    //TODO This is just a temporary fix that prevents sliding on any slope no matter how small
    // it should probably be set per-terrain
    static float nvcd = 0.035f;

    // don't recurse if the new velocity is very small 
    if (newVelocityVector.length() < nvcd)
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
    static float gravity = -4.905f;
    
    // calculate radius of ellipsoid
    const vector3<float> eRadius (placeable::length() / 2.0f, width() / 2.0f, height() / 2.0f);
    
    // calculate position (= center of ellipse --> + 1) and velocity in eSpace 
    vector3<float> eSpacePosition (Position.x() / eRadius.x() + 1, Position.y() / eRadius.y() + 1, Position.z() / eRadius.z() + 1);
    vector3<float> eSpaceVelocity (Velocity.x() / eRadius.x(), Velocity.y() / eRadius.y(), Velocity.z() / eRadius.z());

#if DEBUG_COLLISION
    printf ("    pos [%.3f, %.3f, %.3f]\n", eSpacePosition.x() * eRadius.x(), eSpacePosition.y() * eRadius.y(), (eSpacePosition.z() - 1) * eRadius.z());
#endif
    
    collision collisionData (eSpacePosition, eSpaceVelocity, eRadius);
    vector3<float> finalPosition = execute_move (&collisionData);         
        
#if DEBUG_COLLISION
    gfx::drawing_area da (0, 0, Image->length() - 1, Image->height() - 1);

    // create character bounding box
    cube3 bbox (placeable::length(), width(), height());

    // center on debug image
    u_int16 pos_x = (Image->length() - bbox.max_x()) / 2;
    u_int16 pos_y = (Image->height() - bbox.max_z() + bbox.max_y()) / 2;
        
    bbox.draw (pos_x, pos_y, &da, Image);
    
    // center on character shape (ground level)
    pos_x += placeable::length()/2;
    pos_y += placeable::width()/2;

    // draw position of character
    Image->draw_line (pos_x - 5, pos_y, pos_x + 5, pos_y, Image->map_color (255, 255, 0), &da);                           
    Image->draw_line (pos_x, pos_y - 5, pos_x, pos_y + 5, Image->map_color (255, 255, 0), &da);                           
    
    // actual center of the character
    pos_y -= placeable::height()/2;
    
    // draw velocity along x,y axis
    Image->draw_line (pos_x, pos_y, (u_int16) (pos_x + Velocity.x()*20), (u_int16) (pos_y + Velocity.y() * 20), Image->map_color (0, 0, 255), &da);

    // draw gravity
    Image->draw_line (pos_x, pos_y, pos_x, (u_int16) (pos_y - gravity * 20), Image->map_color (255, 0, 0), &da);        

    // draw velocity along z axis
    Image->draw_line (pos_x, pos_y, pos_x, (u_int16) (pos_y - Velocity.z() * 20), Image->map_color (0, 0, 255), &da);        

    // did we collide at all?
    const triangle3<float> *tri = collisionData.triangle ();
    if (tri != NULL)
    {
        // draw triangle we collided with
        tri->draw (pos_x, pos_y, Image);
    }
    
    printf ("    pos [%.3f, %.3f, %.3f]\n", finalPosition.x() * eRadius.x(), finalPosition.y() * eRadius.y(), (finalPosition.z() - 1) * eRadius.z());    
#endif

    // our velocity for gravity calculation
    float vz = Velocity.z();
    Velocity.set_z (gravity);

    // apply gravity effect    
    eSpaceVelocity = vector3<float> (0.0f, 0.0f, gravity / eRadius.z());
    collisionData.update_movement (finalPosition, eSpaceVelocity);
    finalPosition = execute_move (&collisionData); 

    // restore velocity
    Velocity.set_z (vz);
    
    // convert final result back to R3
    float x = (finalPosition.x() - 1) * eRadius.x();
    float y = (finalPosition.y() - 1) * eRadius.y();
    float z = (finalPosition.z() - 1) * eRadius.z();
        
#if DEBUG_COLLISION
    if (tri != NULL)
    {
        // draw actual movement along x,y axis
        Image->draw_line (pos_x, pos_y, (u_int16) (pos_x + (x - Position.x()) * 20), (u_int16) (pos_y + (y - Position.y()) * 20), Image->map_color (0, 255, 0), &da);
    
        // draw actual movement along z axis
        Image->draw_line (pos_x, pos_y, pos_x, (u_int16) (pos_y - (z - Z) * 20), Image->map_color (0, 255, 0), &da);
    }
#endif

    // update position on map, which must be in whole pixels     
    X = (s_int32) round(x);
    Y = (s_int32) round(y);
    Z = (s_int32) round(z);

    // calculate ground position and update shadow cast by ourself
    calculate_ground_pos ();
            
    // update precise location for next iteration
    Position.set (x, y, z);
}

// calculate z position of ground
void moving::calculate_ground_pos ()
{
    // bbox of everything below our character
    // 1 pixel in every direction less then placeable's size
    // to avoid including objects that just barely touch it
    const vector3<s_int32> min (x() + 1, y() + 1, Mymap.min().z());
    const vector3<s_int32> max (min.x() + placeable::length() - 2, min.y() + placeable::width() - 2, z() - 1);
    
    // get objects below us
    std::list<chunk_info*> ground_tiles = Mymap.objects_in_bbox (min, max, OBJECT);
    
    // discard all completely non-solid objects
    std::list<chunk_info*>::iterator ci = ground_tiles.begin();
    while (ci != ground_tiles.end())
    {
        if ((*ci)->get_object()->is_solid())
        {
            ci++;
        }
        else
        {
            ci = ground_tiles.erase (ci);
        }
    }
    
    if (!ground_tiles.empty ())
    {
        // prepare shadow
        MyShadow->init ();

        // sort according to their z-Order
        ground_tiles.sort (z_order());

        // the highest object will be our ground pos
        ci = ground_tiles.begin ();

        MyShadow->cast_on (*ci);

        // position of character's center relative to tile
        s_int32 px = x() + placeable::length()/2 - (*ci)->center_min().x();
        s_int32 py = y() + placeable::width()/2 - (*ci)->center_min().y();

        // get ground pos
        GroundPos = (*ci)->center_min().z() + (*ci)->get_object()->get_surface_pos (px, py);

        // get the terrain, if any
        Terrain = (*ci)->get_object()->get_terrain();

        // apply remainder of shadow
        for (ci++; ci != ground_tiles.end(); ci++)
        {
            MyShadow->cast_on (*ci);
        }
    }
    else
    {
        // there are no objects below ... this also means we will
        // drop out of the world, so here could be a good place
        // to avoid this. But for now we'll just let it happen ...
        GroundPos = Z - 1;
    }
}

// update movable position
bool moving::update ()
{
    // this is a dummy, as we don't know the real entity
    named_entity e (this, "", false);
    
#if DEBUG_COLLISION
    // clear image
    Image->fillrect (0, 0, Image->length() - 1, Image->height() - 1, Image->map_color (0, 0, 0, 0));
#endif
    
    // we can skip the whole collision stuff if we're not moving
    if (vx() != 0.0f || vy() != 0.0f || vz() != 0.0f || GroundPos != Z)
    {
        // reset shadow for next frame
        MyShadow->reset ();

        // prepare move notification (before the move takes place!)
        world::move_event evt (this);

        entity *myEntity = Mymap.remove (&e, *this);
        if (myEntity != NULL)
        {
            update_position ();
            Mymap.add (myEntity, *this);

            // notify interested parties about movement on the map
            events::manager::raise_event(&evt);
        }
    }
    
    return true; 
}

// debugging
void moving::debug_collision (const u_int16 & x, const u_int16 & y) const
{
#if DEBUG_COLLISION
    Image->draw (x, y);
#endif
}

