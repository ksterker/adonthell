/*
 Copyright (C) 2014 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file gui/window.cc
 *
 * @author Kai Sterker
 * @brief Base class for windows displayable by window manager.
 */


#include "window.h"
#include "layout.h"

/// the rate of movement for disappearing layouts in pixels
#define FADERATE 4

using gui::window;

void window::receive_focus()
{
    if (can_focus())
    {
        gui::layout *child = dynamic_cast<gui::layout*>(Content);
        if (child != NULL)
        {
            // add window at first position of the input queue
            input::listener *il = new input::listener();
            input::manager::add (il);
            input::manager::give_focus (il);

            // assign listener to window
            child->set_listener (il);

            // give focus to new window
            child->focus();
        }
    }
}

void window::remove_focus()
{
    if (can_focus())
    {
        gui::layout *child = dynamic_cast<gui::layout*>(Content);
        if (child != NULL)
        {
            // remove input listeners
            input::listener *il = child->get_listener();
            if (il) input::manager::remove (il);

            // take focus from the window
            child->unfocus();
        }
    }
}

// fade layout in or out
bool window::fade ()
{
    // initial position
    if (Showing && Dx == 0 && Dy == 0)
    {
        switch (FadeType)
        {
            case LEFT:
                Dx = -length() - x();
                break;
            case RIGHT:
                Dx = gfx::screen::length() - x();
                break;
            case TOP:
                Dy = -height() - y();
                break;
            case BOTTOM:
                Dy = gfx::screen::height() - y();
                break;
            default:
                break;
        }
    }
    
    switch (FadeType)
    {
        case LEFT:
            if (Showing)
            {
                Dx += FADERATE;
                if (Dx > 0)
                {
                    Dx = 0;
                    FadeType = NONE;
                }
            }
            else
            {
                Dx -= FADERATE;
                if (Dx + x() + length() < 0)
                {
                    return true;
                }
            }
            break;
        case RIGHT:
            if (Showing)
            {
                Dx -= FADERATE;
                if (Dx < 0)
                {
                    Dx = 0;
                    FadeType = NONE;
                }
            }
            else
            {
                Dx += FADERATE;
                if (Dx + x() > gfx::screen::length())
                {
                    return true;
                }
            }
            break;
        case TOP:
            if (Showing)
            {
                Dy += FADERATE;
                if (Dy > 0)
                {
                    FadeType = NONE;
                    Dy = 0;
                }
            }
            else
            {
                Dy -= FADERATE;
                if (Dy + y() + height() < 0)
                {
                    return true;
                }
            }
            break;
        case BOTTOM:
            if (Showing)
            {
                Dy -= FADERATE;
                if (Dy < 0)
                {
                    FadeType = NONE;
                    Dy = 0;
                }
            }
            else
            {
                Dy += FADERATE;
                if (Dy + y() > gfx::screen::height())
                {
                    return true;
                }
            }
            break;
        case PLAIN:
            FadeType = NONE;
            return !Showing;
        default:
            break;
    }
    
    return false;
}
