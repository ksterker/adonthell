/*
   $Id: surface_sdl.h,v 1.1 2003/07/18 15:16:09 gnurou Exp $

   Copyright (C) 2003   Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef SURFACE_SDL_H_
#define SURFACE_SDL_H_


#include "gfx/surface.h"
#include "SDL.h"

namespace gfx
{
    class surface_sdl : public surface
    {
    public:
        surface_sdl ();

        ~surface_sdl (); 
    
        void set_mask (bool m); 

        void set_alpha (u_int8 a); 

        void draw (s_int16 x, s_int16 y, s_int16 sx, s_int16 sy, u_int16 sl,
                   u_int16 sh, const drawing_area * da_opt = NULL,
                   surface * target = NULL) const;

        void fillrect (s_int16 x, s_int16 y, u_int16 l, u_int16 h,
                       u_int32 col, drawing_area * da_opt = NULL);
    
        void draw_line (const s_int16 sx, const s_int16 sy, const s_int16 ex, const s_int16 ey, 
                        const u_int32 col, const drawing_area * da_opt = NULL); 

        u_int32 map_color(u_int8 r, u_int8 g, u_int8 b) const;
        void unmap_color(u_int32 col, u_int8 & r, u_int8 & g, u_int8 & b) const;
        void lock () const; 
        void unlock () const;
        void put_pix (u_int16 x, u_int16 y, u_int32 col); 
        u_int32 get_pix (u_int16 x, u_int16 y) const; 
    
        surface& operator = (surface& src); 

        void resize (u_int16 l, u_int16 h);

        void clear (); 

    protected:         
        void set_data (void * data, u_int16 l, u_int16 h,
                       u_int8 bytes_per_pixel = RAW_BYTES_PER_PIXEL,
                       u_int32 red_mask = RAW_RED_MASK, u_int32 green_mask = RAW_GREEN_MASK,
                       u_int32 blue_mask = RAW_BLUE_MASK);

        void * get_data (u_int8 bytes_per_pixel,
                         u_int32 red_mask, u_int32 green_mask,
                         u_int32 blue_mask) const;

    private: 
        SDL_Surface *vis;

        /// Has the mask setting changed?
        mutable bool mask_changed; 
     
        /// SDL_Rects used in every blitting function.
        static SDL_Rect srcrect, dstrect; 

        /// Used internally for blitting operations with drawing_areas.
        void setup_rects (u_int16 x, u_int16 y, const drawing_area * draw_to) const
        {
            setup_rects (x, y, 0, 0, length (), height (), draw_to); 
        }
    
        /// Used internally for blitting operations with drawing_areas.
        void setup_rects (s_int16 x, s_int16 y, s_int16 sx, s_int16 sy,
                          u_int16 sl, u_int16 sh, const drawing_area * draw_to) const; 

    };
}

#endif
