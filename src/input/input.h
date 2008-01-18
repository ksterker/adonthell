/*
   $Id: input.h,v 1.4 2008/01/18 21:26:13 ksterker Exp $

   Copyright (C) 2003/2006 Alexandre Courbot <alexandrecourbot@linuxgames.com>
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
 * @file   input/input.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  The input module main include file.
 * 
 * 
 */

#ifndef INPUT_H_
#define INPUT_H_

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
    bool init (const std::string & backend_name);
    
    /**
     * Load input settings from configuration file. This will
     * initialize the mapping of the virtual controls to the
     * actual input devices.
     * 
     * @param cfg the game configuration
     *
     */
    void setup (base::configuration & cfg);
    
    /**
     * Cleanup method. Call this function when you don't need
     * the input module anymore.
     * 
     */
    void cleanup();
}

#endif
