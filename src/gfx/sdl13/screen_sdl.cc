/*
   $Id: screen_sdl.cc,v 1.1 2009/02/23 20:22:18 ksterker Exp $

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
#define gfx_screen_set_video_mode _sdl_LTX_gfx_screen_set_video_mode
#define gfx_screen_update _sdl_LTX_gfx_screen_update
#define gfx_screen_trans_color _sdl_LTX_gfx_screen_trans_color
#define gfx_screen_clear _sdl_LTX_gfx_screen_clear
#define gfx_screen_get_surface _sdl_LTX_gfx_screen_get_surface
#define gfx_screen_info _sdl_LTX_gfx_screen_info
#endif

#include <sstream> 
#include "screen_sdl.h"

u_int32 trans_color = 0;

extern "C"
{
    bool gfx_screen_set_video_mode(u_int16 nl, u_int16 nh, u_int8 depth);
    void gfx_screen_update();
    u_int32 gfx_screen_trans_color();
    void gfx_screen_clear();
    gfx::surface *gfx_screen_get_surface();
    std::string gfx_screen_info();
}

bool gfx_screen_set_video_mode(u_int16 nl, u_int16 nh, u_int8 depth)
{
    u_int32 SDL_flags = SDL_HWSURFACE | SDL_DOUBLEBUF;
	if (gfx::screen::is_fullscreen()) SDL_flags |= SDL_FULLSCREEN;

    if (!display->set_video_mode(nl, nh, depth, SDL_flags)) return false;

    // Setting up the window title
    SDL_WM_SetCaption ("Adonthell", NULL);

    // Setting up transparency color
    trans_color = SDL_MapRGB (
        display->get_vis()->format, gfx::screen::TRANS_RED, gfx::screen::TRANS_GREEN, gfx::screen::TRANS_BLUE);

    return true;
}

void gfx_screen_update()
{
    SDL_Flip (display->get_vis());
}

u_int32 gfx_screen_trans_color()
{
    return trans_color;
}

void gfx_screen_clear()
{
    SDL_FillRect(display->get_vis(), NULL, 0);
}

gfx::surface * gfx_screen_get_surface()
{
    return display;
}

std::string gfx_screen_info()
{
    const SDL_VideoInfo * vi = SDL_GetVideoInfo ();
    std::ostringstream temp; 

    const int driver_name_length = 500;
    char drv_name[driver_name_length];

    temp << "Video information: \n"
         << "Video driver used:                   " << SDL_VideoDriverName(drv_name, driver_name_length) << std::endl
         << "Internal game depth:                 " << ((int) vi->vfmt->BitsPerPixel) << std::endl
         << "Can create hardware surfaces:        " << (vi->hw_available ? "Yes" : "No") << std::endl
         << "Window manager available:            " << (vi->wm_available ? "Yes" : "No") << std::endl
         << "Hardware blits accelerated:          " << (vi->blit_hw ? "Yes" : "No") << std::endl
         << "Colorkey hardware blits accelerated: " << (vi->blit_hw_CC ? "Yes" : "No") << std::endl
         << "Alpha hardware blits accelerated:    " << (vi->blit_hw_A ? "Yes" : "No") << std::endl
         << "Software blits accelerated:          " << (vi->blit_sw ? "Yes" : "No") << std::endl
         << "Colorkey software blits accelerated: " << (vi->blit_sw_CC ? "Yes" : "No") << std::endl
         << "Alpha software blits accelerated:    " << (vi->blit_sw_A ? "Yes" : "No") << std::endl
         << "Color fill blits accelerated:        " << (vi->blit_fill ? "Yes" : "No") << std::endl
         << "Total video memory available:        " << vi->video_mem << " Kb" << std::endl 
         << "Fullscreen:                          " << (gfx::screen::is_fullscreen() ? "Yes" : "No") << std::endl
         // << "Alpha value:                         " << ((int) vi->vfmt->alpha) << std::endl
         << std::ends;

    return temp.str ();
}
