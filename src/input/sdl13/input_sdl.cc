/*
   $Id: input_sdl.cc,v 1.1 2009/02/23 20:22:18 ksterker Exp $

   Copyright (C) 2003   Alexandre Courbot <alexandrecourbot@linuxgames.com>
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

#ifdef USE_LIBTOOL
/* exported names for libltdl */
#define input_init _sdl_LTX_input_init
#define input_cleanup _sdl_LTX_input_cleanup
#endif

#include <iostream>
#include "manager_sdl.h"

extern "C"
{
    bool input_init();
    void input_cleanup();
}

bool input_init()
{
    if (SDL_InitSubSystem (SDL_INIT_JOYSTICK) < 0)
    {
        std::cerr << "Couldn't init joysticks: " << SDL_GetError () << std::endl;
        return false;
    }
    int nbr_joy = SDL_NumJoysticks();
    for (int i = 0; i < nbr_joy; i++)
        input::joysticks.push_back(SDL_JoystickOpen(i));
    SDL_EnableUNICODE(1);
    return true;
}

void input_cleanup()
{
    for (std::vector<SDL_Joystick *>::iterator i = input::joysticks.begin(); 
         i != input::joysticks.end(); i++)
        SDL_JoystickClose(*i);
    SDL_QuitSubSystem (SDL_INIT_JOYSTICK);
    SDL_EnableUNICODE(0);
}
