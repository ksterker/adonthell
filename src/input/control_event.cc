/*
   $Id: control_event.cc,v 1.3 2003/11/22 09:37:13 ksterker Exp $

   Copyright (C) 2002   Alexandre Courbot <alexandrecourbot@linuxgames.com>
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
 * @file   control_event.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the control_event class.
 * 
 * 
 */

#include "control_event.h"

namespace input
{
    std::string control_event::Button_symbol[control_event::NBR_BUTTONS] =
        {
            "no button"
            "up",
            "down",
            "right",
            "left",
            "A button",
            "B button",
            "C button",
            "D button"
        };

    control_event::control_event(event_type t, button_type b) : event(CONTROL_EVENT)
    {
        Type = t;
        Button = b;
    }

    const std::string & control_event::button_symbol() const
    {
        return Button_symbol[button()];
    }

    control_event::button_type control_event::Keyboard_map[keyboard_event::NBR_KEYS] = 
        { control_event::NO_BUTTON };
    control_event::button_type control_event::Mouse_map[mouse_event::NBR_BUTTONS] = 
        { control_event::NO_BUTTON };
    control_event::button_type control_event::Joystick_map[joystick_event::NBR_BUTTONS * 
                                                           joystick_event::max_nbr_joysticks] = 
        { control_event::NO_BUTTON };
}
