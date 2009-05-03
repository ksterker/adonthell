/*
 $Id: window_manager.h,v 1.1 2009/05/03 16:26:00 ksterker Exp $
 
 Copyright (C) 2009 Kai Sterker <kaisterker@linuxgames.com>
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

#include <list>
#include "gui/widget.h"

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
     * Add a window at the topmost position of the window stack.
     * @param window the window to newly open.
     */
    static void add(gui::widget *window);
    
    /**
     * Remove the given window from the window stack.
     * @param window the window to close.
     */
    static void remove(gui::widget *window);
    
private:
    /// forbid instantiation
    window_manager ();
    
    /// the list of open windows
    static std::list<gui::widget*> Windows;
};

} // namespace gui

#endif
