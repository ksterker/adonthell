/*
   $Id: input.h,v 1.1 2003/07/18 15:16:09 gnurou Exp $

   Copyright (C) 2003  Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

/**
 * @file   input.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  The input module main include file.
 * 
 * 
 */

#ifndef INPUT_H_
#define INPUT_H_

#include <string>
#include "input/event.h"
#include "input/control_event.h"
#include "input/keyboard_event.h"
#include "input/joystick_event.h"
#include "input/mouse_event.h"
#include "input/listener.h"
#include "input/manager.h"

/**
 * This module gives an interface to handle user input, from
 * keyboard (keyboard_event), mouse (mouse_event), joystick (joystick_event) 
 * or from a virtual, configurable device (control_event).
 * 
 */
namespace input
{
    /** 
     * Initialisation method. This function must be called
     * before any use of this class.
     * 
     * @param backend_name The name of the backend to use.
     *
     * @return \e true if the initialization was successful,
     *         \e false otherwise.
     *
     */
    bool init(const std::string & backend_name);
    
    /**
     * Cleanup method. Call this function when you don't need
     * the input module anymore.
     * 
     */
    void cleanup();

}

#endif
