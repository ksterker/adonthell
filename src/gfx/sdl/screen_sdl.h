/*
   $Id: screen_sdl.h,v 1.2 2003/07/27 14:08:20 gnurou Exp $

   Copyright (C) 2003   Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


#ifndef GFX_SDL_SCREEN_H_
#define GFX_SDL_SCREEN_H_

#include "SDL.h"
#include "gfx/screen.h"
#include "gfx/sdl/surface_sdl.h"

extern u_int32 trans_color;

namespace gfx
{
    class screen_surface_sdl : public surface_sdl
    {
    public:
        ~screen_surface_sdl() { }
        SDL_Surface * get_vis() { return vis; }
        void resize (u_int16 l, u_int16 h) { std::cerr << "Invalid operation: Can't resize the screen surface!\n"; }
        void clear () { std::cerr << "Invalid operation: Can't clear the screen surface!\n"; }
        bool set_video_mode(u_int16 nl, u_int16 nh, u_int8 depth, u_int32 flags)
        {
            vis = SDL_SetVideoMode (nl, nh, depth, flags);
            if (!vis) return false;
            set_length(nl);
            set_height(nh);
            return true;
        }
    };
}

extern gfx::screen_surface_sdl display;

#endif // GFX_SDL_SCREEN_H_
