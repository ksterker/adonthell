/*
   $Id: joystick_event.cc,v 1.3 2003/11/22 09:37:13 ksterker Exp $

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
 * @file   joystick_event.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the joystick_event class.
 * 
 * 
 */


#include "joystick_event.h"

namespace input
{
    std::string joystick_event::Button_symbol[joystick_event::NBR_BUTTONS] =
        {
            "no button",
            "button 0",
            "button 1",
            "button 2",
            "button 3",
            "button 4",
            "button 5",
            "button 6",
            "button 7",
            "button 8",
            "button 9",
            "axis 0 fore",
            "axis 0 back",
            "axis 1 fore",
            "axis 1 back",
            "axis 2 fore",
            "axis 2 back",
            "axis 3 fore",
            "axis 3 back",
            "axis 4 fore",
            "axis 4 back",
            "axis 5 fore",
            "axis 5 back",
            "axis 6 fore",
            "axis 6 back",
            "axis 7 fore",
            "axis 7 back",
            "axis 8 fore",
            "axis 8 back",
            "axis 9 fore",
            "axis 9 back"
        };

    joystick_event::joystick_event(u_int8 joynbr, event_type t, button_type b)
        : event (event::JOYSTICK_EVENT)
    {
        Joynbr = joynbr;
        Type = t;
        Button = b;
    }

    const std::string & joystick_event::button_symbol() const
    {
        return Button_symbol[button()];
    }
}
