/*
   $Id: screen_sdl.h,v 1.3 2003/11/22 09:37:12 ksterker Exp $

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

extern gfx::screen_surface_sdl *display;

#endif // GFX_SDL_SCREEN_H_
