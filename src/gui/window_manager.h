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
 * @file gui/window_manager.h
 *
 * @author Kai Sterker
 * @brief Handles window order and input.
 */

#ifndef GUI_WINDOW_MANAGER_H
#define GUI_WINDOW_MANAGER_H

#include <adonthell/world/mapview.h>

#include "window.h"
#include "layout.h"

namespace gui
{

/**
 * The window manager is used to display GUI elements on the screen.
 * It handles z-order and input focus of multiple windows. 
 */
class window_manager
{
public:
    /**
     * Draw everything on screen.
     */
    static void update();
    
    /**
     * Add a window at the topmost position of the window stack for the window type.
     * The coordinates are absolute.
     * @param x x coordinate.
     * @param y y coordinate.
     * @param content the data to display in a newly opened window.
     * @param f whether fading in is required.
     * @param w type of the window.
     */
    static void add(const u_int16 & x, const u_int16 & y, gfx::drawable & content, const gui::fade_type & f = NONE, const gui::window_type & w = DIALOG);

    /**
     * Add a window to the front of the window stack for the window type.
     * @param window the window to add.
     * @param f whether fading in is required.
     */
    static void add(gui::window *window, const gui::fade_type & f = NONE);

    /**
     * Remove the given window from the window stack.
     * @param content data displayed in the window to close.
     * @param f whether fading in is required.
     */
    static void remove(gfx::drawable & content, const gui::fade_type & f = NONE);
    
    /**
     * Remove the given window from the window stack.
     * @param window the window to close.
     */
    static void remove(gui::window *window, const gui::fade_type & f = NONE);

    /**
     * Queue an ui event to execute before the next gui update. Required to
     * decouple ui event handling from ui event firing.
     * @param li the listener to execute.
     */
    static void queue_event (events::listener* li)
    {
        PendingEvents.push_back (li);
    }

private:
    /// forbid instantiation
    window_manager ();

    /// forbid copy construction
    window_manager (const window_manager &w);

    /// forbid destruction
    ~window_manager ();

    /// forbid copying
    window_manager operator= (const window_manager &w);
    
    /// perform event handling
    static void fire_events ();

    /// the list of open, absolute windows
    static std::list<window*> Windows;

    /// the list of open, world-relative windows
    static std::list<window*> WorldRelativeWindows;
    
    /// storage for pending events.
    static std::list<events::listener*> PendingEvents;
};

} // namespace gui

#endif
