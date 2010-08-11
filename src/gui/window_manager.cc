/*
 $Id: window_manager.cc,v 1.2 2009/05/04 19:40:39 ksterker Exp $
 
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
 * @file gui/window_manager.cc
 *
 * @author Kai Sterker
 * @brief Handles window order and input.
 */

#include "gfx/screen.h"
#include "input/manager.h"
#include "gui/window_manager.h"
using gui::window_manager;

std::list<gui::widget*> window_manager::Windows;

// render to screen
void window_manager::update()
{
    for (std::list<gui::widget*>::reverse_iterator i = Windows.rbegin(); i != Windows.rend(); i++)
    {
        (*i)->draw (20, 10, NULL, gfx::screen::get_surface());// FIXME: don't hardcode window pos
    }
}

// open window
void window_manager::add (gui::widget *window)
{
    // add window at first position of the input queue
    input::listener *il = new input::listener();
    input::manager::add (il);
    input::manager::give_focus (il);
    
    window->set_listener (il);    
    Windows.push_back (window);
}

// close window
void window_manager::remove (gui::widget *window)
{
    input::listener *il = window->get_listener();
    if (il) input::manager::remove (il);
    
    Windows.remove (window);
}
