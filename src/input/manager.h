/*
   $Id: manager.h,v 1.1 2003/07/18 15:16:09 gnurou Exp $

   Copyright (C) 2002/2003   Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/**
 * @file   manager.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the manager static class.
 * 
 * 
 */


#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <list> 

#include "input/listener.h"
#include "input/keyboard_event.h"
#include "input/mouse_event.h"
#include "input/joystick_event.h"
#include "input/control_event.h"

namespace input
{

    /**
     * This class is responsible for handling all input events and
     * raise callback methods accordingly. It mainly handles a list
     * of input_listeners which hears to certain types of events. When
     * an input event occurs, this class checks for input_listeners
     * that listens to this kind of event and call their callback
     * function with the event as parameter.
     * 
     */
    class manager
    {
    public:
        /**
         * Update the manager status. This method should be 
         * called once per game cycle. It will reads all the events
         * in the event queues and call the connected callback methods 
         * accordingly.
         * 
         */
        static void update() { update_p(); }

        /**
         * Raise an arbitrary event. This will simulate the event and
         * will behave as if it really occured.
         * 
         */
        static void raise_event(event & ev);

        /**
         * Add an input_listener to the list of handled listeners.
         * 
         */
        static void add(listener * il)
        {
            listeners.push_back(il);
        }
    
        /**
         * Remove an input_listener from the list of handled listeners.
         * 
         */
        static void remove(listener * il)
        {
            listeners.remove(il);
        }

        /**
         * Give the focus to the listener given in argument, if applicable.
         *
         * Giving the focus means this listener is queried first when an event
         * is triggered.
         *
         * @param l the listener to give focus to.
         * @return \e true if the listener was found, \e false otherwise.
         */
        static bool give_focus(listener * l);

    private:
        static std::list <listener *> listeners;
        static void (*update_p)();

        friend bool input::init(const std::string &);
    }; 
}

#endif
