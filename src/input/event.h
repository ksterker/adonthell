/*
   $Id: event.h,v 1.2 2003/07/22 17:40:39 ksterker Exp $

   Copyright (C) 2002   Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/**
 * @file   event.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the input_event base class.
 * 
 * 
 */


#ifndef INPUT_EVENT_H
#define INPUT_EVENT_H

#include <string>

#include "base/types.h"

#include "python/callback_support.h"

namespace input
{

    /**
     * Base class for input events classes.
     * 
     */
    class event
    {
    public:

        /**
         * Type of the input event.
         * 
         */
        typedef enum
            {
                KEYBOARD_EVENT = 0,
                MOUSE_EVENT,
                JOYSTICK_EVENT,
                CONTROL_EVENT, 
                NBR_INPUT_TYPES
            } input_type;
    
        /** 
         * Constructor
         * 
         * @param t input event type.
         */
        event (input_type t);

        /*
         * Virtual destructor - since this class is abstract
         * and it doesn't make sense to use it alone 
         */
        virtual ~event() = 0;

        /** 
         * Returns the type of this event.
         * 
         * 
         * @return type of this event.
         */
        input_type type () 
        {
            return Type; 
        }

    private:
        input_type Type;
    
    }; 
}

#ifndef SWIG
PYTHON_AS_CALLBACK_ARGUMENT(input::event)
#endif // SWIG
#endif
