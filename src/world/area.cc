/*
 $Id: area.cc,v 1.5 2007/10/15 02:19:30 ksterker Exp $
 
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
 * @file   world/area.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the area class.
 * 
 */

#include "world/area.h"

using world::coordinates;
using world::area;
using world::object;
using world::character;

area::~area()
{
    clear();
}

void area::clear()
{
    objects.clear();
    characters.clear();
}

void area::resize (const u_int16 nx, const u_int16 ny) 
{
    Grid.resize (nx);
    for (std::vector <std::vector <square> >::iterator i = Grid.begin ();
         i != Grid.end (); i++)
        i->resize (ny); 
}

world::square * area::get (const u_int16 x, const u_int16 y) 
{
    return (&Grid[x][y]); 
}

// place object at given position on the grid
bool area::put (placeable * obj, coordinates & pos)
{
    placeable_shape *shape = obj->current_shape ();

    if (!shape) return false;

    // calculate area of map squares occupied by the object's bounding box
    u_int16 start_x = pos.x (); 
    u_int16 start_y = pos.y (); 
    u_int16 end_x = start_x + ceil ((pos.ox () + shape->length ()) / (float) SQUARE_SIZE);
    u_int16 end_y = start_y + ceil ((pos.oy () + shape->width ()) / (float) SQUARE_SIZE); 

    // make sure we do not exceed map size
    if (end_x > length()) end_x = length() - 1;
    if (end_y > height()) end_y = height() - 1;

    square *sq; 
    
    // add object to all these squares
    for (u_int16 j = start_y; j < end_y; j++)
    {
        for (u_int16 i = start_x; i < end_x; i++) 
        {
            sq = get (i, j);
            coordinates shape_offset (i - start_x, j - start_y, pos.z(), 0, 0);
            sq->add (obj, pos); 
        }
    }
    
    return true; 
}

bool area::put (moving * obj) 
{
    // FIXME (implement like above
    
    /*
    u_int16 i, j;
    placeable_shape * state = obj->current_shape ();

    if (!state) return false;

    u_int16 sx = obj->x () < state->base.x () ? 0 :
        obj->x () - state->base.x (); 
    u_int16 sy = obj->y () < state->base.y () ? 0 :
        obj->y () - state->base.y (); 
    
    u_int16 fx = obj->x () + state->area_length () - state->base.x ();
    u_int16 fy = obj->y () + state->area_height () - state->base.y (); 

    if (obj->ox ()) fx++;
    if (obj->oy ()) fy++; 
    
    if (fx > length()) fx = length() - 1;
    if (fy > height()) fy = height() - 1;

    square * msqr; 
    
    for (j = sy; j < fy; j++) 
        for (i = sx; i < fx; i++) 
        {
            msqr = get (i, j);
            msqr->add (obj); 
        }
    
    */
    return true; 
}

bool area::remove (placeable * obj, coordinates & pos) 
{
    placeable_shape *shape = obj->current_shape ();
    if (!shape) return false;

    // calculate area of map squares occupied by the object's bounding box
    u_int16 start_x = pos.x (); 
    u_int16 start_y = pos.y ();    
    u_int16 end_x = start_x + ceil ((pos.ox () + shape->length ()) / (float) SQUARE_SIZE);
    u_int16 end_y = start_y + ceil ((pos.oy () + shape->width ()) / (float) SQUARE_SIZE); 
    
    // make sure we do not exceed map size
    if (end_x > length()) end_x = length() - 1;
    if (end_y > height()) end_y = height() - 1;
    
    square *sq; 
    
    for (u_int16 j = start_y; j < end_y; j++) 
    {
        for (u_int16 i = start_x; i < end_x; i++) 
        {
            sq = get (i, j);
            coordinates shape_offset (i - start_x, j - start_y, pos.z(), 0, 0);
            sq->remove (obj, pos); 
        }
    }
    
    return true; 
}

bool area::remove (moving * obj) 
{
    return remove (obj, *obj); 
}

void area::update()
{
    objects.update();
    characters.update();
}

object * area::add_object()
{    
    return objects.add(*this);
}

character * area::add_character()
{
    return characters.add(*this);
}

bool area::put_object(u_int32 index, coordinates & pos)
{
    put(objects[index], pos);
    return true;
}

void area::output_occupation()
{
    for (u_int16 j = 0; j < height(); j++)
    {
        for (u_int16 i = 0; i < length(); i++)
        {
            square * msqr = get(i, j);
            u_int16 nb = 0;
            for (square::iterator it = msqr->begin(); it != msqr->end(); it++)
                nb++;
            std::cout << nb << " ";
        }
        std::cout << std::endl;
    }
}
