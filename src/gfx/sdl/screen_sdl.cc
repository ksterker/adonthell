/*
   $Id: screen_sdl.cc,v 1.2 2003/07/27 14:08:20 gnurou Exp $

   Copyright (C) 2003   Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

/* exported names for libltdl */
#define gfx_screen_set_video_mode sdl_LTX_gfx_screen_set_video_mode
#define gfx_screen_update sdl_LTX_gfx_screen_update
#define gfx_screen_trans_color sdl_LTX_gfx_screen_trans_color
#define gfx_screen_clear sdl_LTX_gfx_screen_clear
#define gfx_screen_get_surface sdl_LTX_gfx_screen_get_surface

#include "gfx/sdl/screen_sdl.h"

gfx::screen_surface_sdl display;
u_int32 trans_color = 0;

extern "C"
{
    bool gfx_screen_set_video_mode(u_int16 nl, u_int16 nh, u_int8 depth);
    void gfx_screen_update();
    u_int32 gfx_screen_trans_color();
    void gfx_screen_clear();
    gfx::surface * gfx_screen_get_surface();
}

bool gfx_screen_set_video_mode(u_int16 nl, u_int16 nh, u_int8 depth)
{
    u_int32 SDL_flags = SDL_HWSURFACE | SDL_DOUBLEBUF;

    if (!display.set_video_mode(nl, nh, depth, SDL_flags)) return false;

    // Setting up the window title
    SDL_WM_SetCaption ("Adonthell", NULL);

    trans_color = SDL_MapRGB(display.get_vis()->format, gfx::screen::TRANS_RED, gfx::screen::TRANS_GREEN, 
                             gfx::screen::TRANS_BLUE);

    return true;
}

void gfx_screen_update()
{
    SDL_Flip (display.get_vis());
}

u_int32 gfx_screen_trans_color()
{
    return trans_color;
}

void gfx_screen_clear()
{
    SDL_FillRect(display.get_vis(), NULL, 0);
}

gfx::surface * gfx_screen_get_surface()
{
    return &display;
}
