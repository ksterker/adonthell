/*
   $Id: joystick_event.cc,v 1.1 2003/07/18 15:16:09 gnurou Exp $

   Copyright (C) 2002   Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
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

using namespace input;

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
