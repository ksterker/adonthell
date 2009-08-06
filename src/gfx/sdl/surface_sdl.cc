/*
   $Id: surface_sdl.cc,v 1.14 2009/03/06 22:53:30 ksterker Exp $

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

#include "surface_sdl.h"
#include "screen_sdl.h"

namespace gfx
{
    SDL_Rect surface_sdl::srcrect; 
    SDL_Rect surface_sdl::dstrect; 

    surface_sdl::surface_sdl() : surface_ext () 
    { 
        vis = NULL;
        vis_data = NULL;
        mask_changed = false; 
    }

    surface_sdl::~surface_sdl() 
    {
        if (vis) SDL_FreeSurface (vis);
        vis = NULL;

        if (this->vis_data) free(this->vis_data);
        this->vis_data = NULL;
    }

    void surface_sdl::set_mask (bool m)
    {
        if (m != is_masked ())
        {
            is_masked_ = m;
            mask_changed = true; 
        }
    }


    void surface_sdl::set_alpha (const u_int8 & t, const bool & alpha_channel)
    {
        if ((t == 255) && (alpha_ != 255) && vis)
            SDL_SetAlpha (vis, 0, 0);
        alpha_ = t;
        alpha_channel_ = alpha_channel;
    }

    void surface_sdl::draw (s_int16 x, s_int16 y, s_int16 sx, s_int16 sy, u_int16 sl,
                            u_int16 sh, const drawing_area * da_opt,
                            surface * target) const
    { 
        SDL_Surface * display_target;

        if (target == NULL) display_target = display->vis; 
        else display_target = ((surface_sdl *)target)->vis;

        setup_rects (x, y, sx, sy, sl, sh, da_opt); 

        if (!dstrect.w || !dstrect.h)
            return;

        if (mask_changed)
        {
            mask_changed = false;
            if (is_masked ())
                SDL_SetColorKey (vis, SDL_SRCCOLORKEY | SDL_RLEACCEL, trans_color);
            else
                SDL_SetColorKey (vis, 0, 0); 
        }

        if (alpha () != 255 || has_alpha_channel())
            SDL_SetAlpha (vis, SDL_SRCALPHA, alpha_);

        SDL_BlitSurface (vis, &srcrect, display_target, &dstrect); 
    }

    void surface_sdl::fillrect (s_int16 x, s_int16 y, u_int16 l, u_int16 h, u_int32 col, 
                                drawing_area * da_opt)
    {
        if (da_opt) 
        {
            drawing_area da = da_opt->setup_rects (); 
            dstrect.x = da.x();
            dstrect.y = da.y();
            dstrect.w = da.length();
            dstrect.h = da.height();
        }
        else
        { 
            dstrect.x = x;
            dstrect.y = y;
            dstrect.w = l;
            dstrect.h = h;
        }

        SDL_FillRect (vis, &dstrect, col);
    }

    // convert RGBA color to surface format
    u_int32 surface_sdl::map_color (const u_int8 & r, const u_int8 & g, const u_int8 & b, const u_int8 & a) const
    {
        return SDL_MapRGBA(vis->format, r, g, b, a);
    }

    // convert surface color format into RGBA
    void surface_sdl::unmap_color(u_int32 col, u_int8 & r, u_int8 & g, u_int8 & b, u_int8 & a) const
    {
        SDL_GetRGBA(col, vis->format, &r, &g, &b, &a);
    }

    void surface_sdl::lock () const
    {
        if (!length () || !height ()) return;   
        if (SDL_MUSTLOCK(vis)) 
            SDL_LockSurface (vis);
    }

    void surface_sdl::unlock () const
    {
        if (!length () || !height ()) return;   
        if (SDL_MUSTLOCK(vis)) 
            SDL_UnlockSurface (vis);
    }

    void surface_sdl::put_pix (u_int16 x, u_int16 y, u_int32 col) 
    {
        u_int8 * offset = ((Uint8 *) vis->pixels) + y * vis->pitch
            + x*vis->format->BytesPerPixel;

        switch (vis->format->BytesPerPixel) 
        {
            case 1:
                *((Uint8 *) (offset)) = (Uint8) col;
                break;
            case 2:
                *((Uint16 *) (offset)) = (Uint16) col;
                break;
            case 3:
            {
                u_int8 r, g, b;

                r = (col >> vis->format->Rshift);
                g = (col >> vis->format->Gshift);
                b = (col >> vis->format->Bshift);
                *((offset) + (vis->format->Rshift >> 3)) = r; 
                *((offset) + (vis->format->Gshift >> 3)) = g;
                *((offset) + (vis->format->Bshift >> 3)) = b;
                break;
            }
            case 4:
                *((Uint32 *) (offset)) = (Uint32) col;
                break;
        }
    }

    u_int32 surface_sdl::get_pix (u_int16 x, u_int16 y) const
    {
        u_int32 col;

        u_int8 * offset = ((Uint8 *) vis->pixels) + y * vis->pitch
            + x * vis->format->BytesPerPixel;

        switch (vis->format->BytesPerPixel) 
        {
            case 1:
                col = *((Uint8 *)(offset));
                break;
            case 2:
                col = *((Uint16 *)(offset));
                break;
            case 3:
            {
                u_int8 r, g, b;
                col = 0;
                u_int32 t;

                r = *((offset) + (vis->format->Rshift >> 3)); 
                g = *((offset) + (vis->format->Gshift >> 3));
                b = *((offset) + (vis->format->Bshift >> 3));

                t = r << vis->format->Rshift;
                col |= t; 
                t = g << vis->format->Gshift;
                col |= t; 
                t = b << vis->format->Bshift;
                col |= t; 

                break;
            }
            case 4:
                col = *((Uint32 *)(offset));
                break;
            default:
                col = 0;
                break;
        }
        return col;
    }

    surface & surface_sdl::operator = (const surface& src)
    {
        const surface_sdl & src_sdl = (const surface_sdl &) src;

        (drawable&) (*this) = (drawable&) src;
        alpha_channel_ = src.has_alpha_channel();
        is_masked_ = src.is_masked();
        alpha_ = src.alpha();
        if (vis) SDL_FreeSurface (vis);
        if (!src_sdl.vis) 
            vis = NULL;
        else
            vis = SDL_DisplayFormat (src_sdl.vis);
        return *this; 
    }

    void surface_sdl::resize (u_int16 l, u_int16 h)
    {
        if (l == length () && h == height ()) return;

        if (vis) SDL_FreeSurface (vis); 

        set_length (l);
        set_height (h); 

        // is screen surface initialized?
        if (display->vis)
        {
            // create surface with per-pixel alpha?
            if (alpha_channel_)
            {
                SDL_Surface *tmp = SDL_CreateRGBSurface (
                    SDL_HWSURFACE | SDL_SRCCOLORKEY | SDL_SRCALPHA | SDL_ASYNCBLIT,
                    length (), height (), BYTES_PER_PIXEL*8,
                    R_MASK, G_MASK, B_MASK, A_MASK);
                
                // does created surface match screen?
                if (display->vis->format->BytesPerPixel != BYTES_PER_PIXEL)
                {
                    vis = SDL_DisplayFormatAlpha (tmp);
                    SDL_FreeSurface (tmp);
                }
                else
                {
                    vis = tmp;
                }
            }
            else
            {
                vis = SDL_CreateRGBSurface (SDL_HWSURFACE | SDL_SRCCOLORKEY | SDL_SRCALPHA | SDL_ASYNCBLIT,
                    length (), height (),
                    display->vis->format->BitsPerPixel,
                    display->vis->format->Rmask,
                    display->vis->format->Gmask,
                    display->vis->format->Bmask,
                    display->vis->format->Amask);
            }
        }
        else
        {
            fprintf (stderr, "*** surface:resize: screen surface not initialized!\n");
        }
    }

    void surface_sdl::clear () 
    {
        if (vis) 
        {
            SDL_FreeSurface (vis);
            vis = NULL; 
            set_length (0);
            set_height (0); 
            set_alpha (255);
            set_mask (false);
        }
    }

    void surface_sdl::set_data(void * data, u_int16 l, u_int16 h, u_int8 bytes_per_pixel, u_int32 red_mask, 
                               u_int32 green_mask, u_int32 blue_mask, u_int32 alpha_mask)
    {
        if (vis) SDL_FreeSurface(vis);
        if (this->vis_data) free(this->vis_data);
        this->vis_data = data;

        set_length(l);
        set_height(h);

        SDL_Surface * tmp = SDL_CreateRGBSurfaceFrom(this->vis_data, length(),
                                                     height(), bytes_per_pixel * 8,
                                                     length() * bytes_per_pixel,
                                                     red_mask, green_mask,
                                                     blue_mask, alpha_mask);

        if (alpha_mask)
        {
            vis = SDL_DisplayFormatAlpha(tmp);
            alpha_channel_ = true;
        }
        else
            vis = SDL_DisplayFormat(tmp);

        SDL_FreeSurface(tmp);
    }

    void * surface_sdl::get_data (u_int8 bytes_per_pixel,
                                  u_int32 red_mask, u_int32 green_mask,
                                  u_int32 blue_mask, u_int32 alpha_mask) const
    {
        SDL_Surface * tmp = SDL_CreateRGBSurface(0, 10, 10,
                                                 bytes_per_pixel * 8,
                                                 red_mask, green_mask, blue_mask, alpha_mask);
        
        SDL_Surface * tmp2 = SDL_ConvertSurface(vis, tmp->format, 0);
        SDL_FreeSurface(tmp);
        
        void * ret = calloc(bytes_per_pixel, length() * height());
        memcpy(ret, tmp2->pixels, length() * height() * bytes_per_pixel);
        SDL_FreeSurface(tmp2);
        
        return ret;
    }
    
    void surface_sdl::setup_rects (s_int16 x, s_int16 y, s_int16 sx, s_int16 sy,
                                   u_int16 sl, u_int16 sh, const drawing_area * draw_to) const
    {
        if (draw_to)
        { 
            drawing_area im_zone (x, y, sl, sh);
            drawing_area da_int = draw_to->setup_rects ();

            im_zone.assign_drawing_area (&da_int);
            da_int = im_zone.setup_rects ();
            SDL_Rect tr;
            tr.x = da_int.x();
            tr.y = da_int.y();
            tr.w = da_int.length();
            tr.h = da_int.height();

            dstrect = tr; 
            srcrect = dstrect;
            srcrect.x = x < dstrect.x ? sx + dstrect.x - x : sx;
            srcrect.y = y < dstrect.y ? sy + dstrect.y - y : sy;
        }
        else
        {
            srcrect.x = sx;
            srcrect.y = sy;
            srcrect.w = sl;
            srcrect.h = sh;

            dstrect = srcrect;

            dstrect.x = x;
            dstrect.y = y;
        } 
    }
}
