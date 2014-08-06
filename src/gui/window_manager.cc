/*
 Copyright (C) 2009/2010 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file gui/window_manager.cc
 *
 * @author Kai Sterker
 * @brief Handles window order and input.
 */

#include <adonthell/world/area_manager.h>
#include <adonthell/world/vector3.h>
#include "window_manager.h"

using gui::window_manager;

/// the list of open, absolute windows
std::list<gui::window*> window_manager::Windows;
/// the list of open, world-relative windows
std::list<gui::window*> window_manager::WorldRelativeWindows;
/// storage for pending events.
std::list<events::listener*> window_manager::PendingEvents;

// render to screen
void window_manager::update()
{
    // trigger the event listeners
    fire_events();

    /* draw all mapviews
    for (std::list<gui::mapview_container>::reverse_iterator i = Mapviews.rbegin(); i != Mapviews.rend(); i++)
    {
        i->Map->draw(i->X, i->Y);

        if (FirstWorldRelative != Windows.end()) // skip retrieving mapobjects if not necessary
        {
            // get all objects visible in the mapview
            std::list<world::chunk_info*> objects_in_mapview = world::area_manager::get_map()->objects_in_view(
                i->Map->get_view_start_x(),
                i->Map->get_view_start_y(),
                i->Map->get_z(),
                i->Map->length(),
                i->Map->height()
            );

            // draw all windows that should be visible in the mapview
            for (std::list<gui::maprel_window>::reverse_iterator w = MaprelWindows.rbegin(); w != MaprelWindows.rend(); w++)
            {
                // Only draw window, if the object it's bound to is visible
                for (std::list<world::chunk_info*>::iterator ci = objects_in_mapview.begin(); ci != objects_in_mapview.end(); ci++)
                {
                    world::entity *en = (*ci)->get_entity();
                    if(en->has_name() && *(en->id()) == w->Entity_id)
                    {
                        world::vector3<s_int32> objpos = (*ci)->center_min();
                        // The coords: where mapview starts being displayed +
                        // + (mapobject's coords - coords of mapview's starting map point)
                        s_int16 coord_x = i->X + i->Map->get_view_offset_x () + objpos.x() + w->Relative_x - i->Map->get_view_start_x();
                        s_int16 coord_y = i->Y + i->Map->get_view_offset_y () + objpos.y() + w->Relative_y - i->Map->get_view_start_y();
                        w->Window->draw (coord_x, coord_y, NULL, gfx::screen::get_surface ());
                        break;
                    }
                }
            }
        }
    } */

    // draw all windows
    std::list<gui::window*>::reverse_iterator i = Windows.rbegin();
    while (i != Windows.rend())
    {
        if ((*i)->fading() && (*i)->fade ())
        {
            delete *i;
            i = std::list<gui::window*>::reverse_iterator(Windows.erase (--(i.base())));

            if (i != Windows.rend())
            {
                (*i)->receive_focus();
            }

            continue;
        }

        switch ((*i)->type())
        {
            case WORLD_VIEW:
            {
                (*i)->draw();
                
                // draw world-relative windows, if any
                std::list<gui::window*>::reverse_iterator j = WorldRelativeWindows.rbegin();
                while (j != WorldRelativeWindows.rend())
                {
                    if ((*j)->fading() && (*j)->fade ())
                    {
                        delete *j;

                        j = std::list<gui::window*>::reverse_iterator(WorldRelativeWindows.erase (--(j.base())));
                        continue;
                    }
                    
                    (*j)->draw((*i)->display_x(), (*i)->display_y());
                    j++;
                }
                break;
            }
            case WORLD_RELATIVE:
            {
                // skip over world-relative windows in normal drawing
                break;
            }
            default:
            {
                (*i)->draw();
                break;
            }
        }

        i++;
    }
}

// open widget as a new window
void window_manager::add (const u_int16 & x, const u_int16 & y, gfx::drawable & content, const gui::fade_type & f, const gui::window_type & w)
{
    gfx::drawing_area area (x, y, content.length(), content.height());
    add(new gui::window (content, area, w), f);
}

// open a new window
void window_manager::add(window *window, const gui::fade_type & f)
{
    if (window->type() == WORLD_RELATIVE)
    {
        // relative windows are kept in their own list and may not get focus
        window->fade_in(f);
        
        WorldRelativeWindows.push_front(window);
    }
    else
    {
        std::list<gui::window*>::iterator i = Windows.begin();
        for (; i != Windows.end(); i++)
        {
            if (window->type() > (*i)->type())
            {
                // insert in front of all windows of lower or equal type
                // note that window-list is sorted in reverse drawing order
                break;
            }
        }
        
        if (i != Windows.begin() && i != Windows.end())
        {
            // remove focus, if required
            (*i)->remove_focus();
        }
        
        window->fade_in(f);
        
        // acquire focus, if required
        window->receive_focus();

        Windows.insert(i, window);
    }
}

// close window
void window_manager::remove (gfx::drawable & content, const gui::fade_type & f)
{    
    gfx::drawing_area pos (0, 0, content.length(), content.height());
    gui::window wnd(content, pos, window_type::INTERFACE);

    for (std::list<gui::window*>::iterator i = Windows.begin(); i != Windows.end(); i++)
    {
        if (*(*i) == wnd)
        {
            remove(*i, f);
            return;
        }
    }
}

void window_manager::remove (gui::window *window, const gui::fade_type & f)
{
    window->remove_focus();
    window->fade_out(f);
}

void window_manager::fire_events()
{
    // call event handlers
    for (std::list<events::listener*>::iterator li = PendingEvents.begin(); li != PendingEvents.end(); li++)
    {
        (*li)->raise_event ((*li)->get_event());
    }

    // clear list of pending events
    PendingEvents.clear();
}
