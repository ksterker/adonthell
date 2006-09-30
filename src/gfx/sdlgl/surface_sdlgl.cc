/*
   $Id: surface_sdlgl.cc,v 1.2 2006/09/30 21:05:08 gnurou Exp $

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
    SDL_Rect surface_sdlgl::srcrect; 
    SDL_Rect surface_sdlgl::dstrect; 

    surface_sdlgl::surface_sdlgl() : surface () 
    {
	// TODO
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


    void surface_sdlgl::set_alpha (u_int8 t)
    {
        if ((t == 255) && (alpha_ != 255) && vis)
            SDL_SetAlpha (vis, 0, 0);
        alpha_ = t;
    }

    void surface_sdlgl::draw (s_int16 x, s_int16 y, s_int16 sx, s_int16 sy, u_int16 sl,
                            u_int16 sh, const drawing_area * da_opt,
                            surface * target) const
    {
	// TODO
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

    u_int32 surface_sdlgl::map_color(u_int8 r, u_int8 g, u_int8 b) const
    {
	// TODO
    }

    void surface_sdlgl::unmap_color(u_int32 col, u_int8 & r, u_int8 & g, u_int8 & b) const
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
	// TODO
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
