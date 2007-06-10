/*
 $Id: placeable_area.cc,v 1.4 2007/06/10 03:58:22 ksterker Exp $
 
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
 */

#include "world/placeable_area.h"

using world::placeable_area;

// change area size
void placeable_area::set_area_size(u_int16 nx, u_int16 ny)
{
    area.resize (nx);
    for (std::vector <std::vector <square_walkable_info> >::iterator i = area.begin ();
         i != area.end (); i++)
        i->resize (ny);
}

// get y size of area
u_int16 placeable_area::area_height() const
{
    if (area.size ()) return area[0].size ();
    else return 0; 
}

// save to stream
bool placeable_area::put_state (base::flat & file) const
{
    u_int16 len = area_length();
    u_int16 hgt = area_height();
    
    // save area size
    file.put_uint16 ("length", len);
    file.put_uint16 ("height", hgt);
    file.put_uint16 ("size", zsize);

    for (u_int16 j = 0; j < hgt; j++)
        for (u_int16 i = 0; i < len; i++)
            area[i][j].put_state (file);

    // save position
    file.put_uint16 ("x", base.x());
    file.put_uint16 ("y", base.y());
        
    // save offset
    file.put_uint16 ("x_off", base.ox());
    file.put_uint16 ("y_off", base.oy());

    return true;
}

// load from stream
bool placeable_area::get_state (base::flat & file)
{
    // get area size
    u_int16 var1 = file.get_uint16 ("length");
    u_int16 var2 = file.get_uint16 ("height");
    zsize = file.get_uint16 ("size");
    set_area_size (var1, var2);

    printf (" %i %i", var1, var2);
    fflush (stdout);
    
    for (u_int16 i = 0; i < var1; i++)
        for (u_int16 j = 0; j < var2; j++)
            area[i][j].get_state (file);
    
    // get position
    var1 = file.get_uint16 ("x");
    var2 = file.get_uint16 ("y");
    base.set_position (var1, var2);

    printf (" %i %i", var1, var2);

    // get offset
    var1 = file.get_uint16 ("x_off");
    var2 = file.get_uint16 ("y_off");
    base.set_offset (var1, var2);

    printf (" %i %i", var1, var2);

    return file.success ();
}
