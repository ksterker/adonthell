/*
   $Id: input_sdl.cc,v 1.2 2003/07/24 12:57:58 gnurou Exp $

   Copyright (C) 2003   Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

/* exported names for libltdl */
#define input_init sdl_LTX_input_init
#define input_cleanup sdl_LTX_input_cleanup

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
