/*
 $Id: placeable_shape.cc,v 1.2 2007/12/09 21:39:43 ksterker Exp $
 
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

#include "world/placeable_shape.h"

using world::placeable_shape;

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

// check for collision
void placeable_shape::collide (collision * collisionData) const
{
    for (std::vector<cube3*>::const_iterator i = Parts.begin(); i != Parts.end (); i++)
	{
        // check each part of the shape
        (*i)->collide (collisionData);
	}    
}

// save to stream
bool placeable_shape::put_state (base::flat & file) const
{
	base::flat record;
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
            return false;
        }
    }
	
    return true;
}
