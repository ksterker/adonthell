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

#include "gui/layout.h"

namespace gui
{

/// different fading styles for toplevel windows
enum fadetype {NONE, PLAIN, FADE, LEFT, RIGHT, BOTTOM, TOP};    

#ifndef SWIG
/**
 *
 */
class manager_child : public layoutchild
{
public:
    manager_child (gui::layout *l, gfx::drawing_area da, const fadetype & f, const bool & s) 
    : layoutchild (l, da)
    {
        Fading = f;
        Showing = s;
        Dx = 0;
        Dy = 0;
    }
    
    s_int32 x()
    {
        return Dx + Pos.x();
    }

    s_int32 y()
    {
        return Dx + Pos.x();
    }

    bool operator== (const gui::manager_child & c)
    {
        return Child == c.Child;
    }
    
    s_int32 Dx; 
    s_int32 Dy;
    fadetype Fading;
    bool Showing;
};
#endif // SWIG

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
     * @param x x coordinate.
     * @param y y coordinate.
     * @param window the window to newly open.
     * @param f whether fading in is required.
     */
    static void add(const u_int16 & x, const u_int16 & y, gui::layout *window, const gui::fadetype & f = NONE);
    
    /**
     * Remove the given window from the window stack.
     * @param window the window to close.
     * @param f whether fading in is required.
     */
    static void remove(gui::layout *window, const gui::fadetype & f = NONE);
    
private:
    /// forbid instantiation
    window_manager ();
    
    static void fade(gui::manager_child & c);
    
    /// the list of open windows
    static std::list<manager_child> Windows;
};

} // namespace gui

#endif
