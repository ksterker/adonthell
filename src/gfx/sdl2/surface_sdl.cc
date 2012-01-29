/*
   Copyright (C) 2003 Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Copyright (C) 2011 Kai Sterker <kai.sterkerr@gmail.com>
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
#include <algorithm>

#include "surface_sdl.h"
#include "screen_sdl.h"

namespace gfx
{
    SDL_Rect surface_sdl::srcrect; 
    SDL_Rect surface_sdl::dstrect; 
    SDL_Texture *surface_sdl::RenderTarget = NULL;

    surface_sdl::surface_sdl() : surface_ext () 
    { 
        Surface = NULL;
        mask_changed = false; 
    }

    surface_sdl::~surface_sdl() 
    {
        if (Surface) SDL_DestroyTexture (Surface);
    }

    void surface_sdl::set_mask (bool m)
    {
        // since SDL textures do not support masking, we need to
        // convert the masked image into the appropriate RGBA
        // representation. This behavior is different than the
        // SDL 1.2 backend, but with the current usage of set_mask,
        // we can get away by doing this here.

        if (!Surface)
        {
            LOG(FATAL) << "surface::set_mask called before creating surface!";
        }

        if (m && m != is_masked ())
        {
            is_masked_ = true;
       
            // TODO: convert pixel of mask color to transparency
            SDL_Surface *s2 = SDL_CreateRGBSurfaceFrom(NULL, length(), height(),
                            32, 0, R_MASK, G_MASK, B_MASK, A_MASK);
            u_int32 format = s2->format->format;

            SDL_Texture *tmp = SDL_CreateTexture (display->get_renderer(), format, SDL_TEXTUREACCESS_TARGET, length(), height());
            if (!tmp) LOG(ERROR) << "*** surface_sdl::set_mask: " << SDL_GetError();

            set_render_target (tmp);
            SDL_SetTextureAlphaMod (Surface, SDL_ALPHA_OPAQUE);
            SDL_SetTextureBlendMode (Surface, SDL_BLENDMODE_NONE);
            SDL_RenderCopy (display->get_renderer(), Surface, NULL, NULL);
            SDL_DestroyTexture(Surface);

            Surface = tmp;
            alpha_channel_ = true;
        }
    }

    void surface_sdl::set_alpha (const u_int8 & t, const bool & alpha_channel)
    {
        if ((t == 255) && (alpha_ != 255) && Surface)
        {
            SDL_SetTextureAlphaMod(Surface, t);
            SDL_SetTextureBlendMode(Surface, SDL_BLENDMODE_NONE);
        }
        
        else if (!alpha_channel && alpha_channel_ && Surface)
        {
            SDL_SetTextureBlendMode(Surface, SDL_BLENDMODE_NONE);
        }

        alpha_ = t;
        alpha_channel_ = alpha_channel || is_masked_;
    }

    void surface_sdl::draw (s_int16 x, s_int16 y, s_int16 sx, s_int16 sy, u_int16 sl,
                            u_int16 sh, const drawing_area * da_opt,
                            surface * target) const
    { 
        setup_rects (x, y, sx, sy, sl, sh, da_opt); 

        if (!dstrect.w || !dstrect.h)
            return;

        SDL_Texture *render_target = target != NULL ? ((surface_sdl*) target)->Surface : NULL;
        set_render_target (render_target);

        if (alpha_channel_ || alpha_ != 255)
        {
            if (!alpha_channel_ || is_masked_) SDL_SetTextureAlphaMod(Surface, alpha_);
            SDL_SetTextureBlendMode(Surface, SDL_BLENDMODE_BLEND);
        }

        if (base::Scale > 1)
        {
            dstrect.x *= base::Scale;
            dstrect.y *= base::Scale;
            dstrect.w *= base::Scale;
            dstrect.h *= base::Scale;
        }

        SDL_RenderCopy (display->get_renderer(), Surface, &srcrect, &dstrect);
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

        u_int8 r, g, b, a;
        unmap_color(col, r, g, b, a);

        if (base::Scale > 1)
        {
            dstrect.x *= base::Scale;
            dstrect.y *= base::Scale;
            dstrect.w *= base::Scale;
            dstrect.h *= base::Scale;
        }

        set_render_target (Surface);

        SDL_SetRenderDrawBlendMode(display->get_renderer(), SDL_BLENDMODE_NONE);
        SDL_SetRenderDrawColor(display->get_renderer(), r, g, b, a);
        SDL_RenderFillRect(display->get_renderer(), &dstrect);
    }

    // convert RGBA color to surface format
    u_int32 surface_sdl::map_color (const u_int8 & r, const u_int8 & g, const u_int8 & b, const u_int8 & a) const
    {
        return (a << 24) | (r << 16) | (g << 8) | b;
    }

    // convert surface color format into RGBA
    void surface_sdl::unmap_color(u_int32 col, u_int8 & r, u_int8 & g, u_int8 & b, u_int8 & a) const
    {
        a = (col & 0xFF000000) >> 24;
        r = (col & 0x00FF0000) >> 16;
        g = (col & 0x0000FF00) >> 8;
        b = (col & 0x000000FF);
    }

    void surface_sdl::put_pix (u_int16 x, u_int16 y, u_int32 col) 
    {
        if (base::Scale > 1)
        {
            fillrect(x, y, 1, 1, col);
            return;
        }

        u_int8 r, g, b, a;
        unmap_color(col, r, g, b, a);

        set_render_target (Surface);

        SDL_SetRenderDrawBlendMode(display->get_renderer(), SDL_BLENDMODE_NONE);
        SDL_SetRenderDrawColor(display->get_renderer(), r, g, b, a);
        SDL_RenderDrawPoint(display->get_renderer(), x, y);
    }

    u_int32 surface_sdl::get_pix (u_int16 x, u_int16 y) const
    {
        u_int32 pixel;

#ifdef __BIG_ENDIAN__
        u_int32 format = has_alpha_channel() ? SDL_PIXELFORMAT_ARGB8888 : SDL_PIXELFORMAT_RGB24;
#else
        u_int32 format = has_alpha_channel() ? SDL_PIXELFORMAT_ABGR8888 : SDL_PIXELFORMAT_BGR24;
#endif
        const SDL_Rect rect = { x, y, 1, 1 };
        SDL_RenderReadPixels(display->get_renderer(), &rect, format, (void*) &pixel, 4);

        return pixel;
    }

    void surface_sdl::scale(surface *target, const u_int32 & factor) const
    {
        // scaling of the final result is handled in surface_sdl::draw
        if (!target || target == display) return;

    	if (length() * factor > target->length() ||
    		height() * factor > target->height())
    		return;

    	surface_sdl *target_sdl = (surface_sdl*) target;

    	SDL_Rect dest = { 0, 0, length() * factor, height() * factor };
    	set_render_target (target_sdl->Surface);
        SDL_RenderCopy(display->get_renderer(), Surface, NULL, &dest);
    }

    surface & surface_sdl::operator = (const surface& src)
    {
        const surface_sdl & src_sdl = (const surface_sdl &) src;

        (drawable&) (*this) = (drawable&) src;
        alpha_channel_ = src.has_alpha_channel();
        is_masked_ = src.is_masked();
        alpha_ = src.alpha();

        if (Surface) SDL_DestroyTexture(Surface);
        if (!src_sdl.Surface)
        {
            Surface = NULL;
        }
        else
        {
            int l, h;
            u_int32 format;

            SDL_QueryTexture(src_sdl.Surface, &format, NULL, &l, &h);
            Surface = SDL_CreateTexture (display->get_renderer(), format, SDL_TEXTUREACCESS_TARGET, length(), height());

            set_render_target (Surface);
            SDL_RenderCopy(display->get_renderer(), src_sdl.Surface, NULL, NULL);
        }

        return *this;
    }

    void surface_sdl::resize (u_int16 l, u_int16 h)
    {
        if (l == length () && h == height ()) return;

        if (Surface) SDL_DestroyTexture(Surface);

        set_length (l);
        set_height (h); 

        // is screen surface initialized?
        if (display->get_renderer())
        {
#ifdef __BIG_ENDIAN__
            u_int32 format = has_alpha_channel() ? SDL_PIXELFORMAT_ARGB8888 : SDL_PIXELFORMAT_RGB24;
#else
            u_int32 format = has_alpha_channel() ? SDL_PIXELFORMAT_ABGR8888 : SDL_PIXELFORMAT_BGR24;
#endif
            Surface = SDL_CreateTexture (display->get_renderer(), format, SDL_TEXTUREACCESS_TARGET, l, h);
            if (!Surface) LOG(ERROR) << "*** surface::resize: " << SDL_GetError();
        }
        else
        {
            Surface = NULL;
            LOG(FATAL) << "*** surface:resize: screen surface not initialized!";
        }
    }

    void surface_sdl::clear () 
    {
        if (Surface)
        {
            SDL_DestroyTexture(Surface);
            Surface = NULL;
            set_length (0);
            set_height (0); 
            set_alpha (255);
            is_masked_ = false;
        }
    }

    void surface_sdl::set_data(void * data, u_int16 l, u_int16 h, u_int8 bytes_per_pixel, u_int32 red_mask, 
                               u_int32 green_mask, u_int32 blue_mask, u_int32 alpha_mask)
    {
        if (Surface) SDL_DestroyTexture(Surface);

        set_length(l);
        set_height(h);

        alpha_channel_ = alpha_mask != 0;

        u_int32 format = SDL_MasksToPixelFormatEnum (bytes_per_pixel * 8, red_mask, green_mask, blue_mask, alpha_mask);
        Surface = SDL_CreateTexture (display->get_renderer(), format, SDL_TEXTUREACCESS_TARGET, l, h);
        if (!Surface) LOG(ERROR) << "*** surface_sdl::set_data: " << SDL_GetError();

        SDL_UpdateTexture (Surface, NULL, data, l*bytes_per_pixel);

        free (data);
    }

    void * surface_sdl::get_data (u_int8 bytes_per_pixel,
                                  u_int32 red_mask, u_int32 green_mask,
                                  u_int32 blue_mask, u_int32 alpha_mask) const
    {

        u_int32 dst_format = SDL_MasksToPixelFormatEnum(bytes_per_pixel*8, red_mask, green_mask, blue_mask, alpha_mask);
        void *dst_pixels = SDL_calloc (bytes_per_pixel, length() * height());
        int dst_pitch = length() * bytes_per_pixel;

        // TODO: FIXME does return mostly garbage
        set_render_target(Surface);
        SDL_RenderReadPixels(display->get_renderer(), NULL, dst_format, dst_pixels, dst_pitch);

        return dst_pixels;
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

    void surface_sdl::set_render_target (SDL_Texture *target) const
    {
        if (target == RenderTarget) return;
        else if (target == display->Surface)
        {
            if (RenderTarget == NULL) return;
            else SDL_SetRenderTarget(display->get_renderer(), NULL);
        }
        else SDL_SetRenderTarget(display->get_renderer(), target);

        RenderTarget = target;
    }
}
