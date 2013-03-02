/*
 $Id: renderer.cc,v 1.9 2009/03/21 11:59:47 ksterker Exp $
 
 Copyright (C) 2008/2009/2010 Kai Sterker <kaisterker@linuxgames.com>
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

#include <adonthell/base/timer.h>
#include <adonthell/gfx/screen.h>
#include "renderer.h"

namespace world
{

// draw object and shadow on that object to screen
void renderer_base::draw (const s_int16 & x, const s_int16 & y, const render_info & obj, const gfx::drawing_area & da, gfx::surface * target) const
{
    // render object
    obj.Sprite->draw (x + obj.screen_x(), y + obj.screen_y(), &da, target);
    
    if (obj.Shape->is_solid())
    {
        // render shadows cast onto the object
        for (std::vector<shadow_info>::const_iterator shdw = obj.Shadow->begin(); shdw != obj.Shadow->end(); shdw++)
        {
            // set shadow opacity according to distance above ground
            shdw->Image->set_alpha (192 - (shdw->Distance > 192 ? 32 : shdw->Distance));
            // draw all pieces of the shadow
            for (std::list<gfx::drawing_area>::const_iterator area = shdw->Area.begin(); area != shdw->Area.end(); area++)
            {
                // relocate area to mapview position
                gfx::drawing_area part (x + area->x(), y + area->y() - (obj.z() + obj.Shape->height()), area->length(), area->height());
                // clip with mapview ... just in case
                part.assign_drawing_area (&da);
                // render shadow
                shdw->Image->draw (x + shdw->X, y + shdw->Y - (obj.z() + obj.Shape->height()), &part, target);
            }
        }
    }
}

// default rendering
void default_renderer::render (const s_int16 & x, const s_int16 & y, const std::list <world::chunk_info*> & objectlist, const gfx::drawing_area & da, gfx::surface * target) const
{
    std::list <render_info> render_queue;

    // populate render queue
    for (std::list<world::chunk_info*>::const_iterator i = objectlist.begin(); i != objectlist.end(); i++)
    {
        const placeable *object = (*i)->get_object();
        
        for (placeable::iterator obj = object->begin(); obj != object->end(); obj++)
        {
            render_queue.push_back (render_info ((*obj)->current_shape(), (*obj)->get_sprite(), (*i)->center_min(), (*i)->get_shadow(*obj)));
        }
    }
    
    render (x, y, render_queue, da, target);
}

#define CAN_SKIP 0
#define CAN_DRAW 1
#define CAN_DROP 2

// default rendering
void default_renderer::render (const s_int16 & x, const s_int16 & y, std::list <world::render_info> & render_queue, const gfx::drawing_area & da, gfx::surface * target) const
{        
    // paint while object remain in the queue
    while (!render_queue.empty())
    {
        size_t size = render_queue.size();

        // check each object if it can be drawn
        for (reverse_iterator it = render_queue.rbegin(); it != render_queue.rend(); /* nothing */)
        {
            const_iterator begin = render_queue.begin();
            const_iterator end = render_queue.end();

            // an object can be drawn if it cannot possibly collide with another object in the queue
            switch (can_draw_object (*it, begin, end))
            {
                case CAN_DRAW:
                {
                    // draw and remove from queue
                    draw (x, y, *it, da, target);
                    /* fallthrough */
                }
                case CAN_DROP:
                {
                    render_queue.erase (--it.base());
                    break;
                }
                default:
                {
                    it++;
                    break;
                }
            }
        }
   
        // should not happen, but does lead to a deadlock
        if (size == render_queue.size())
        {
            LOG(ERROR) << "*** warning: deadlock during rendering detected!";
            for (iterator it = render_queue.begin(); it != render_queue.end(); it++)
                VLOG(3) << "  - (" << it->x() << ", " << it->y() << "," << it->z()
                        << ") - (" << it->x() + it->Shape->length() << ", " <<  it->y() + it->Shape->width() << ", " << it->z() + it->Shape->height() << ")";

            //visualize_deadlock (render_queue);
            //exit(1);
            
            draw (x, y, render_queue.front(), da, target);
            render_queue.pop_front();
        }
    }
}

// check if object can be rendered
u_int32 default_renderer::can_draw_object (render_info & obj, const_iterator & begin, const_iterator & end) const
{
    // assume there are no objects in the way, so we can draw
    u_int32 result = CAN_DRAW;

    // the parts of obj that are visible on screen
    std::list<gfx::drawing_area> visible_area;

    // initially, the whole object is visible
    gfx::drawing_area va (obj.screen_x(), obj.screen_y(), obj.Sprite->length(), obj.Sprite->height());
    visible_area.push_back (va);

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
        if (is_object_below (*it, obj))
        {
            // at that point, there's at least one object in the way
            result = CAN_SKIP;
            continue;
        }
        else if (it->Sprite->is_opaque())
        {
            gfx::drawing_area obj_surface (it->screen_x(), it->screen_y(), it->Sprite->length(), it->Sprite->height());
            obj_surface.subtract_from (visible_area);

            if (visible_area.empty())
            {
                // we're completely hidden behind other objects
                return CAN_DROP;
            }
        }
    }
    
    return result;
}

// default rendering
void hw_renderer::render (const s_int16 & x, const s_int16 & y, std::list <world::render_info> & render_queue, const gfx::drawing_area & da, gfx::surface * target) const
{
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
            LOG(ERROR) << "*** warning: deadlock during rendering detected!";
            for (iterator it = render_queue.begin(); it != render_queue.end(); it++)
                VLOG(3) << " - (" << it->x() << ", " << it->y() << "," << it->z()
                        << ") - (" << it->x() + it->Shape->length() << ", " << it->y() + it->Shape->width() << ", " << it->z() + it->Shape->height() << ")";

            //visualize_deadlock (render_queue);
            //exit(1);

            draw (x, y, render_queue.front(), da, target);
            render_queue.pop_front();
        }
    }
}

// check if object can be rendered
u_int32 hw_renderer::can_draw_object (render_info & obj, const_iterator & begin, const_iterator & end) const
{
    // compare given object with all objects remaining in the draw queue
    for (const_iterator it = begin; it != end; it++)
    {
        // ... but not with itself
        if (&(*it) == &obj) continue;

        // if objects don't overlap, we're still good
        if (obj.min_x() >= it->max_x() ||
            obj.min_yz() >= it->max_yz() ||
            it->min_x() >= obj.max_x() ||
            it->min_yz() >= obj.max_yz())
            continue;

        // objects do overlap, so we need to figure out position of objects
        // relative to each other
        if (is_object_below (*it, obj))
            return CAN_SKIP;
    }

    // there are no objects in the way, so we can draw
    return CAN_DRAW;
}

#define YT 1
#define YB 2
#define ZT 4
#define ZB 8

// check object order (is obj2 below obj?)
bool default_renderer::is_object_below (const render_info & obj, const render_info & obj2) const
{
    s_int32 min_x = obj2.x();
    s_int32 min_y = obj2.y();
    s_int32 min_z = obj2.z();
    s_int32 max_x = min_x + obj2.Shape->length();
    s_int32 max_y = min_y + obj2.Shape->width();
    s_int32 max_z = min_z + obj2.Shape->height();
    
    // only comparing two objects by their y and z coordinate, we
    // want to figure out if an object is in front of the other or
    // not. For that purpose, we divide space into 9 segments, with
    // object two as the center segement #8. We then check in which 
    // segment the first object is located.
    //
    // segments    coordinates  is below?
    // 2 | 3 | 4   y            x | F | F
    // --+---+--   ^            --+---+-- max_y
    // 1 | 8 | 5   |            T | o | F
    // --+---+--   |            --+---+-- min_y
    // 0 | 7 | 6   +-----> z    T | T | x
    //                            \   \-- max_z
    //                             \----- min_z
    //
    // If the first object falls completely into segment 0, 1 or 7
    // it is below object two. In segment 3, 4 or 5 it is above.
    // In segments 2 and 6, the two objects do not overlap, but
    // since that is tested in advance, this case should not occur.
    // Finally, if object one falls into segment 8, the two objects
    // intersect and there is no easy answer which object is in front
    // and which behind.
    
    if (obj.y() + obj.Shape->width() <= min_y)
    {
        if (obj.z() + obj.Shape->height() <= min_z) // segment 0
        {
            return true;
        }
        else if (obj.z() >= max_z) // segment 6
        {
            fprintf (stderr, "*** default_renderer::is_object_below: objects do not overlap!\n");
            fprintf (stderr, "    [%i, %i, %i] - [%i, %i, %i]\n", min_x, min_y, min_z, max_x, max_y, max_z);
            fprintf (stderr, "    (%i, %i, %i) - (%i, %i, %i)\n", obj.x(), obj.y(), obj.z(), obj.x() + obj.Shape->length(), obj.y() + obj.Shape->width(), obj.z() + obj.Shape->height());
            return false;
        }
        else // segment 7
        {
            return true;
        }
    }
    else if (obj.y() >= max_y)
    {
        if(obj.z() + obj.Shape->height() <= min_z) // segment 2
        {
            fprintf (stderr, "*** default_renderer::is_object_below: objects do not overlap!\n");
            fprintf (stderr, "    [%i, %i, %i] - [%i, %i, %i]\n", min_x, min_y, min_z, max_x, max_y, max_z);
            fprintf (stderr, "    (%i, %i, %i) - (%i, %i, %i)\n", obj.x(), obj.y(), obj.z(), obj.x() + obj.Shape->length(), obj.y() + obj.Shape->width(), obj.z() + obj.Shape->height());
            return false;
        }
        else if(obj.z() >= max_z) // segment 4
        {
            return false;
        }
        else // 3
        {
            return false;
        }
    }
    else if (obj.z() >= max_z) // segment 5
    {
        return false;
    }
    else if (obj.z() + obj.Shape->height() <= min_z) // segment 1
    {
        return true;
    }
    else // segment 8
    {
        // TODO: here we'd need to cut non-solid flat objects that intersect with
        // solid non-flat objects
        
        // this will be allowed for certain cases and needs code to split objects for correct rendering
        // in other cases, it cannot be avoided due to map structure or differences between collision
        // detection and the code utilized here.

        if (obj.Shape->is_flat() == obj2.Shape->is_flat())
        {
            if (obj.Shape->is_flat())
            {
                // both objects are floor tiles 
                if (obj.z() + obj.Shape->height() != max_z)
                {
                    // draw the one first that's below
                    return obj.z() + obj.Shape->height() < max_z;
                }
                else
                {
                    // equal position --> draw the thinner first
                    return obj.Shape->height() > obj2.Shape->height();
                }
            }
            else
            {
                // both objects are walls
                if (obj.y() + obj.Shape->width() != max_y)
                {
                    // draw the one first that's behind
                    return obj.y() + obj.Shape->width() < max_y;
                }
                else
                {
                    // equal position --> draw the thinner first
                    return obj.Shape->width() > obj2.Shape->width();
                }
            }
        }

        //  ZT      YB       YT       ZB       0
        //          _______
        //  | |     ---|---     |        | |     |
        //  |-|---     |     ___|___  ---|-|  ---|---
        //  | |        |     ---'---     | |     |

        int res = 0;
        if (obj.y() >= min_y && obj.y() <= max_y && obj.y() + obj.Shape->width() >= max_y)
        {
            res += YT; // obj in front of obj2
        }
        else if (obj.y() + obj.Shape->width() <= max_y && obj.y() + obj.Shape->width() >= min_y && obj.y() <= min_y)
        {
            res += YB; // obj behind obj2
        }
        
        if (obj.z() >= min_z && obj.z() <= max_z && obj.z() + obj.Shape->height() >= max_z)
        {
            res += ZT; // obj on top obj2
        }
        else if (obj.z() + obj.Shape->height() <= max_z && obj.z() + obj.Shape->height() >= min_z && obj.z() <= min_z)
        {
            res += ZB; // obj below obj2
        }
        
        switch (res)
        {
            case 0:
            default:
            {                
                // fprintf (stderr, "*** default_renderer::is_object_below: total intersection of objects!\n"); // E
                // fprintf (stderr, "    [%i, %i, %i] - [%i, %i, %i]\n", min_x, min_y, min_z, max_x, max_y, max_z);
                // fprintf (stderr, "    (%i, %i, %i) - (%i, %i, %i)\n", obj.x(), obj.y(), obj.z(), obj.x() + obj.Shape->length(), obj.y() + obj.Shape->width(), obj.z() + obj.Shape->height());

                // FIXME: this is a bad guess
                return obj2.Shape->is_flat();
            }                
            case YT:
            {
                return false;
            }
            case YB:
            {
                return true;
            }
            case ZT:
            {
                return false;
            }
            case ZB:
            {
                return true;
            }
            case YT|ZT:
            {
                return false;
            }
            case YT|ZB:
            {
                return true;
            }
            case YB|ZT:
            {
                return false;
            }
            case YB|ZB:
            {
                return true;
            }
        }
    }
}

// debug rendering
void debug_renderer::draw (const s_int16 & x, const s_int16 & y, const render_info & obj, const gfx::drawing_area & da, gfx::surface * target) const
{
    default_renderer::draw (x, y, obj, da, target);
    
    if (DrawBBox)
    {
        // draw detailed collision information
        for (std::vector<world::cube3*>::const_iterator i = obj.Shape->begin(); i != obj.Shape->end(); i++)
        {
            (*i)->draw (x + obj.Pos.x(), y + obj.Pos.y() - obj.Pos.z(), &da, target);
        }
        
        // draw complete bbox around placeable
        // cube3 bbox (obj.Shape->length(), obj.Shape->width(), obj.Shape->height());
        // bbox.draw (x + obj.x(), y + obj.y() - obj.z(), &da, target);
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

void default_renderer::visualize_deadlock (std::list <world::render_info> & render_queue) const
{        
    std::vector <world::render_info> queue (render_queue.begin(), render_queue.end());
    
    std::ofstream graph ("deadlock.dot");
    graph << "digraph deadlock {" << std::endl;

    // print all nodes
    for (size_t i = 0; i < render_queue.size(); i++)
    {
        const render_info & ri = queue[i];
        graph << "n" << i << " [label=\"" << ri.Pos << "\\n" << ri.Shape->get_min() << " - " << ri.Shape->get_max() << "\"];" << std::endl;  
    }

    graph << std::endl;

    // check each object if it can be drawn
    for (size_t j = 0; j < render_queue.size(); j++)
    {
        for (size_t i = 0; i < render_queue.size(); i++)
        {
            // ... but not with itself
            if (i == j) continue;
            
            const render_info & ri = queue[i];
            const render_info & rj = queue[j];
            
            // if objects don't overlap, we're still good
            if (rj.min_x()  >= ri.max_x()  ||
                rj.min_yz() >= ri.max_yz() ||
                ri.min_x()  >= rj.max_x()  ||
                ri.min_yz() >= rj.max_yz())
                continue;
            
            // objects do overlap, so we need to figure out position of objects 
            // relative to each other
            if (is_object_below (ri, rj))
            {
                // draw and remove from queue
                graph << "  n" << j << " -> n" << i << ";" << std::endl;
                break;
            }
        }
    }
    
    graph << "}" << std::endl;
    graph.close();
}

}
