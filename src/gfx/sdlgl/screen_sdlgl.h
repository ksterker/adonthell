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


#ifndef GFX_SDL_SCREEN_H_
#define GFX_SDL_SCREEN_H_

#include "SDL.h"
#include "base/logging.h"
#include "gfx/screen.h"
#include "gfx/sdlgl/surface_sdlgl.h"

extern u_int32 trans_color;

namespace gfx
{
    class screen_surface_sdlgl : public surface_sdlgl /* TODO: not really... */
    {
    public:
        ~screen_surface_sdlgl() { }
        void resize (u_int16 l, u_int16 h) { LOG(ERROR) << logging::indent() << "Invalid operation: Can't resize the screen surface!"; }
        void clear () { LOG(ERROR) << logging::indent() << "Invalid operation: Can't clear the screen surface!"; }
        bool set_video_mode(u_int16 nl, u_int16 nh, u_int8 depth = 0);

        virtual void fillrect (s_int16 x, s_int16 y, u_int16 l, u_int16 h,
                               u_int32 col, drawing_area * da_opt = NULL);
    };
}

extern gfx::screen_surface_sdlgl *display;

#endif // GFX_SDL_SCREEN_H_
