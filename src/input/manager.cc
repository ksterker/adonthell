/*
   $Id: manager.cc,v 1.2 2003/07/24 12:57:58 gnurou Exp $

   Copyright (C) 2002   Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/**
 * @file   manager.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com
 * 
 * @brief  Defines the manager class members that are
 *         system-independant.
 * 
 * 
 */


#include "manager.h"
#include <algorithm>

namespace input
{
    std::list <listener *> manager::listeners;
    void (*manager::update_p)() = NULL;


    void manager::raise_event(event & ev)
    {
        for (std::list<listener *>::iterator it = listeners.begin(); 
             it != listeners.end(); it++)
        {
            bool res = (*it)->raise_event(&ev);
            if (res) break;
        }

        // Check if the event is mapped to a virtual controller event
        switch (ev.type())
        {
            case event::KEYBOARD_EVENT:
            {
                keyboard_event & ke = (keyboard_event &)ev;
                if (control_event::keyboard_mapping(ke.key()) != control_event::NO_BUTTON)
                {
                    control_event ce(ke.type() == keyboard_event::KEY_PUSHED ?
                                     control_event::BUTTON_PUSHED : 
                                     control_event::BUTTON_RELEASED, 
                                     control_event::keyboard_mapping(ke.key()));
                    raise_event(ce);
                }
                break;
            }

            case event::MOUSE_EVENT:
            {
                mouse_event & me = (mouse_event &)ev;
                if (control_event::mouse_mapping(me.button()) != control_event::NO_BUTTON)
                {
                    control_event ce(me.type() == mouse_event::BUTTON_PUSHED ?
                                     control_event::BUTTON_PUSHED :
                                     control_event::BUTTON_RELEASED, 
                                     control_event::mouse_mapping(me.button()));
                    raise_event(ce);
                }
                break;
            }

            case event::JOYSTICK_EVENT:
            {
                joystick_event & je = (joystick_event &)ev;
                if (control_event::joystick_mapping(je.button()) != control_event::NO_BUTTON)
                {
                    control_event ce(je.type() == joystick_event::BUTTON_PUSHED ? 
                                     control_event::BUTTON_PUSHED : 
                                     control_event::BUTTON_RELEASED,
                                     control_event::joystick_mapping(je.button()));
                    raise_event(ce);
                }
                break;
            }
        
            default:
                break;
        }
    }

    bool manager::give_focus(listener * l)
    {
        std::list<listener *>::iterator it = std::find(listeners.begin(), listeners.end(), l);
        if (it != listeners.end())
        {
            listeners.erase(it);
            listeners.push_front(l);
            return true;
        }
        return false;
    }
}
