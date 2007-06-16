/*
 $Id: area.cc,v 1.4 2007/06/16 23:19:01 ksterker Exp $
 
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
    u_int16 i, j;
    placeable_area * state = obj->current_state ();

    if (!state) return false;

    u_int16 start_x = pos.x () < state->base.x () ? 0 : pos.x () - state->base.x (); 
    u_int16 start_y = pos.y () < state->base.y () ? 0 : pos.y () - state->base.y (); 
    
    u_int16 end_x = pos.x () + state->area_length () - state->base.x ();
    u_int16 end_y = pos.y () + state->area_height () - state->base.y (); 

    if (pos.ox ()) end_x++;
    if (pos.oy ()) end_y++; 
    
    if (end_x > length()) end_x = length() - 1;
    if (end_y > height()) end_y = height() - 1;

    square * msqr; 
    
    for (j = start_y; j < end_y; j++) 
        for (i = start_x; i < end_x; i++) 
        {
            msqr = get (i, j);
            // coordinates new_pos(pos.x() + i - start_x, pos.y() + j - start_y, pos.z(), 0, 0);
            msqr->add (obj, pos); 
        }

    return true; 
}

bool area::put (moving * obj) 
{
    u_int16 i, j;
    placeable_area * state = obj->current_state ();

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

    return true; 

    //    return put (obj, *obj); 
}

bool area::remove (placeable * obj, coordinates & pos) 
{
    u_int16 i, j;
    placeable_area * state = obj->current_state ();

    if (!state) return false;

    u_int16 sx = pos.x () < state->base.x () ? 0 :
        pos.x () - state->base.x (); 
    u_int16 sy = pos.y () < state->base.y () ? 0 :
        pos.y () - state->base.y (); 
    
    u_int16 fx = pos.x () + state->area_length () - state->base.x ();
    u_int16 fy = pos.y () + state->area_height () - state->base.y (); 

    if (pos.ox ()) fx++;
    if (pos.oy ()) fy++; 
    
    if (fx > length()) fx = length() - 1;
    if (fy > height()) fy = height() - 1;

    square * msqr; 
    
    for (j = sy; j < fy; j++) 
        for (i = sx; i < fx; i++) 
        {
            msqr = get (i, j);
            msqr->remove (obj, pos); 
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
