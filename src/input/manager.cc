/*
   $Id: manager.cc,v 1.5 2006/07/09 15:57:34 ksterker Exp $

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
 * @file   input/manager.cc
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
    void (*manager::textinput_p)(bool) = NULL;


    void manager::raise_event(event & ev)
    {
        for (std::list<listener *>::iterator it = listeners.begin(); 
             it != listeners.end(); it++)
        {
            bool res = (*it)->raise_event(&ev);
            if (res) break;
        }

        // Check if the event is mapped to a virtual controller event
        switch (ev.event_type())
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
