/*
   $Id: listener.h,v 1.2 2003/07/24 12:57:58 gnurou Exp $

   Copyright (C) 2002   Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
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
        void connect_keyboard_function(base::functor_1ret<keyboard_event *, int> * f);

        void connect_mouse_function(base::functor_1ret<mouse_event *, int> * f);

        void connect_joystick_function(base::functor_1ret<joystick_event *, int> * f);

        void connect_control_function(base::functor_1ret<control_event *, int> * f);

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
        base::functor_1ret<keyboard_event *, int> * Keyboard_callback;
        base::functor_1ret<mouse_event *, int> * Mouse_callback;
        base::functor_1ret<joystick_event *, int> * Joystick_callback;
        base::functor_1ret<control_event *, int> * Control_callback;
    };
}

#ifndef SWIG
PYTHON_AS_CALLBACK_ARGUMENT(input::listener)
#endif // SWIG
#endif
