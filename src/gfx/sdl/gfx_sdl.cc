/*
   $Id: gfx_sdl.cc,v 1.1 2003/07/18 15:16:09 gnurou Exp $

   Copyright (C) 2003   Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

/* exported names for libltdl */
#define gfx_init sdl_LTX_gfx_init
#define gfx_cleanup sdl_LTX_gfx_cleanup

#define gfx_create_surface sdl_LTX_gfx_create_surface

#include <iostream>
#include "surface_sdl.h"
#include "SDL.h"

extern "C"
{
    bool gfx_init();
    void gfx_cleanup();

    gfx::surface * gfx_create_surface();
}

bool gfx_init()
{
    if (SDL_InitSubSystem (SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Couldn't init display: " << SDL_GetError () << std::endl;
        return false;
    }
    return true;
}

void gfx_cleanup()
{
    SDL_QuitSubSystem (SDL_INIT_VIDEO);
}

gfx::surface * gfx_create_surface()
{
    return new gfx::surface_sdl();
}
