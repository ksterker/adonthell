/*
 $Id: renderer.cc,v 1.2 2008/11/09 14:07:40 ksterker Exp $
 
 Copyright (C) 2008 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file   world/renderer.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Defines a part of the map visible on screen.
 * 
 */

#include "base/timer.h"
#include "gfx/screen.h"
#include "world/renderer.h"
#include "world/render_info.h"

namespace world
{
    
// default rendering
void default_renderer::render (const s_int16 & x, const s_int16 & y, const std::list <world::chunk_info> & objectlist, const gfx::drawing_area & da, gfx::surface * target) const
{
    std::list <render_info> flat_tiles, wall_tiles, render_queue;
    
    // split objects into "ground" and "wall" tiles
    for (std::list<world::chunk_info>::const_iterator i = objectlist.begin(); i != objectlist.end(); i++)
    {
        for (placeable::iterator obj = i->Object->begin(); obj != i->Object->end(); obj++)
        {
            placeable_shape *shape = (*obj)->current_shape();
            if (shape->is_flat ())
            {
                flat_tiles.push_back (render_info (shape, (*obj)->get_sprite(), i->Min));
            }
            else
            {
                wall_tiles.push_back (render_info (shape, (*obj)->get_sprite(), i->Min));
            }
        }
    }
    
    // sort according to specific drawing order
    flat_tiles.sort ();
    wall_tiles.sort ();
    
    const_iterator it_flat;
    const_iterator flat_end = flat_tiles.end();
    
    // FIXME: we need to figure out with which queue to start
    for (it_flat = flat_tiles.begin (); it_flat != flat_end; it_flat++)
    {
        s_int32 max_x = it_flat->x() + it_flat->Shape->length();
        s_int32 max_y = it_flat->y() + it_flat->Shape->width();
        s_int32 max_z = it_flat->z() + it_flat->Shape->height();        
        
        render_queue.push_back (*it_flat);
    
        for (iterator it_wall = wall_tiles.begin (); it_wall != wall_tiles.end(); /* nothing */)
        {
            // we can draw a wall tile if its bottom right corner is completely on the
            // ground tile we drew last and if there is no other ground tile that would
            // be below that wall
            if (it_wall->x() + it_wall->Shape->length() >= it_flat->x() && it_wall->x() + it_wall->Shape->length() <= max_x &&
                it_wall->y() + it_wall->Shape->width()  >= it_flat->y() && it_wall->y() + it_wall->Shape->width()  <= max_y &&
                it_wall->z() >= max_z && !is_flat_below (it_flat, flat_end, it_wall))
            {
                render_queue.push_back (*it_wall);
                it_wall = wall_tiles.erase (it_wall);
                continue;
            }
            
            it_wall++;
        }
    }    

    // finally do the actual drawing
    for (iterator it = render_queue.begin(); it != render_queue.end(); it++)
    {
        draw (x, y, *it, da, target);
    }
}

// check draw order between floor and wall tiles
bool default_renderer::is_flat_below (const_iterator & begin, const_iterator & end, iterator & it_wall) const
{
    const_iterator it_flat = begin;

    s_int32 max_x = it_wall->x() + it_wall->Shape->length();
    s_int32 max_y = it_wall->y() + it_wall->Shape->width();

    // compare all following flats with the current wall tile
    for (it_flat++; it_flat != end; it_flat++)
    {
        // no intersection possible
        if (it_flat->x() > max_x || it_flat->x() + it_flat->Shape->length() < it_wall->x() ||
            it_flat->y() > max_y || it_flat->y() + it_flat->Shape->width()  < it_wall->y())
            continue;
        
        // intersection on x/y plane, so check z-plane
        if (it_flat->z() + it_flat->Shape->height() <= it_wall->z()) return true;
    }
    
    // there are no floor tiles below this wall tile
    return false;
}

// debug rendering
void debug_renderer::draw (const s_int16 & x, const s_int16 & y, const render_info & obj, const gfx::drawing_area & da, gfx::surface * target) const
{
    default_renderer::draw (x, y, obj, da, target);
    
    if (DrawBBox)
    {
        cube3 bbox (obj.Shape->length(), obj.Shape->width(), obj.Shape->height());
        bbox.draw (x + obj.x(), y + obj.Pos.y () + obj.Shape->y() - obj.z(), &da, target);
    }
    
    if (Print)
    {
        printf ("Pos [%i, %i, %i] ", obj.Pos.x(), obj.Pos.y(), obj.Pos.z());
        printf ("Shape [%i, %i, %i] - [%i, %i, %i] ", obj.Shape->x(), obj.Shape->y(), obj.Shape->z(),
                obj.Shape->x() + obj.Shape->length(), obj.Shape->y() + obj.Shape->width(), obj.Shape->z() + obj.Shape->height());
        printf ("Sprite [%i x %i] %c\n", obj.Sprite->length(), obj.Sprite->height(), (obj.Shape->is_flat() ? '_' : '|'));
    }
    
    if (Delay > 0)
    {
        // debug rendering by drawing one tile at a time
        base::timer::sleep (Delay);
        gfx::screen::update ();
    }
}

}