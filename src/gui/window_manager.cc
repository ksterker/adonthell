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

#include "gfx/screen.h"
#include "input/manager.h"
#include "gui/window_manager.h"

/// the rate of movement for disappearing layouts in pixels
#define FADERATE 4

using gui::window_manager;

std::list<gui::manager_child> window_manager::Windows;

// render to screen
void window_manager::update()
{
    for (std::list<gui::manager_child>::reverse_iterator i = Windows.rbegin(); i != Windows.rend(); i++)
    {
        if (i->Fading) fade (*i);
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

    gfx::drawing_area pos (x, y, window->length(), window->height());
    Windows.push_back (manager_child (window, pos, f, true));
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
            else Windows.erase (i);
            return;
        }
    }
}

// fade layout in or out
void window_manager::fade (gui::manager_child & c)
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
                    Windows.remove (c);
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
                    Windows.remove (c);
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
                    Windows.remove (c);
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
                    Windows.remove (c);
                }
            }
            break;
    }
}
