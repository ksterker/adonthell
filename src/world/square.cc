/*
 $Id: square.cc,v 1.7 2008/02/23 20:51:17 ksterker Exp $
 
 Copyright (C) 2002/2007 Alexandre Courbot <alexandrecourbot@linuxgames.com>
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
 * @file   world/square.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the square class.
 * 
 */

#include <algorithm>
#include "world/square.h"

using world::square_info;
using world::square;

// ctor
square_info::square_info (coordinates & pos, const s_int32 & ground_z, const bool & is_base)
 : coordinates (pos) 
{
    GroundZ = ground_z;
    IsBase = is_base;
}

// determine drawing order of map objects
bool square_info::operator < (const square_info & mi) const
{
    if (z_pos () <= mi.GroundZ) return true;
    if (GroundZ >= mi.z_pos ()) return false;
    
    /*
    s_int32 y1 = y() * SQUARE_SIZE + oy() + obj->current_shape()->y() + z_pos();
    s_int32 y2 = mi.y() * SQUARE_SIZE + mi.oy() + mi.obj->current_shape()->y() + mi.z_pos();
    if (y1 < y2) return true;
    if (y1 > y2) return false;
     */
    
    if (y() < mi.y()) return true;
    if (y() > mi.y()) return false;
    if (oy() < mi.oy()) return true;
    if (oy() > mi.oy()) return false;
     
    // If the objects are at the same y position, we better
    // make an arbitrary test to make sure a moving object
    // won't go from behind to before another object when
    // their y coordinates are the same and the x coordinate
    // of the moving object become greater than the other object.
    return false;
}

// add object to cell
bool square::add (placeable * obj, coordinates & pos, const s_int32 & ground_z, const bool & is_base)
{
    square_info mi (pos, ground_z, is_base);
    mi.obj = obj; 
    std::vector<square_info>::iterator it = objects.begin();
    while(it != objects.end() && mi.z_pos () > it->z_pos ()) ++it;
    objects.insert(it, mi);
    return true; 
}

// remove object from cell
bool square::remove (placeable * obj, coordinates & pos)
{
    square_info mi (pos, pos.z(), false);
    mi.obj = obj; 
    std::vector <square_info>::iterator er;
    er = std::find (objects.begin (), objects.end (), mi);
    if (er == objects.end ()) return false;
    objects.erase (er);
    return true; 
}

// check if cell contains object
bool square::exist (placeable * obj, coordinates & pos)
{
    square_info mi (pos, pos.z(), false);
    mi.obj = obj; 
    std::vector <square_info>::iterator er;
    er = std::find (objects.begin (), objects.end (), mi); 
    if (er == objects.end ()) return false;
    return true; 
}
