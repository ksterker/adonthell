/*
   $Id: control_event.h,v 1.7 2006/07/09 15:57:34 ksterker Exp $

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
 * @file   input/control_event.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the control_event class.
 * 
 * 
 */


#ifndef CONTROL_EVENT_H
#define CONTROL_EVENT_H

#include <adonthell/base/configuration.h>
#include "keyboard_event.h"
#include "mouse_event.h"
#include "joystick_event.h"

namespace input
{
    /**
     * Logical input events. These events represent a virtual joypad,
     * to which others input events can be mapped. Such events are designed
     * to be caught by the player controlling method, so the player can
     * map the keys/buttons he wants to these events.
     * 
     */
    class control_event : public event
    {
    public:

        /**
         * Types of control events.
         * 
         */
        typedef enum
            {
                BUTTON_PUSHED, BUTTON_RELEASED
            } event_type;

    
        /**
         * Control buttons enumeration.
         * 
         */
        typedef enum
            {
                NO_BUTTON = 0,
                UP_BUTTON,
                DOWN_BUTTON,
                RIGHT_BUTTON,
                LEFT_BUTTON,
                A_BUTTON,
                B_BUTTON,
                C_BUTTON,
                D_BUTTON,
                NBR_BUTTONS
            } button_type;

        /** 
         * Constructor.
         * 
         * @param t type of this control_event
         * @param b button concerned by this event.
         * 
         */
        control_event(event_type t, button_type b);

        /**
         * @name Member access
         */
        //@{
        /** 
         * Returns the type of this event.
         * 
         * 
         * @return type of this control_event.
         */    
        event_type type () const
        {
            return Type; 
        }

        /** 
         * Returns the button concerned by this event.
         * 
         * 
         * @return button concerned by this event.
         */
        button_type button () const
        {
            return Button; 
        }

        /** 
         * Returns a reference to a string containing the symbol
         * of the button concerned by this event.
         * 
         * 
         * @return symbol of the button concerned by this event.
         */
        const std::string & button_symbol () const;
        //@}
        
        /**
         * @name Key/Name - mapping
         */
        //@{
        /**
         * Get the name of a button when giving the button code.
         * @return name of a given button.
         */
        static const std::string & name_for_button (button_type btn)
        {
            return Button_symbol[btn];
        }
        
        /**
         * Get the button code when giving a certain key name.
         * @return key code or NO_BUTTON if no match found.
         */
        static const button_type button_for_name (const std::string & name)
        {
            for (int i = 0; i < NBR_BUTTONS; i++)
                if (Button_symbol[i] == name)
                    return (button_type) i;
            
            return NO_BUTTON;
        }        
        //@}
        
        /**
         * @name Mapping of controls
         */
        //@{
        /**
         * Return the default keyboard key for the given
         * control action.
         * @param btn the control action.
         * @return the default key assigned to the action.
         */
        static const keyboard_event::key_type default_key (button_type btn);

        /**
         * Read control mappings from configuration
         * @param cfg configuration file.
         */
        static void map_controls (base::configuration & cfg);
        
        /** 
         * Map a keyboard key to a control button.
         * 
         * @param kek keyboard key to be mapped
         * @param cek control button to map to.
         */
        static void map_keyboard_key(keyboard_event::key_type kek, control_event::button_type cek)
        {
            Keyboard_map[kek] = cek;
        }

        /** 
         * Map a mouse button to a control button.
         * 
         * @param kek mouse button to be mapped
         * @param cek control button to map to.
         */
        static void map_mouse_button(mouse_event::button_type kek, control_event::button_type cek)
        {
            Mouse_map[kek] = cek;
        }

        /** 
         * Map a joystick button to a control button.
         * 
         * @param joynbr the number of the joystick to map.
         * @param kek joystick button to be mapped.
         * @param cek control button to map to.
         */
        static void map_joystick_button(int joynbr, joystick_event::button_type kek, control_event::button_type cek)
        {
            Joystick_map[kek + joynbr * joystick_event::NBR_BUTTONS] = cek;
        }

        /** 
         * Returns the mapping of a keyboard key.
         * 
         * @param t keyboard key to check mapping
         * 
         * @return control button assigned to this key.
         */
        static button_type keyboard_mapping(keyboard_event::key_type t)
        {
            return Keyboard_map[t];
        }

        /** 
         * Returns the mapping of a mouse button.
         * 
         * @param t mouse button to check mapping
         * 
         * @return control button assigned to this button.
         */
        static button_type mouse_mapping(mouse_event::button_type t)
        {
            return Mouse_map[t];
        }

        /** 
         * Returns the mapping of a joystick button.
         * 
         * @param t joystick button to check mapping
         * 
         * @return control button assigned to this button.
         */
        static button_type joystick_mapping(joystick_event::button_type t)
        {
            return Joystick_map[t];
        }
        //@}
        
#ifndef SWIG
        GET_TYPE_NAME(input::control_event)
#endif // SWIG

    private:
        event_type Type;
        button_type Button;
        static std::string Button_symbol[NBR_BUTTONS];

        static button_type Keyboard_map[keyboard_event::NBR_KEYS];
        static button_type Mouse_map[mouse_event::NBR_BUTTONS];
        static button_type Joystick_map[joystick_event::NBR_BUTTONS * 
                                        joystick_event::max_nbr_joysticks];
    };
}

#endif
