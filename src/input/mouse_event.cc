/*
   $Id: mouse_event.cc,v 1.3 2003/11/22 09:37:13 ksterker Exp $

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
 * @file   mouse_event.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the mouse_event class.
 * 
 * 
 */



#include "mouse_event.h"

namespace input
{
    std::string mouse_event::Button_symbol[mouse_event::NBR_BUTTONS] =
        {
            "no button",
            "left button",
            "middle button",
            "right_button",
            "wheel up",
            "wheel down"
        };

    mouse_event::mouse_event (event_type t, button_type b, u_int16 xpos, u_int16 ypos)
        : event (MOUSE_EVENT)
    {
        Type = t;
        Button = b;
        X = xpos;
        Y = ypos;
    }

    const std::string & mouse_event::button_symbol() const
    {
        return Button_symbol[button()];
    }
}
