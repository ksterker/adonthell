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

#include "base/logging.h"
#include "gfx/screen.h"
#include "surface_sdl.h"

extern u_int32 trans_color;

namespace gfx
{
    class screen_surface_sdl : public surface_sdl
    {
    public:
        screen_surface_sdl()
        {
            Renderer = NULL;
            Window = NULL;
        }

        ~screen_surface_sdl()
        {
            if (Renderer) SDL_DestroyRenderer(Renderer);
            if (Window) SDL_DestroyWindow(Window);

            Renderer = NULL;
            Window = NULL;
        }

        SDL_Renderer *get_renderer()
        {
            return Renderer;
        }

        u_int32 format() const
        {
            return SDL_GetWindowPixelFormat(Window);
        }

        void resize (u_int16 l, u_int16 h)
        {
            LOG(ERROR) << logging::indent() << "Invalid operation: Can't resize the screen surface!";
        }

        void clear ()
        {
            LOG(ERROR) << logging::indent() << "Invalid operation: Can't clear the screen surface!";
        }

        bool set_video_mode(u_int16 nl, u_int16 nh, u_int8 depth, u_int32 flags)
        {
            Window = SDL_CreateWindow ("Adonthell", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, nl, nh, flags);
            if (!Window)
            {
                LOG(ERROR) << logging::indent() << "Failed creating window: " << SDL_GetError();
                return false;
            }

            Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_TARGETTEXTURE);
            if (!Renderer)
            {
                LOG(WARNING) << logging::indent() << "Failed creating accelerated renderer: " << SDL_GetError();
                Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_SOFTWARE);
                if (!Renderer)
                {
                    LOG(ERROR) << logging::indent() << "Failed creating renderer: " << SDL_GetError();
                    return false;
                }
            }

            set_length(nl);
            set_height(nh);
            return true;
        }

    private:
        /// The output window
        SDL_Window *Window;

        /// the render target
        SDL_Renderer *Renderer;
    };
}

extern gfx::screen_surface_sdl *display;

#endif // GFX_SDL_SCREEN_H_
