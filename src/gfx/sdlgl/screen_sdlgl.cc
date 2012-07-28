/*
   $Id: screen_sdlgl.cc,v 1.9 2008/10/05 10:53:26 ksterker Exp $

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
#define gfx_screen_set_video_mode _sdlgl_LTX_gfx_screen_set_video_mode
#define gfx_screen_update _sdlgl_LTX_gfx_screen_update
#define gfx_screen_trans_color _sdlgl_LTX_gfx_screen_trans_color
#define gfx_screen_clear _sdlgl_LTX_gfx_screen_clear
#define gfx_screen_get_surface _sdlgl_LTX_gfx_screen_get_surface
#endif

#include "screen_sdlgl.h"

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
    if (!display->set_video_mode(nl, nh, depth)) return false;

    // Setting up the window title
    SDL_WM_SetCaption ("Adonthell", NULL);

//    trans_color = SDL_MapRGB(display->get_vis()->format, gfx::screen::TRANS_RED, gfx::screen::TRANS_GREEN, 
//                             gfx::screen::TRANS_BLUE);

    return true;
}

bool gfx::screen_surface_sdlgl::set_video_mode(u_int16 nl, u_int16 nh, u_int8 depth)
{
	u_int32 SDL_flags = SDL_OPENGL;
	if (gfx::screen::is_fullscreen()) SDL_flags |= SDL_FULLSCREEN;

	// Can be changed
	SDL_Surface * vis = SDL_SetVideoMode (nl, nh, depth, SDL_flags);
	if (!vis) return false;
	set_length(nl);
	set_height(nh);

	// GL Initialization
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0);
	glShadeModel(GL_SMOOTH);
	// Non-power-of-two textures
	glEnable(GL_TEXTURE_RECTANGLE_ARB);

//	glEnable(GL_BLEND);
//	glDisable(GL_DEPTH_TEST);

	// TODO: glViewport and gluOrtho2D may not always be the same
	// resolution.
	// Can be changed
	glViewport(0,0,nl,nh);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, nl, nh, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); 
	return true;
}


void gfx_screen_update()
{
    SDL_GL_SwapBuffers();
}

u_int32 gfx_screen_trans_color()
{
    // TODO
    return 0;
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

