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

/// the rate of movement for disappearing layouts in pixels
#define FADERATE 4

using gui::window_manager;

/// the list of open windows
std::list<gui::manager_child> window_manager::Windows;

std::list<gui::maprel_window> window_manager::MaprelWindows;

std::list<gui::mapview_container> window_manager::Mapviews;

std::list<events::listener*> window_manager::PendingEvents;

// render to screen
void window_manager::update()
{
    // trigger the event listeners
    fire_events();

    // draw all mapviews
    for (std::list<gui::mapview_container>::reverse_iterator i = Mapviews.rbegin(); i != Mapviews.rend(); i++)
    {
        i->Map->draw(i->X, i->Y);

        if (!MaprelWindows.empty()) // skip retrieving mapobjects if not necessary
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
    }

    // draw normal windows
    for (std::list<gui::manager_child>::reverse_iterator i = Windows.rbegin(); i != Windows.rend(); i++)
    {
        if (i->Fading && fade (*i))
        {
            delete i->Child;
            Windows.remove (*i);
            continue;
        }

        i->Child->draw (i->x(), i->y(), NULL, gfx::screen::get_surface());
    }
}

// open window
void window_manager::add (const u_int16 & x, const u_int16 & y, gui::layout *window, const gui::fadetype & f)
{
    // add window at first position of the input queue
    input::listener *il = new input::listener();
    input::manager::add (il);
    input::manager::give_focus (il);
    
    // assign listener to window
    window->set_listener (il);

    // give focus to new window
    window->focus();

    gfx::drawing_area pos (x, y, window->length(), window->height());
    Windows.push_back (manager_child (window, pos, f, true));
}

void window_manager::add (const s_int16 & x, const s_int16 & y, gui::layout *window, const std::string &entity_id)
{
    MaprelWindows.push_back(maprel_window(x, y, window, entity_id));
}

void window_manager::add (const s_int16 & x, const s_int16 & y, world::mapview *map)
{
    Mapviews.push_back(mapview_container(x, y, map));
}

// close window
void window_manager::remove (gui::layout *window, const gui::fadetype & f)
{
    input::listener *il = window->get_listener();
    if (il) input::manager::remove (il);
    
    for (std::list<gui::manager_child>::iterator i = Windows.begin(); i != Windows.end(); i++)
    {
        if (i->Child == window)
        {
            if (f) i->Fading = f;
            else i->Fading = PLAIN;
            i->Showing = false;
            return;
        }
    }
}

void window_manager::remove (gui::layout *window, const std::string &entity_id)
{
    for (std::list<gui::maprel_window>::iterator i = MaprelWindows.begin(); i != MaprelWindows.end(); i++)
    {
        // we do not fade
        if (i->Window == window && i->Entity_id == entity_id)
        {
            delete i->Window;
            MaprelWindows.remove (*i);
            return;
        }
    }
}

void window_manager::remove(world::mapview *map)
{
    for (std::list<gui::mapview_container>::iterator i = Mapviews.begin(); i != Mapviews.end(); i++)
        if (i->Map == map)
            Mapviews.remove(*i);
    //TODO What about making the mapview invisible?
}

// fade layout in or out
bool window_manager::fade (gui::manager_child & c)
{
    // initial position
    if (c.Showing && c.Dx == 0 && c.Dy == 0)
    {
        switch (c.Fading)
        {
            case LEFT:
                c.Dx = -c.Pos.length() - c.Pos.x();
                break;
            case RIGHT:
                c.Dx = gfx::screen::length() - c.Pos.x();
                break;
            case TOP:
                c.Dy = -c.Pos.height() - c.Pos.y();
                break;
            case BOTTOM:
                c.Dy = gfx::screen::height() - c.Pos.y();
                break;
            default:
                break;
        }
    }
    
    switch (c.Fading)
    {
        case LEFT:
            if (c.Showing)
            {
                c.Dx += FADERATE;
                if (c.Dx > 0)
                {
                    c.Dx = 0;
                    c.Fading = NONE;
                }
            }
            else
            {
                c.Dx -= FADERATE;
                if (c.Dx + c.Pos.x() + c.Pos.length() < 0)
                {
                    return true;
                }
            }
            break;
        case RIGHT:
            if (c.Showing)
            {
                c.Dx -= FADERATE;
                if (c.Dx < 0)
                {
                    c.Dx = 0;
                    c.Fading = NONE;
                }
            }
            else
            {
                c.Dx+= FADERATE;
                if (c.Dx + c.Pos.x() > gfx::screen::length())
                {
                    return true;
                }
            }
            break;
        case TOP:
            if (c.Showing)
            {
                c.Dy += FADERATE;
                if (c.Dy > 0)
                {
                    c.Fading = NONE;
                    c.Dy = 0;
                }
            }
            else
            {
                c.Dy -= FADERATE;
                if (c.Dy + c.Pos.y() + c.Pos.height() < 0)
                {
                    return true;
                }
            }
            break;
        case BOTTOM:
            if (c.Showing)
            {
                c.Dy -= FADERATE;
                if (c.Dy < 0)
                {
                    c.Fading = NONE;
                    c.Dy = 0;
                }
            }
            else
            {
                c.Dy += FADERATE;
                if (c.Dy + c.Pos.y() > gfx::screen::height())
                {
                    return true;
                }
            }
            break;
        case PLAIN:
            c.Fading = NONE;
            return !c.Showing;

        default:
            break;
    }
    return false;
}

void window_manager::fire_events()
{
    // event handlers might fire new events, so use a copy
    std::list<events::listener*> copy (PendingEvents.begin(), PendingEvents.end());

    // clear list of pending events
    PendingEvents.clear();

    // call event handlers
    for (std::list<events::listener*>::iterator li = copy.begin(); li != copy.end(); li++)
    {
        (*li)->raise_event ((*li)->get_event());
    }
}
