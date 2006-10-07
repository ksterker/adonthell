/*
   $Id: screen_sdlgl.h,v 1.3 2006/10/07 17:00:55 gnurou Exp $

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
#include "gfx/sdlgl/surface_sdlgl.h"

extern u_int32 trans_color;

namespace gfx
{
    class screen_surface_sdlgl : public surface_sdlgl
    {
    public:
        ~screen_surface_sdlgl() { }
        void resize (u_int16 l, u_int16 h) { std::cerr << "Invalid operation: Can't resize the screen surface!\n"; }
        void clear () { std::cerr << "Invalid operation: Can't clear the screen surface!\n"; }
        bool set_video_mode(u_int16 nl, u_int16 nh, u_int8 depth, u_int32 flags)
        {
            SDL_Surface * vis = SDL_SetVideoMode (nl, nh, depth, flags);
            if (!vis) return false;
            set_length(nl);
            set_height(nh);
            
            // GL Initialization
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClearDepth(1.0);
			glShadeModel(GL_SMOOTH);
			// Non-power-of-two textures
			glEnable(GL_TEXTURE_RECTANGLE_ARB);

			glEnable(GL_BLEND);
			glDisable(GL_DEPTH_TEST);

			// TODO: glViewport and gluOrtho2D may not always be the same
			// resolution.
			glViewport(0,0,nl,nh);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluOrtho2D(0, nl, nh, 0);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
            
            return true;
        }

        virtual void fillrect (s_int16 x, s_int16 y, u_int16 l, u_int16 h,
                               u_int32 col, drawing_area * da_opt = NULL);
    };
}

extern gfx::screen_surface_sdlgl *display;

#endif // GFX_SDL_SCREEN_H_
