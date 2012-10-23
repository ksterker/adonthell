/*
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
 * @file   world/placeable_shape.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Defines the placeable_shape class.
 * 
 */

#include "placeable_shape.h"

using world::placeable_shape;

// dtor
placeable_shape::~placeable_shape()
{
    for (std::vector<cube3*>::const_iterator i = Parts.begin(); i != Parts.end (); i++)
	{
        delete *i;
    }
    Parts.clear();
}

// add a part to object shape
void placeable_shape::add_part (world::cube3 * part)
{
    if (Parts.empty ())
    {
        // init bounding box
        Min.set (part->min_x(), part->min_y(), part->min_z());
        Max.set (part->max_x(), part->max_y(), part->max_z());
    }
    else
    {
        // update bounding box of shape
        if (Min.x () > part->min_x ()) Min.set_x (part->min_x ());
        if (Max.x () < part->max_x ()) Max.set_x (part->max_x ());
        if (Min.y () > part->min_y ()) Min.set_y (part->min_y ());
        if (Max.y () < part->max_y ()) Max.set_y (part->max_y ());
        if (Min.z () > part->min_z ()) Min.set_z (part->min_z ());
        if (Max.z () < part->max_z ()) Max.set_z (part->max_z ());
    }
    
    Parts.push_back (part);
}

// remove a part from object shape
void placeable_shape::remove_part (world::cube3 * part)
{
    std::vector<cube3*>::iterator i;
    for (i = Parts.begin(); i != Parts.end(); i++)
    {
        if (*i == part)
        {
            Parts.erase (i);
            break;
        }
    }
    
    if (Parts.empty ())
    {
        // reset bounding box
        Min.set (0, 0, 0);
        Max.set (0, 0, 0);
    }
    else
    {
        // we swap min and max and go from there ...
        vector3<s_int32> tmp = Min;
        Min.set (Max.x (), Max.y (), Max.z ());
        Max.set (tmp.x (), tmp.y (), tmp.z ());
        
        // update bounding box from remaining shapes
        for (i = Parts.begin(); i != Parts.end(); i++)
        {
            if (Min.x () > (*i)->min_x ()) Min.set_x ((*i)->min_x ());
            if (Max.x () < (*i)->max_x ()) Max.set_x ((*i)->max_x ());
            if (Min.y () > (*i)->min_y ()) Min.set_y ((*i)->min_y ());
            if (Max.y () < (*i)->max_y ()) Max.set_y ((*i)->max_y ());
            if (Min.z () > (*i)->min_z ()) Min.set_z ((*i)->min_z ());
            if (Max.z () < (*i)->max_z ()) Max.set_z ((*i)->max_z ());
        }
    }
}

// check for collision
void placeable_shape::collide (collision * collisionData, const vector3<s_int16> & offset) const
{
    if (Solid)
    {
        for (std::vector<cube3*>::const_iterator i = Parts.begin(); i != Parts.end (); i++)
        {
            // check each part of the shape
            (*i)->collide (collisionData, offset);
        }
    }
}

// check for intersection
bool placeable_shape::intersects (const placeable_shape *other, const vector3<s_int32> & offset) const
{
    for (std::vector<cube3*>::const_iterator i = Parts.begin(); i != Parts.end (); i++)
    {
        for (std::vector<cube3*>::const_iterator j = other->begin(); j != other->end (); j++)
        {
            // simple bbox check for now, but we might get fancier later ...
            if (((*i)->min_x() + offset.x()) < (*j)->max_x() && ((*i)->max_x() + offset.x()) > (*j)->min_x() &&
                ((*i)->min_y() + offset.y()) < (*j)->max_y() && ((*i)->max_y() + offset.y()) > (*j)->min_y() &&
                ((*i)->min_z() + offset.z()) < (*j)->max_z() && ((*i)->max_z() + offset.z()) > (*j)->min_z())
            {
                return true;
            }
        }
    }

    return false;
}

s_int32 placeable_shape::bottom (const s_int32 & x1, const s_int32 & y1, const s_int32 & x2, const s_int32 & y2) const
{
    s_int32 result = Max.z();

    for (std::vector<cube3*>::const_iterator i = Parts.begin(); i != Parts.end (); i++)
    {
        if ((*i)->max_x() >= x1 && (*i)->min_x() <= x2 &&
            (*i)->max_y() >= y1 && (*i)->min_y() <= y2 &&
            result > (*i)->min_z())
        {
            result = (*i)->min_z();
        }
    }

    return result == Max.z() ? Min.z() : result;
}

// save to stream
bool placeable_shape::put_state (base::flat & file) const
{
	base::flat record;
    
    if (!Solid)
    {
        record.put_bool ("non_solid", true);
    }
    
    if (ox() || oy())
    {
        Offset.put_state (record, "offset");
    }
    
    record.put_uint16 ("num", Parts.size());
	for (std::vector<cube3*>::const_iterator i = Parts.begin(); i != Parts.end (); i++)
	{
		(*i)->put_state (record);
	}
	
	file.put_flat ("shape", record);
    return true;
}

// load from stream
bool placeable_shape::get_state (base::flat & file)
{
	base::flat record = file.get_flat ("shape");
    if (!file.success ()) return false;

    // optional non-solid flag, false if missing
    Solid = !record.get_bool ("non_solid", true);
    
    // get optional sprite offset
    std::string offset = record.get_string ("offset", true);
    if (offset.size() > 0)
    {
        Offset.set_str (offset);
    }
        
    u_int16 size = record.get_uint16 ("num");
    for (u_int16 i = 0; i < size; i++)
    {
        cube3 *part = new cube3 (0, 0, 0);
        if (part->get_state (record))
        {
            add_part (part);
        }
        else
        {
            delete part;
            return false;
        }
    }
	
    return true;
}
