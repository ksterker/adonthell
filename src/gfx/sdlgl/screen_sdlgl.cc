/*
   $Id: screen_sdlgl.cc,v 1.3 2006/10/07 21:16:21 gnurou Exp $

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
#define gfx_screen_set_video_mode sdlgl_LTX_gfx_screen_set_video_mode
#define gfx_screen_update sdlgl_LTX_gfx_screen_update
#define gfx_screen_trans_color sdlgl_LTX_gfx_screen_trans_color
#define gfx_screen_clear sdlgl_LTX_gfx_screen_clear
#define gfx_screen_get_surface sdlgl_LTX_gfx_screen_get_surface
#endif

#include "gfx/sdlgl/screen_sdlgl.h"

#include <GL/gl.h>

u_int32 trans_color = 0;

extern "C"
{
    bool gfx_screen_set_video_mode(u_int16 nl, u_int16 nh, u_int8 depth);
    void gfx_screen_update();
    u_int32 gfx_screen_trans_color();
    void gfx_screen_clear();
    gfx::surface * gfx_screen_get_surface();
}

gfx::screen_surface_sdlgl *display = NULL;

bool gfx_screen_set_video_mode(u_int16 nl, u_int16 nh, u_int8 depth)
{
    u_int32 SDL_flags = SDL_OPENGL;

    if (!display->set_video_mode(nl, nh, depth, SDL_flags)) return false;

    // Setting up the window title
    SDL_WM_SetCaption ("Adonthell", NULL);

//    trans_color = SDL_MapRGB(display->get_vis()->format, gfx::screen::TRANS_RED, gfx::screen::TRANS_GREEN, 
//                             gfx::screen::TRANS_BLUE);

    return true;
}

void gfx_screen_update()
{
    SDL_GL_SwapBuffers();
}

u_int32 gfx_screen_trans_color()
{
// TODO
}

void gfx_screen_clear()
{
// TODO
}

gfx::surface * gfx_screen_get_surface()
{
    return display;
}

void gfx::screen_surface_sdlgl::fillrect (s_int16 x, s_int16 y, u_int16 l, u_int16 h,
               u_int32 col, drawing_area * da_opt)
{
	glBegin(GL_QUADS);
	glColor3ub((col & 0xff0000) >> 16, (col & 0x00ff00) >> 8, col & 0x0000ff);
	glVertex3i(x, y, 0);
	glVertex3i(x + l, y, 0);
	glVertex3i(x + l, y + l, 0);
	glVertex3i(x, y + l, 0);
	glEnd();
	glColor3ub(0xff,0xff,0xff);
}

