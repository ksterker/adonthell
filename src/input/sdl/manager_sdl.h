/*
   $Id: manager_sdl.h,v 1.1 2003/07/18 15:16:09 gnurou Exp $

   Copyright (C) 2002/2003   Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/**
 * @file   manager_sdl.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  SDL version of the input manager.
 * 
 * 
 */

#ifndef INPUT_MANAGER_SDL_H
#define INPUT_MANAGER_SDL_H

#include "input/input.h"
#include "SDL.h"
#include <vector>

extern input::keyboard_event::key_type sdl_key_trans[SDLK_LAST];
extern input::mouse_event::button_type sdl_button_trans[SDL_BUTTON_RIGHT + 3];
extern input::joystick_event::button_type sdl_axis_trans[input::joystick_event::NBR_BUTTONS - 
                                                         input::joystick_event::AXIS0_FORE];
extern input::joystick_event::button_type sdl_joy_button_trans[input::joystick_event::AXIS0_FORE];
extern bool joystick_state_table[input::joystick_event::NBR_BUTTONS * 10];
extern std::vector<SDL_Joystick *> joysticks;


#endif
