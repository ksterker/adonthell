/*
   $Id: listener.cc,v 1.4 2003/11/22 09:37:13 ksterker Exp $

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
 * @file   listener.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the listener class.
 * 
 * 
 */

#include "listener.h"

namespace input
{
    listener::listener () : Keyboard_callback(NULL), Mouse_callback(NULL), Joystick_callback(NULL), Control_callback(NULL)
    {
    }

    listener::~listener()
    {
        disconnect_keyboard_function();
        disconnect_mouse_function();
        disconnect_joystick_function();
        disconnect_control_function();
    }


    void listener::connect_keyboard_function(base::functor_1ret<keyboard_event *, bool> * f)
    {
        disconnect_keyboard_function();
        Keyboard_callback = f;
    }

    void listener::connect_mouse_function(base::functor_1ret<mouse_event *, bool> * f)
    {
        disconnect_mouse_function();
        Mouse_callback = f;
    }

    void listener::connect_joystick_function(base::functor_1ret<joystick_event *, bool> * f)
    {
        disconnect_joystick_function();
        Joystick_callback = f;
    }

    void listener::connect_control_function(base::functor_1ret<control_event *, bool> * f)
    {
        disconnect_control_function();
        Control_callback = f;
    }


    void listener::disconnect_keyboard_function()
    {
        if (Keyboard_callback)
        {
            delete Keyboard_callback;
            Keyboard_callback = NULL;
        }
    }

    void listener::disconnect_mouse_function()
    {
        if (Mouse_callback)
        {
            delete Mouse_callback;
            Mouse_callback = NULL;
        }
    }

    void listener::disconnect_joystick_function()
    {
        if (Joystick_callback)
        {
            delete Joystick_callback;
            Joystick_callback = NULL;
        }
    }

    void listener::disconnect_control_function()
    {
        if (Control_callback)
        {
            delete Control_callback;
            Control_callback = NULL;
        }
    }


    int listener::raise_event (event * ev)
    {
        switch(ev->type())
        {
            case event::CONTROL_EVENT:
                if (Control_callback) return (*Control_callback)((control_event *)ev);
            case event::KEYBOARD_EVENT:
                if (Keyboard_callback) return (*Keyboard_callback)((keyboard_event *)ev);
            case event::JOYSTICK_EVENT:
                if (Joystick_callback) return (*Joystick_callback)((joystick_event *)ev);
            case event::MOUSE_EVENT:
                if (Mouse_callback) return (*Mouse_callback)((mouse_event *)ev);
            default:
                break;
        }
        return 0;
    }
}
