/*
 $Id: square.cc,v 1.3 2007/10/15 02:19:34 ksterker Exp $
 
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
square_info::square_info (coordinates & pos)
 : coordinates (pos) 
{
    zground = pos.z();
}

bool square_info::operator < (const square_info & mi) const
{
//     if (y() == mi.y() || (y() == mi.y() + 1 && mi.oy()) &&
//         z() + obj->current_shape()->zsize <= mi.z()) return true;

//     if (mi.y() == y() || (mi.y() == y() + 1 && oy()) &&
//         z() >= mi.z() + mi.obj->current_shape()->zsize) return false;

    if (z() + obj->current_shape()->height() <= mi.zground) return true;
    if (zground >= mi.z() + mi.obj->current_shape()->height()) return false;
    if (y() < mi.y()) return true;
    if (y() > mi.y()) return false;
    if (oy() < mi.oy()) return true;
    if (oy() > mi.oy()) return false;

//     if (obj->type() == OBJECT) return true;
//     if (obj->type() == CHARACTER) return false;
//     if (z() + obj->current_shape()->zsize < mi.z() + mi.obj->current_shape()->zsize) return true;
//     if (z() + obj->current_shape()->zsize > mi.z() + mi.obj->current_shape()->zsize) return false;

//     if ((y() * square_size) + oy() + square_size <=
//         (mi.y() - (mi.obj->current_shape()->area_height() - 1)) * square_size + mi.oy()) return true;

//     if ((y() - (obj->current_shape()->area_height() - 1)) * square_size + oy() >=
//         square_size + mi.y() * square_size + mi.oy()) return false;

    // If the objects are at the same y position, we better
    // make an arbitrary test to make sure a moving object
    // won't go from behind to before another object when
    // their y coordinates are the same and the x coordinate
    // of the moving object become greater than the other object.
    return false;
}

bool square::add (placeable * obj, coordinates & pos)
{
    square_info mi (pos);
    mi.obj = obj; 
    std::vector<square_info>::iterator it = objects.begin();
    while(it != objects.end() && mi.z() + mi.obj->current_shape()->height() < 
          it->z() + it->obj->current_shape()->height()) ++it;
    objects.insert(it, mi);
    return true; 
}

bool square::add (moving * obj)
{
    square_info mi (*obj);
    mi.obj = obj; 
    mi.zground = obj->zground;
    std::vector<square_info>::iterator it = objects.begin();
    while(it != objects.end() && mi.z() + mi.obj->current_shape()->height() < 
          it->z() + it->obj->current_shape()->height()) ++it;
    objects.insert(it, mi);
    return true; 
}

bool square::remove (placeable * obj, coordinates & pos)
{
    square_info mi (pos);
    mi.obj = obj; 
    std::vector <square_info>::iterator er;
    er = std::find (objects.begin (), objects.end (), mi);
    if (er == objects.end ()) return false;
    objects.erase (er);
    return true; 
}

bool square::exist (placeable * obj, coordinates & pos)
{
    square_info mi (pos);
    mi.obj = obj; 
    std::vector <square_info>::iterator er;
    er = std::find (objects.begin (), objects.end (), mi); 
    if (er == objects.end ()) return false;
    return true; 
}
