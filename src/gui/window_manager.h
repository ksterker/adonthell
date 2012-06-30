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
#include "layout.h"
#include "ui_event_manager.h"

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
    
    /**
     * Destructor.
     */
    virtual ~manager_child()
    {
    }

    s_int32 x()
    {
        return Dx + Pos.x();
    }

    s_int32 y()
    {
        return Dy + Pos.y();
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

// This class should hold x and y relative to that of the mapobject (possibly negative)
class maprel_window
{
public:
    maprel_window (s_int16 rel_x, s_int16 rel_y, gui::layout *l, const std::string &entity_name)
    : Window(l), Entity_id(entity_name), Relative_x(rel_x), Relative_y(rel_y)
    {
    }

    virtual ~maprel_window()
    {
    }

    bool operator== (const gui::maprel_window & mw)
    {
        return Window == mw.Window && Entity_id == mw.Entity_id;
    }

    gui::layout *Window;
    std::string Entity_id;
    s_int16 Relative_x;
    s_int16 Relative_y;
};

class mapview_container
{
public:
    mapview_container(s_int16 x, s_int16 y, world::mapview *map) : X(x), Y(y), Map(map)
    {
    }

    virtual ~mapview_container()
    {
    }

    bool operator== (const gui::mapview_container & mc)
    {
        return Map == mc.Map;
    }

    s_int16 X;
    s_int16 Y;
    world::mapview *Map;
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
     * The coordinates are absolute.
     * @param x x coordinate.
     * @param y y coordinate.
     * @param window the window to newly open. Ownership passes to the manager.
     * @param f whether fading in is required.
     */
    static void add(const u_int16 & x, const u_int16 & y, gui::layout *window, const gui::fadetype & f = NONE);

    /**
     * Add a window at the topmost position of the relative windows stack
     * (The relative windows are always under absolutely-positioned windows).
     * The window's position will be relative to given entity on the map
     * and the window will move if the entity/view moves.
     * The window will only be displayed, if the entity is visible in the map view.
     * Anonymous entities can't have windows bound to them
     * @param x x coordinate relative to map object's x.
     * @param y y coordinate relative to map object's y.
     * @param window the window to newly open. Ownership passes to the manager.
     * @param entity_id the unique name of the entity the window will be bound to.
     */
    static void add(const s_int16 & x, const s_int16 & y, gui::layout *window, const std::string &entity_id);

    /**
     * Add a window containing a map view at the topmost position of the map view stack.
     * Map views are displayed beneath all other windows.
     * @param x x coordinate.
     * @param y y coordinate.
     * @param map the map view to display.
     */
    static void add(const s_int16 & x, const s_int16 & y, world::mapview *map);

    /**
     * Remove the given window from the window stack.
     * @param window the window to close.
     * @param f whether fading in is required.
     */
    static void remove(gui::layout *window, const gui::fadetype & f = NONE);
    
    /**
     * Remove the given object-relative window from the window stack.
     * @param window the window to close.
     * @param entity_id the unique name of the entity to which the window is bound.
     */
    static void remove(gui::layout *window, const std::string &entity_id);

    /**
     * Remove the given mapview from the mapview stack.
     * @param map the mapview to close.
     */
    static void remove(world::mapview *map);

private:
    /// forbid instantiation
    window_manager ();
    
    static bool fade(gui::manager_child & c);
    
    /// the list of open absolutely-positioned windows
    static std::list<manager_child> Windows;

    /// the list of open object-relative windows
    static std::list<maprel_window> MaprelWindows;

    /// the list of open mapviews
    static std::list<mapview_container> Mapviews;

    /// ui_event manager instance to decouple event triggering from event execution
    static ui_event_manager EventManager;
};

} // namespace gui

#endif
