/*
 Copyright (C) 2008 Rian Shelley
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
 * @file   gui/widget.cpp
 * @author Rian Shelley
 * @brief  Declares the widget class.
 */

#include "input/manager.h"
#include "gui/widget.h"

namespace gui
{
    void widget::draw (const s_int16 & x, const s_int16 & y, const gfx::drawing_area * da, gfx::surface * target) const
	{
		Look->draw (x, y, da, target);
	}
    
    // set input listener
    void widget::set_listener (input::listener *lstnr) 
    {
        if (Listener) 
        {
            input::manager::remove (Listener);
            delete Listener;
        }
        
        Listener = lstnr;
        
        if (Listener) 
        {
            Listener->connect_keyboard_function (::base::make_functor_ret(*this, &widget::on_keyboard_event));
        }
    }
    
    // get input listener
    input::listener * widget::get_listener () const 
    {
        return Listener;
    }
    
    // handle keyboard events
    bool widget::on_keyboard_event (input::keyboard_event * evt) 
    {
        switch (evt->type())
        {
            case input::keyboard_event::KEY_PUSHED:
            {
                return keydown (*evt);
            }
            case input::keyboard_event::KEY_RELEASED:
            {
                return keyup (*evt);
            }
            case input::keyboard_event::TEXT_INPUT:
            {
                return input (*evt);
            }
        }
        
        return false;
    } 
};
