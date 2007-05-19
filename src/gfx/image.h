/*
 $Id: image.h,v 1.1 2007/05/19 07:42:08 ksterker Exp $
 
 Copyright (C) 1999/2000/2001/2002 Alexandre Courbot <alexandrecourbot@linuxgames.com>
 Copyright (C) 2007 Kai Sterker <kaisterker@linuxgames.com>
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


/**
 * @file   gfx/image.h 
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Declares the image class.
 *  
 */

#ifndef GFX_IMAGE_H
#define GFX_IMAGE_H

#include "gfx/drawable.h"

namespace gfx
{
    class image : public drawable
    {
    public:
        /** 
         * Creates an image with a specified size.
         * @param lenght width of the image.
         * @param height height of the image.
         */
        image (const u_int16 & length, const u_int16 & height);
        
       /** 
        * Draw the object on the %screen.
        * 
        * @param x X position where to draw.
        * @param y Y position where to draw.
        * @param da_opt optional drawing_area to use during the drawing operation.
        * @param target pointer to the surface where to draw the drawable. If NULL,
        *               draw on the screen.
        */
        virtual void draw (s_int16 x, s_int16 y, const drawing_area * da_opt = NULL,
                           surface * target = NULL) const;

        
        /** 
         * Fills an area of the surface with a given color.
         *
         * This function is independant of the screen depth. You just give
         * the red, green and blue triplets of the color you want to fill with.
         * 
         * @param x X position where to fill.
         * @param y Y position where to fill.
         * @param l length of the area to fill.
         * @param h height of the area to fill.
         * @param r red value of the color to fill with.
         * @param g green value of the color to fill with.
         * @param b blue value of the color to fill with.
         * @param da_opt optionnal drawing_area to use during the fill operation.
         *
         * @attention Not accessible from Python. Use fillrect_rgb from Python instead.
         * @sa fillrect_rgb ()  
         */
        void fillrect (s_int16 x, s_int16 y, u_int16 l, u_int16 h, u_int8 r,
                       u_int8 g, u_int8 b, drawing_area * da_opt = NULL)
        {
            // TODO: handle color conversion with map_color
            // fillrect (x, y, l, h, SDL_MapRGB (vis->format, r, g, b), da_opt);
        }
        
        /**
         * Fills an area of the surface with a given color.
         *
         * @param x X position where to fill.
         * @param y Y position where to fill.
         * @param l length of the area to fill.
         * @param h height of the area to fill.
         * @param col color to fill the surface with.
         * @param da_opt optionnal drawing_area to use during the fill operation.
         *
         */
        virtual void fillrect (s_int16 x, s_int16 y, u_int16 l, u_int16 h,
                               u_int32 col, drawing_area * da_opt = NULL)        
        {
            // TODO: pass on to surface
        }
        
        void set_alpha (const u_int8 & alpha)
        {
            // TODO: pass on to surface
        }
        
#ifndef SWIG
        GET_TYPE_NAME_VIRTUAL(gfx::animation)
#endif // SWIG
        
    private:
        /**
         * Forbid value passing.
         */
        image (const image& src);    
    };
}

#endif // GFX_IMAGE_H