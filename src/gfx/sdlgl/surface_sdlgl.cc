/*
   $Id: surface_sdlgl.cc,v 1.7 2008/10/04 16:52:30 ksterker Exp $

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


#include <iostream>

#include "surface_sdlgl.h"
#include "screen_sdlgl.h"

namespace gfx
{
    surface_sdlgl::surface_sdlgl() : surface () 
    {
	texture = 0;
    }

    surface_sdlgl::~surface_sdlgl() 
    {
	// TODO
    }

    void surface_sdlgl::set_mask (bool m)
    {
        if (m != is_masked ())
        {
            is_masked_ = m;
            mask_changed = true; 
        }
    }

    // set alpha properties of surface
    void surface_sdlgl::set_alpha (const u_int8 & surface_alpha, const bool & alpha_channel)
    {
        alpha_ = surface_alpha;
        alpha_channel_ = alpha_channel;
    }

    void surface_sdlgl::draw (s_int16 x, s_int16 y, s_int16 sx, s_int16 sy, u_int16 sl,
                            u_int16 sh, const drawing_area * da_opt,
                            surface * target) const
    {
	// If alpha is equal to 0, do nothing...
	if (alpha() == 0) return;

	// Otherwise we are going to draw something.
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	// Blending func - we enable alpha by default.
	if (!is_masked() && alpha() == 255) glBlendFunc(GL_ONE, GL_ZERO);
	else glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Set the global alpha
	glColor4ub(255, 255, 255, alpha());
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, texture);
	glBegin(GL_QUADS);
		glTexCoord2i(0, 0); glVertex3i(x, y, 0);
		glTexCoord2i(sl, 0); glVertex3i(x + sl, y, 0);
		glTexCoord2i(sl, sh); glVertex3i(x + sl, y + sh, 0);
		glTexCoord2i(0, sh); glVertex3i(x, y + sh, 0);
	glEnd();
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
    }

    void surface_sdlgl::fillrect (s_int16 x, s_int16 y, u_int16 l, u_int16 h, u_int32 col, 
                                drawing_area * da_opt)
    {
	// TODO
    }

    void surface_sdlgl::draw_line(const s_int16 x1, const s_int16 y1, const s_int16 x2, const s_int16 y2, 
                                const u_int32 color, const drawing_area * da_opt)
    {
	// TODO
    }

    u_int32 surface_sdlgl::map_color(const u_int8 & r, const u_int8 & g, const u_int8 & b, const u_int8 & a) const
    {
	// TODO
        return 0;
    }

    void surface_sdlgl::unmap_color(u_int32 col, u_int8 & r, u_int8 & g, u_int8 & b, u_int8 & a) const
    {
	// TODO
    }

    void surface_sdlgl::lock () const
    {
	// TODO
    }

    void surface_sdlgl::unlock () const
    {
	// TODO
    }

    void surface_sdlgl::put_pix (u_int16 x, u_int16 y, u_int32 col) 
    {
	// TODO
    }

    u_int32 surface_sdlgl::get_pix (u_int16 x, u_int16 y) const
    {
	// TODO
        return 0;
    }

    surface & surface_sdlgl::operator = (const surface& src)
    {
	// TODO
        return *this; 
    }

    void surface_sdlgl::resize (u_int16 l, u_int16 h)
    {
	// TODO
    }

    void surface_sdlgl::clear () 
    {
	// TODO
    }

    void surface_sdlgl::set_data(void * data, u_int16 l, u_int16 h, u_int8 bytes_per_pixel, u_int32 red_mask, 
                               u_int32 green_mask, u_int32 blue_mask, u_int32 alpha_mask)
    {
	if (texture) { glDeleteTextures(1, &texture); texture = 0; /*glDeleteTextures(1, &mask_texture); mask_texture = 0;*/ }

	// Keep the raw data
	texture_data = data;

	set_length(l);
	set_height(h);

	u_int8 * ndata = (u_int8*) malloc(l * h * 4);
	for (u_int32 i = 0; i < (u_int32) l * h; i++)
	{
		u_int8 red = ((u_int8*)data)[i * bytes_per_pixel];
		u_int8 green = ((u_int8*)data)[i * bytes_per_pixel + 1];
		u_int8 blue = ((u_int8*)data)[i * bytes_per_pixel + 2];

		u_int8 alpha;
		if (red == screen::TRANS_RED &&
		    green == screen::TRANS_GREEN &&
		    blue == screen::TRANS_BLUE)
			{ alpha = 0x00; }
		else { alpha = 0xff; }

		((u_int8*)ndata)[i * 4] = red;
		((u_int8*)ndata)[i * 4 + 1] = green;
		((u_int8*)ndata)[i * 4 + 2] = blue;
		((u_int8*)ndata)[i * 4 + 3] = alpha;
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, texture);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGBA8, l, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, ndata);

	free(ndata);
    }

    void * surface_sdlgl::get_data (u_int8 bytes_per_pixel,
                                  u_int32 red_mask, u_int32 green_mask,
                                  u_int32 blue_mask) const
    {
	// TODO
	return NULL;
    }

    void surface_sdlgl::setup_rects (s_int16 x, s_int16 y, s_int16 sx, s_int16 sy,
                                   u_int16 sl, u_int16 sh, const drawing_area * draw_to) const
    {
	// TODO
    }
}
