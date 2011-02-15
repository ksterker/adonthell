/*
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
#define gfx_screen_get_video_mode _sdl_LTX_gfx_screen_get_video_mode
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
	void gfx_screen_get_video_mode(u_int16 *l, u_int16 *h, u_int8 *depth);
    bool gfx_screen_set_video_mode(u_int16 nl, u_int16 nh, u_int8 depth);
    void gfx_screen_update();
    u_int32 gfx_screen_trans_color();
    void gfx_screen_clear();
    gfx::surface *gfx_screen_get_surface();
    std::string gfx_screen_info();
}

void gfx_screen_get_video_mode(u_int16 *l, u_int16 *h, u_int8 *depth)
{
	SDL_DisplayMode mode;
	SDL_GetDesktopDisplayMode (0, &mode);

	*l = mode.w;
	*h = mode.h;
	*depth = SDL_BYTESPERPIXEL (mode.format);
}

bool gfx_screen_set_video_mode(u_int16 nl, u_int16 nh, u_int8 depth)
{
    u_int32 SDL_flags = SDL_WINDOW_SHOWN;
	if (gfx::screen::is_fullscreen()) SDL_flags |= SDL_WINDOW_FULLSCREEN;

    if (!display->set_video_mode(nl, nh, depth, SDL_flags)) return false;

    // Setting up transparency color
    trans_color = 0xFF00FF;
    return true;
}

void gfx_screen_update()
{
    SDL_RenderPresent(display->get_renderer());
}

u_int32 gfx_screen_trans_color()
{
    return trans_color;
}

void gfx_screen_clear()
{
    SDL_SetRenderDrawColor(display->get_renderer(), 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(display->get_renderer());
}

gfx::surface * gfx_screen_get_surface()
{
    return display;
}

std::string gfx_screen_info()
{
    SDL_version version_info;
    SDL_RendererInfo render_info;
    std::ostringstream temp;

    SDL_GetVersion(&version_info);
    SDL_GetRendererInfo(display->get_renderer(), &render_info);
    temp << "Video information: " << std::endl
         << "Platform:          " << SDL_GetPlatform() << std::endl
         << "Backend:           " << "SDL " <<(int) version_info.major << "." << (int) version_info.minor << "." << (int) version_info.patch << std::endl
         << "Video driver used: " << SDL_GetCurrentVideoDriver() << std::endl
         << "Renderer used:     " << render_info.name << std::endl
         << "HW Accelerated:    " << ((render_info.flags & SDL_RENDERER_ACCELERATED) == SDL_RENDERER_ACCELERATED ? "Yes" : "No") << std::endl
         << "Display Format:    " << SDL_GetPixelFormatName (display->format()) << std::endl
         << "Fullscreen:        " << (gfx::screen::is_fullscreen() ? "Yes" : "No") << std::endl
         << std::ends;

    return temp.str ();
}
