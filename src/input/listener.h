/*
   $Id: listener.h,v 1.4 2003/11/22 09:37:13 ksterker Exp $

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
 * @file   listener.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the listener class.
 * 
 * 
 */


#ifndef INPUT_LISTENER_H
#define INPUT_LISTENER_H

#include "input/keyboard_event.h"
#include "input/mouse_event.h"
#include "input/joystick_event.h"
#include "input/control_event.h"
#include "base/callback.h"

#include "python/callback_support.h"

namespace input
{
    /**
     * Class that has the ability to get notified of input events by the manager
     * and launch callback functions when such events are triggered.
     * 
     */
    class listener
    {    
    public:
        /**
         * Constructor.
         * 
         */
        listener ();

        /**
         * Destructor.
         * 
         */
        ~listener ();    

        /** 
         * Returns whether this listener will react to a certain kind of events.
         * 
         * @param t event type to test
         * 
         * @return \e true if this listener listens to events of type t, \e false otherwise.
         */
        bool is_listening_to_keyboard () 
        {
            return (Keyboard_callback != NULL);
        }

        bool is_listening_to_mouse () 
        {
            return (Mouse_callback != NULL);
        }

        bool is_listening_to_joystick () 
        {
            return (Joystick_callback != NULL);
        }

        bool is_listening_to_control () 
        {
            return (Control_callback != NULL);
        }

        /** 
         * Connect a callback function to a type of event for this listener.
         * The listener will automatically listen to this kind of events.
         * 
         * @param t type of event to listen to
         * @param f callback function to call when an event of type \e t is raised
         */
        void connect_keyboard_function(base::functor_1ret<keyboard_event *, bool> * f);

        void connect_mouse_function(base::functor_1ret<mouse_event *, bool> * f);

        void connect_joystick_function(base::functor_1ret<joystick_event *, bool> * f);

        void connect_control_function(base::functor_1ret<control_event *, bool> * f);

        /**
         * Stops listening to events of type \e t.
         *
         * @param t type of events to stop listening to.
         */
        void disconnect_keyboard_function();

        void disconnect_mouse_function();

        void disconnect_joystick_function();

        void disconnect_control_function();


        /**
         * Raise the \e ev event with the listener. This will call the
         * appropriate callback function with the appropriate parameters
         * if this listener listens to events of the type of \e ev.
         *
         * @warning You should never have to call this function by yourself. If
         *          you want to manually raise an event, use
         *          input_manager::raise_event() instead.
         *
         * @param ev event to raise
         *
         * @return value returned by the callback function
         */
        int raise_event (event * ev);

    private:
        base::functor_1ret<keyboard_event *, bool> * Keyboard_callback;
        base::functor_1ret<mouse_event *, bool> * Mouse_callback;
        base::functor_1ret<joystick_event *, bool> * Joystick_callback;
        base::functor_1ret<control_event *, bool> * Control_callback;
    };
}

#ifndef SWIG
PYTHON_AS_CALLBACK_ARGUMENT(input::listener)
#endif // SWIG
#endif
