/*
   $Id: joystick_event.h,v 1.4 2004/04/29 08:07:49 ksterker Exp $

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

#ifndef JOYSTICK_EVENT_H
#define JOYSTICK_EVENT_H

/**
 * @file   joystick_event.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the joystick_event class.
 * 
 * 
 */

#include "input/event.h"

namespace input
{

    /**
     * Represents a joystick event, that is a button and
     * a status (BUTTON_PUSHED or BUTTON_RELEASED)
     * 
     */
    class joystick_event : public event
    {
    public:

        /**
         * Types of joystick events.
         * 
         */
        typedef enum
            {
                BUTTON_PUSHED, BUTTON_RELEASED
            }event_type;

        /**
         * Joystick buttons enumeration.
         * 
         */
        typedef enum
            {
                NO_BUTTON = 0, BUTTON_0, BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4,
                BUTTON_5, BUTTON_6, BUTTON_7, BUTTON_8, BUTTON_9,
                AXIS0_FORE, AXIS0_BACK, AXIS1_FORE, AXIS1_BACK, AXIS2_FORE, 
                AXIS2_BACK, AXIS3_FORE, AXIS3_BACK, AXIS4_FORE, AXIS4_BACK,
                AXIS5_FORE, AXIS5_BACK, AXIS6_FORE, AXIS6_BACK, AXIS7_FORE,
                AXIS7_BACK, AXIS8_FORE, AXIS8_BACK, AXIS9_FORE, AXIS9_BACK,
                NBR_BUTTONS
            }button_type;

        /** 
         * Constructor.
         * 
         * @param joynbr number of the joystick
         * @param t type of joystick event
         * @param b button concerned by this event
         * 
         */
        joystick_event(u_int8 joynbr, event_type t, button_type b);

        /** 
         * Returns the number of the joystick concerned by this event.
         * 
         * 
         * @return joystick number.
         */
        u_int8 joystick_nbr() const
        {
            return Joynbr;
        }

        /** 
         * Returns the type of this event.
         * 
         * 
         * @return type of this joystick_event.
         */    
        event_type type() const
        {
            return Type;
        }

        /** 
         * Returns the button concerned by this event.
         * 
         * 
         * @return button concerned by this event.
         */
        button_type button() const
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
        const std::string & button_symbol() const;

        /**
         * Maximum number of joysticks we support.
         * 
         */
        static const int max_nbr_joysticks = 5;

#ifndef SWIG
        GET_TYPE_NAME(input::joystick_event)
#endif // SWIG

    private:
        static std::string Button_symbol[NBR_BUTTONS];
        u_int8 Joynbr;
        event_type Type;
        button_type Button;
    };
}

#endif
