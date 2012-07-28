/*
   $Id: manager_sdl.h,v 1.1 2009/02/23 20:22:18 ksterker Exp $

   Copyright (C) 2002/2003   Alexandre Courbot <alexandrecourbot@linuxgames.com>
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
 * @file   manager_sdl.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  SDL version of the input manager.
 * 
 * 
 */

#ifndef INPUT_MANAGER_SDL_H
#define INPUT_MANAGER_SDL_H

#define SDL_NO_COMPAT 1

#include "../input.h"
#include "SDL.h"
#include <vector>

namespace input
{
    extern keyboard_event::key_type sdl_key_trans[/*SDLK_LAST*/SDL_NUM_SCANCODES];
    extern mouse_event::button_type sdl_button_trans[SDL_BUTTON_RIGHT + 3];
    extern joystick_event::button_type sdl_axis_trans[joystick_event::NBR_BUTTONS - 
                                                      joystick_event::AXIS0_FORE];
    extern joystick_event::button_type sdl_joy_button_trans[joystick_event::AXIS0_FORE];
    extern bool joystick_state_table[joystick_event::NBR_BUTTONS * 10];
    extern std::vector<SDL_Joystick *> joysticks;
}

#endif
