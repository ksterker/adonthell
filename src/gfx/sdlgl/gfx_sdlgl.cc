/*
   $Id: gfx_sdlgl.cc,v 1.2 2006/09/30 21:05:08 gnurou Exp $

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
#define gfx_init sdlgl_LTX_gfx_init
#define gfx_cleanup sdlgl_LTX_gfx_cleanup
#define gfx_create_surface sdlgl_LTX_gfx_create_surface
#endif

#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include "surface_sdlgl.h"
#include "screen_sdlgl.h"
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

    display = new gfx::screen_surface_sdlgl ();
    return true;
}

void gfx_cleanup()
{
    delete display;
    //BUG  this function may cause problems because it will call destructors before python does.
    SDL_QuitSubSystem (SDL_INIT_VIDEO);
}

gfx::surface * gfx_create_surface()
{
    return new gfx::surface_sdlgl();
}
