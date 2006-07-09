/*
   $Id: control_event.cc,v 1.4 2006/07/09 15:57:34 ksterker Exp $

   Copyright (C) 2002/2006 Alexandre Courbot <alexandrecourbot@linuxgames.com>
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
 * @file   input/control_event.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the control_event class.
 * 
 * 
 */

#include "control_event.h"

namespace input
{
    // make sure these values contain no spaces, as this will break the XML config file!
    std::string control_event::Button_symbol[control_event::NBR_BUTTONS] =
        {
            "no button",
            "move_north",
            "move_south",
            "move_east",
            "move_west",
            "a_button",
            "b_button",
            "c_button",
            "d_button"
        };

    // ctor
    control_event::control_event(event_type t, button_type b) : event(CONTROL_EVENT)
    {
        Type = t;
        Button = b;
    }

    // return name of button concerned by event
    const std::string & control_event::button_symbol() const
    {
        return Button_symbol[button()];
    }

    // map controls from configuration
    void control_event::map_controls (base::configuration & cfg)
    {
        std::string key, value;
        
        // TODO: change default controls once we know what each key does
        const keyboard_event::key_type defaultMapping[NBR_BUTTONS] = {
            keyboard_event::UNKNOWN_KEY,
            keyboard_event::UP_KEY,
            keyboard_event::DOWN_KEY,
            keyboard_event::RIGHT_KEY,
            keyboard_event::LEFT_KEY,
            keyboard_event::SPACE_KEY,
            keyboard_event::ESCAPE_KEY,
            keyboard_event::LCTRL_KEY,
            keyboard_event::LALT_KEY            
        };
        
        // read setting for each of our controls
        for (u_int32 btn = control_event::UP_BUTTON; btn < control_event::NBR_BUTTONS; btn++)
        {
            // get name of control (= config option)
            key = control_event::name_for_button ((control_event::button_type) btn);
            
            // get mapped control, using given default if not set yet
            value = cfg.get_string ("Input", key, keyboard_event::name_for_key (defaultMapping[btn]));
            
            // setup internal mapping tables
            keyboard_event::key_type kt = keyboard_event::key_for_name (value);
            if (kt != keyboard_event::UNKNOWN_KEY)
            {
                map_keyboard_key (kt, (control_event::button_type) btn);
                continue;
            }
            
            mouse_event::button_type mb = mouse_event::button_for_name (value);
            if (mb != mouse_event::NO_BUTTON)
            {
                map_mouse_button (mb, (control_event::button_type) btn);
                continue;
            }
            
            joystick_event::button_type jb = joystick_event::button_for_name (value);
            if (jb != joystick_event::NO_BUTTON)
            {
                // TODO: get proper joystick number
                map_joystick_button (0, jb, (control_event::button_type) btn);
                continue;
            }
            
            // invalid control value --> use default, but don't update configuration
            fprintf (stderr, "*** control_event::map_controls: unknown control '%s' for action '%s'!\n", value.c_str(), key.c_str());
            kt = defaultMapping[btn];
            fprintf (stderr, "    substituting default key '%s' instead!\n", keyboard_event::name_for_key (kt).c_str ());
            map_keyboard_key (kt, (control_event::button_type) btn);
        }
    }

    // keyboard mappings
    control_event::button_type control_event::Keyboard_map[keyboard_event::NBR_KEYS] = 
        { control_event::NO_BUTTON };
    // mouse mappings
    control_event::button_type control_event::Mouse_map[mouse_event::NBR_BUTTONS] = 
        { control_event::NO_BUTTON };
    // joystick mappings
    control_event::button_type control_event::Joystick_map[joystick_event::NBR_BUTTONS * 
                                                           joystick_event::max_nbr_joysticks] = 
        { control_event::NO_BUTTON };
}
