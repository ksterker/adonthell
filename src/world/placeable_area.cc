/*
 $Id: placeable_area.cc,v 1.1 2007/05/19 07:42:10 ksterker Exp $
 
 Copyright (C) 2002 Alexandre Courbot <alexandrecourbot@linuxgames.com>
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
 * @file   world/placeable_area.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the placeable_area class.
 * 
 * 
 */

#include "placeable_area.h"

using namespace world;

void placeable_area::set_area_size(u_int16 nx, u_int16 ny)
{
    area.resize (nx);
    for (std::vector <std::vector <square_walkable_info> >::iterator i = area.begin ();
         i != area.end (); i++)
        i->resize (ny);
}

u_int16 placeable_area::area_height() const
{
    if (area.size ()) return area[0].size ();
    else return 0; 
}

void placeable_area::put(base::ogzstream & file)
{
    u_int32 i, j;
    area_height() >> file;
    area_length() >> file;
    for (j = 0; j < area_height(); j++)
        for (i = 0; i < area_length(); i++)
            area[i][j].put(file);
    
    base.x() >> file;
    base.y() >> file;
    base.ox() >> file;
    base.oy() >> file;

    zsize >> file;
}

void placeable_area::get(base::igzstream & file)
{
    u_int16 l, h;
    u_int32 i, j;
    h << file;
    l << file;
    
    set_area_size(l, h);
    for (j = 0; j < area_height(); j++)
        for (i = 0; i < area_length(); i++)
            area[i][j].get(file);
    
    u_int16 x, y, ox, oy;
    x << file;
    y << file;
    ox << file;
    oy << file;
    base.set_position(x, y);
    base.set_offset(ox, oy);

    zsize << file;
}
