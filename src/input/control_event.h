/*
   $Id: control_event.h,v 1.2 2003/07/24 12:57:58 gnurou Exp $

   Copyright (C) 2002   Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

/**
 * @file   control_event.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the control_event class.
 * 
 * 
 */


#ifndef CONTROL_EVENT_H
#define CONTROL_EVENT_H

#include <string>

#include "input/event.h"
#include "input/keyboard_event.h"
#include "input/mouse_event.h"
#include "input/joystick_event.h"

#include "python/callback_support.h"

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
                NO_BUTTON,
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

#ifndef SWIG
PYTHON_AS_CALLBACK_ARGUMENT(input::control_event)
#endif // SWIG
#endif
