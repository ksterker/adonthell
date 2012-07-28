/*
   $Id: event.h,v 1.7 2006/07/09 15:57:34 ksterker Exp $

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
 * @file   input/event.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the input_event base class.
 * 
 * 
 */


#ifndef INPUT_EVENT_H
#define INPUT_EVENT_H

#include <string>

#include <adonthell/base/types.h>
#include <adonthell/python/callback_support.h>

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
        input_type event_type () 
        {
            return EventType; 
        }

#ifndef SWIG
        GET_TYPE_NAME_ABSTRACT(input::event)
#endif // SWIG

    private:
        input_type EventType;
    
    }; 
}

#endif
