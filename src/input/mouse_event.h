/*
   $Id: mouse_event.h,v 1.1 2003/07/18 15:16:09 gnurou Exp $

   Copyright (C) 2002   Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

/**
 * @file   mouse_event.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the mouse_event class.
 * 
 * 
 */


#ifndef MOUSE_EVENT_H
#define MOUSE_EVENT_H

#include "input/event.h"

#include "python/callback_support.h"

namespace input
{

    /**
     * Represents a mouse event, that is a motion event,
     * or a mouse button press or release.
     * 
     */
    class mouse_event : public event
    {
    public:

        /**
         * Types of mouse events.
         * 
         */
        typedef enum
            {
                BUTTON_PUSHED, BUTTON_RELEASED, MOUSE_MOTION
            }event_type;

        /**
         * Mouse buttons enumeration.
         * 
         */
        typedef enum
            {
                NO_BUTTON = 0, LEFT_BUTTON, MIDDLE_BUTTON, RIGHT_BUTTON, 
                WHEEL_UP, WHEEL_DOWN, NBR_BUTTONS
            }button_type;

        /** 
         * Constructor.
         * 
         * @param t type of mouse event.
         * @param b type of button pushed (NO_BUTTON if the event type is 
         *          MOUSE_MOTION)
         * @param xpos x position of the cursor when the event occured.
         * @param ypos y position of the cursor when the event occured.
         * 
         */
        mouse_event (event_type t, button_type b, u_int16 xpos, u_int16 ypos);

        /** 
         * Returns the type of this event.
         * 
         * 
         * @return type of this mouse_event.
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
         * Returns the x position of the cursor in this event.
         * 
         * 
         * @return x position of the cursor.
         */
        u_int16 x() const
        {
            return X;
        }

        /** 
         * Returns the y position of the cursor in this event.
         * 
         * 
         * @return y position of the cursor.
         */
        u_int16 y() const
        {
            return Y;
        }

        /** 
         * Returns a reference to a string containing the symbol
         * of the button concerned by this event.
         * 
         * 
         * @return symbol of the button concerned by this event.
         */
        const std::string & button_symbol() const;

    private:
        static std::string Button_symbol[NBR_BUTTONS];
        event_type Type;
        u_int16 X, Y;
        button_type Button;
    };
}

PYTHON_AS_CALLBACK_ARGUMENT(input::mouse_event)

#endif
