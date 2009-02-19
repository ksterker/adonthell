/*
 $Id: renderer.cc,v 1.8 2009/02/19 07:25:48 ksterker Exp $
 
 Copyright (C) 2008/2009 Kai Sterker <kaisterker@linuxgames.com>
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

namespace world
{
    
// default rendering
void default_renderer::render (const s_int16 & x, const s_int16 & y, const std::list <world::chunk_info*> & objectlist, const gfx::drawing_area & da, gfx::surface * target) const
{
    std::list <render_info> render_queue;

    // populate render queue
    for (std::list<world::chunk_info*>::const_iterator i = objectlist.begin(); i != objectlist.end(); i++)
    {
        for (placeable::iterator obj = (*i)->Object->begin(); obj != (*i)->Object->end(); obj++)
        {
            render_queue.push_back (render_info ((*obj)->current_shape(), (*obj)->get_sprite(), (*i)->Min, (*i)->get_shadow()));
        }
    }
    
    // paint while object remain in the queue
    while (!render_queue.empty())
    {
        int size = render_queue.size();

        // check each object if it can be drawn
        for (iterator it = render_queue.begin(); it != render_queue.end(); /* nothing */)
        {
            const_iterator begin = render_queue.begin();
            const_iterator end = render_queue.end();

            // an object can be drawn if it cannot possibly collide with another object in the queue
            if (can_draw_object (*it, begin, end))
            {
                // draw and remove from queue
                draw (x, y, *it, da, target);
                it = render_queue.erase (it);
                continue;
            }
            
            it++;
        }
   
        // should not happen, but does lead to a deadlock
        if (size == render_queue.size())
        {
            fprintf (stderr, "*** warning: deadlock during rendering detected!\n");
            for (iterator it = render_queue.begin(); it != render_queue.end(); it++)
                fprintf (stderr, "  - (%i, %i, %i) - (%i, %i, %i)\n", it->x(), it->y(), it->z(), it->x() + it->Shape->length(), it->y() + it->Shape->width(), it->z() + it->Shape->height());

            draw (x, y, render_queue.front(), da, target);
            render_queue.pop_front();
        }
    }
}

// check if object can be rendered
bool default_renderer::can_draw_object (render_info & obj, const_iterator & begin, const_iterator & end) const
{
    // compare given object with all objects remaining in the draw queue
    for (const_iterator it = begin; it != end; it++)
    {
        // ... but not with itself
        if (&(*it) == &obj) continue;
        
        // if objects don't overlap, we're still good
        if (obj.min_x()  >= it->max_x()  ||
            obj.min_yz() >= it->max_yz() ||
            it->min_x()  >= obj.max_x()  ||
            it->min_yz() >= obj.max_yz())
            continue;

        // objects do overlap, so we need to figure out position of objects 
        // relative to each other
        if (is_object_below (*it, obj.x(), obj.y(), obj.z(), 
                obj.x() + obj.Shape->length(), obj.y() + obj.Shape->width(), obj.z() + obj.Shape->height())) 
            return false;
    }
    
    // there are no objects in the way, so we can draw
    return true;
}

// check object order
bool default_renderer::is_object_below (const render_info & obj, const s_int32 & min_x, const s_int32 & min_y, const s_int32 & min_z, const s_int32 & max_x, const s_int32 & max_y, const s_int32 & max_z) const
{
    // 2 | 3 | 4   y           x | F | F
    // --+---+--   ^           --+---+--
    // 1 | 8 | 5   |           T | o | F
    // --+---+--   |           --+---+--
    // 0 | 7 | 6   +-----> z   T | T | x

    if (obj.y() + obj.Shape->width() <= min_y)
    {                 
        if (obj.z() + obj.Shape->height() <= min_z) // 0
        {
            return true;
        }
        else if (obj.z() >= max_z) // 6
        {
            fprintf (stderr, "*** default_renderer::is_object_below: objects do not overlap!\n");
            fprintf (stderr, "    [%i, %i, %i] - [%i, %i, %i]\n", min_x, min_y, min_z, max_x, max_y, max_z);
            fprintf (stderr, "    (%i, %i, %i) - (%i, %i, %i)\n", obj.x(), obj.y(), obj.z(), obj.x() + obj.Shape->length(), obj.y() + obj.Shape->width(), obj.z() + obj.Shape->height());
            return false;
        }
        else // 7
        {
            return true;
        }
    }
    else if (obj.y() >= max_y)
    {
        if(obj.z() + obj.Shape->height() <= min_z) // 2
        {
            fprintf (stderr, "*** default_renderer::is_object_below: objects do not overlap!\n");
            fprintf (stderr, "    [%i, %i, %i] - [%i, %i, %i]\n", min_x, min_y, min_z, max_x, max_y, max_z);
            fprintf (stderr, "    (%i, %i, %i) - (%i, %i, %i)\n", obj.x(), obj.y(), obj.z(), obj.x() + obj.Shape->length(), obj.y() + obj.Shape->width(), obj.z() + obj.Shape->height());
            return false;
        }
        else if(obj.z() >= max_z) // 4
        {
            return false;
        }
        else // 3
        {
            return false;
        }
    }
    else if (obj.z() >= max_z) // 5
    {
        return false;
    }
    else if (obj.z() + obj.Shape->height() <= min_z) // 1
    {
        return true;
    }
    else // 8
    {
        // this will be allowed for certain cases and needs code to split objects for correct rendering
        // in other cases, it cannot be avoided due to map structure or differences between collision
        // detection and the code utilized here.
        
        // fprintf (stderr, "*** default_renderer::is_object_below: object intersection!\n");
        // fprintf (stderr, "\n    [%i, %i, %i] - [%i, %i, %i]\n", min_x, min_y, min_z, max_x, max_y, max_z);
        // fprintf (stderr, "    (%i, %i, %i) - (%i, %i, %i)\n", obj.x(), obj.y(), obj.z(), obj.x() + obj.Shape->length(), obj.y() + obj.Shape->width(), obj.z() + obj.Shape->height());

        s_int32 oy = 0, oz = 0;

        // figure out area of least overlap
        if (obj.y() + obj.Shape->width() >= min_y && obj.y() + obj.Shape->width() <= max_y)
            oy = obj.y() + obj.Shape->width() - min_y;
        else if (max_y >= obj.y() && max_y <= obj.y() + obj.Shape->width())
            oy = obj.y() - max_y;
        
        if (obj.z() + obj.Shape->height() >= min_z && obj.z() + obj.Shape->height() <= max_z)
            oz = obj.z() + obj.Shape->height() - min_z;
        else if (max_z >= obj.z() && max_z <= obj.z() + obj.Shape->height())
            oz = obj.z() - max_z;
        
        if (oy != 0 /* && abs (oy) != max_y - min_y */ && abs(oy) < abs(oz))
        {
            // fprintf (stderr, "    [%i, %i, %i] - [%i, %i, %i]\n",  min_x, min_y + (oy > 0 ? oy : 0), min_z, max_x, max_y + (oy < 0 ? oy : 0), max_z);
            return is_object_below (obj, min_x, min_y + (oy > 0 ? oy : 0), min_z, max_x, max_y + (oy < 0 ? oy : 0), max_z);
        }
        else
        {
            // fprintf (stderr, "    [%i, %i, %i] - [%i, %i, %i]\n", min_x, min_y, min_z + (oz > 0 ? oz : 0), max_x, max_y, max_z + (oz < 0 ? oz : 0));
            return is_object_below (obj, min_x, min_y, min_z + (oz > 0 ? oz : 0), max_x, max_y, max_z + (oz < 0 ? oz : 0));
        }
    }
}

// debug rendering
void debug_renderer::draw (const s_int16 & x, const s_int16 & y, const render_info & obj, const gfx::drawing_area & da, gfx::surface * target) const
{
    default_renderer::draw (x, y, obj, da, target);
    
    if (DrawBBox)
    {
        cube3 bbox (obj.Shape->length(), obj.Shape->width(), obj.Shape->height());
        bbox.draw (x + obj.x(), y + obj.y() - obj.z(), &da, target);
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
