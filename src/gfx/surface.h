/*
   $Id: surface.h,v 1.4 2004/04/29 08:07:49 ksterker Exp $

   Copyright (C) 1999/2000/2001/2002/2003   Alexandre Courbot <alexandrecourbot@linuxgames.com>
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
 * @file   gfx/surface.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the surface interface.
 * 
 * 
 */


#ifndef SURFACE_H_
#define SURFACE_H_

#include "gfx/drawable.h"
#include <string>
#include <fstream>

#include "python/callback_support.h"

namespace gfx
{

    /**
     * Class where drawables can actually be drawn to.
     * Another name for a surface could be "pixmap", or "image". A surface
     * is nothing more than an array of pixels where drawables can be drawn to. 
     * A surface being a drawable itself, you can draw surfaces on other surfaces.
     *
     * A surface is always located off-screen - that is, you have to draw it if
     * you want it to be visible. It features many manipulation methods, like
     * images loading and special effects of many sorts (zooming, etc.).
     *
     */
    class surface : public drawable
    {
    public:
        /**
         * Default constructor.
         * The surface will be totally empty, that is
         * (0, 0) sized, no mask, alpha value of 255 (opaque).
         * 
         */ 
        surface ();

        /**
         * Destructor.
         * 
         */
        virtual ~surface () = 0;

        /**
         * @name Settings
         * These methods sets the parameters of the surface.
         * 
         */ 
        //@{
        

        /** 
         * 
         * Returns whether a surface is masked or not.
         * 
         * @return true if the surface is masked, false if it isn't.
         */
        bool is_masked () const
        {
            return is_masked_; 
        }

        /** 
         * Sets the mask parameter of the surface.
         * 
         * @param m true if the surface should be masked, false otherwise.
         */
        virtual void set_mask (bool m) = 0;
    
        /** 
         * Returns the alpha value of the surface.
         * 
         * 
         * @return the alpha value of the surface.
         */
        u_int8 alpha () const
        {
            return alpha_; 
        }

        /** 
         * Sets the alpha value of the surface.
         * 
         * @param a The new alpha value for this surface.
         */
        virtual void set_alpha (u_int8 a) = 0;
    
        //@}


        /**
         * @name Drawing Methods
         * 
         */

        //@{

    
        /** 
         * Draw the surface.
         * 
         * @param x X position where to draw.
         * @param y Y position where to draw.
         * @param da_opt optional drawing_area to use during the drawing operation.
         * @param target pointer to the surface where to draw the drawable. If NULL, draw on the screen.
         */ 
        void draw (s_int16 x, s_int16 y, const drawing_area * da_opt = NULL,
                   surface * target = NULL) const
        {
            draw (x, y, 0, 0, length (), height (), da_opt, target); 
        }

        /** 
         * Draw a part of the surface.
         * 
         * @param x X position where to draw.
         * @param y Y position where to draw.
         * @param sx X position where to start drawing from this image.
         * @param sy Y position where to start drawing from this image.
         * @param sl length of the part of this image to draw.
         * @param sh height of the part of this image to draw.
         * @param da_opt optional drawing_area to use during the drawing operation.
         * @param target pointer to the surface where to draw the drawable. If NULL, draw on the screen.
         *
         * @attention Not accessible from Python. Use draw_part () from Python instead.
         * @sa draw_part () 
         *
         */
        virtual void draw (s_int16 x, s_int16 y, s_int16 sx, s_int16 sy, u_int16 sl,
                           u_int16 sh, const drawing_area * da_opt = NULL,
                           surface * target = NULL) const = 0;
    
        /** 
         * Fills an area of the surface with a given color.
         *
         * @attention This method works with the actual surface format. You
         * can use map_color to convert a hex triplet into the same color in
         * the right format.
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
                               u_int32 col, drawing_area * da_opt = NULL) = 0;
    

        /**
         * Draw a line of color col between coordinates (sx, sy) and (ex, ey).
         *
         */ 
        virtual void draw_line (const s_int16 sx, const s_int16 sy, const s_int16 ex, const s_int16 ey, 
                                const u_int32 col, const drawing_area * da_opt = NULL) = 0;

        //@}

        /**
         * @name Pixel manipulation Methods
         * Use these methods to directly and quickly manipulate
         * pixels from a surface.
         * 
         */ 
        //@{
         
    
        /** 
         * Maps the color triplet made of (r, g, b) into the 
         * same color for the surface format.
         * 
         * @param r Red componant.
         * @param g Green componant.
         * @param b Blue componant.
         * 
         * @return The corresponding color for the hex triplet (r, g, b),
         *         but in the actual surface format.
         */
        virtual u_int32 map_color(u_int8 r, u_int8 g, u_int8 b) const = 0;

        /** 
         * Unmaps the color col (in the surface format) into the hex
         * triplet of its red, green and blue componants.
         * 
         * @param col The color to unmap.
         * @param r Red componant.
         * @param g Green componant.
         * @param b Blue componant.
         * 
         */
        virtual void unmap_color(u_int32 col, u_int8 & r, u_int8 & g, u_int8 & b) const = 0;

        /** 
         * Locks the surface.
         * Sometimes you may want to access directly the pixels of a surface. This
         * can be done with the get_pix ()  and put_pix () methods, thus you must
         * ABSOLUTELY lock the surface before doing so. This function is made for
         * that. Note that using get_pix () or put_pix () without locking the surface
         * may result in unpredictable behavior, crashes included.
         * 
         */
        virtual void lock () const = 0;
    
        /** 
         * Unlock the surface after you've worked on it's pixels with the
         * get_pix () and put_pix () methods.
         * 
         */
        virtual void unlock () const = 0;
    
        /** 
         * Puts a pixel of a given color.
         *
         * The col parameter is specific to the current screen depth,
         * and must come from a game's function like get_pix or
         * screen::trans_col ().
         * 
         * @attention This method works with the actual surface format. You
         * can use map_color to convert a hex triplet into the same color in
         * the right format.
         *
         * @param x X position of the pixel to change.
         * @param y Y position of the pixel to change.
         * @param col color to put.
         */
        virtual void put_pix (u_int16 x, u_int16 y, u_int32 col) = 0;

    
        /** 
         * Gets a pixel from the surface.
         *
         * The col parameter is specific to the current screen depth,
         * and can be used with functions like put_pix ().
         * 
         * @attention This method returns a color in the actual surface format. You
         * can use unmap_color to convert it to an hex triplet.

         * @param x X position of the pixel to change.
         * @param y Y position of the pixel to change.
         * @return Color of the pixel at (x, y).
         */
        virtual u_int32 get_pix (u_int16 x, u_int16 y) const = 0;


        //@}

#ifndef SWIG    
        /**
         * Surface copy.
         *
         * @attention Not available from Python. Use copy() from Python instead.
         * @sa copy ()
         */
        virtual surface& operator = (const surface& src) = 0;
#endif

        /**
         * Synonym of operator = to guarantee its access from Python.
         *
         * @sa operator = 
         */
        void copy (const surface& src) 
        {
            *this = src; 
        }
         
        /** 
         * Resize this surface. All the content will be lost.
         * 
         * @param l new length.
         * @param h new height.
         */
        virtual void resize (u_int16 l, u_int16 h) = 0;

        /** 
         * Resets the surface to it's initial state, that is totally
         * empty.
         * 
         */
        virtual void clear () = 0;

        /**
         * @name Loading/saving Methods
         * Load and save surfaces in various formats.
         * 
         */ 
        //@{

        /** Loads an image from an opened file, in PNM format, without
         *  alpha and mask values.
         *  @param file the opened file from which to read.
         *  @return \e true in case of success, false otherwise.
         *  @sa load_pnm () 
         */
        bool get_pnm (std::ifstream & file);
    
        /** Loads an image from a file name, in PNM format, without
         *  alpha and mask values.
         *  @param fname the name of the file to load.
         *  @return \e true in case of success, false otherwise.
         *  @sa get_pnm ()
         */ 
        bool load_pnm (const std::string & fname); 
        
        /** Saves an image into an opened file, in PNM format, without
         *  alpha and mask values.
         *
         *  @param file opened file where to save into.
         *  @return \e true in case of success, false otherwise.
         *  @sa save_pnm ()
         */
        bool put_pnm (std::ofstream & file) const;

        /** Saves an image into an file, in PNM format, without
         *  alpha and mask values.
         *
         *  @param fname file name where to save into.
         *  @return \e true in case of success, false otherwise.
         *  @sa put_pnm ()
         */
        bool save_pnm (const std::string & fname) const;

        //@}

#ifndef SWIG
        GET_TYPE_NAME_VIRTUAL(gfx::surface)
#endif // SWIG

    protected:
        /// Mask
        bool is_masked_;

        /// Alpha value
        u_int8 alpha_;

        /** 
         * Sets the surface pixel data from a given memory zone.
         * \e data must point to a memory area of size 
         * l * h * bytes_per_pixel, containing the pixel data in
         * the true color format, with the following masks:
         * - Red: red_mask
         * - Green: green_mask
         * - Blue: blue_mask
         * 
         * @param data Pointer to the new pixel data.
         * @param l length of the image (in pixels)
         * @param h height of the image (in pixels)
         * @param bytes_per_pixel Number of data bytes per pixel.
         * @param red_mask Red color mask.
         * @param green_mask Green color mask.
         * @param blue_mask Blue color mask.
         */
        virtual void set_data (void * data, u_int16 l, u_int16 h,
                               u_int8 bytes_per_pixel = RAW_BYTES_PER_PIXEL,
                               u_int32 red_mask = RAW_RED_MASK, u_int32 green_mask = RAW_GREEN_MASK,
                               u_int32 blue_mask = RAW_BLUE_MASK) = 0;

        /** 
         * Returns the surface pixel data in the chosen format.
         * The returned pointer points to a memory area of size
         * length() * height() * bytes_per_pixel containing the pixel 
         * data in the requested color format, with the following masks:
         * - Red: red_mask
         * - Green: green_mask
         * - Blue: blue_mask
         *
         * @attention The returned value has been allocated with calloc() - 
         * it's up to you to free() it when you no longer need it.
         * 
         * @param bytes_per_pixel Number of data bytes per pixel.
         * @param red_mask Red color mask.
         * @param green_mask Green color mask.
         * @param blue_mask Blue color mask.
         * @return Image data converted to the requested format.
         */
        virtual void * get_data (u_int8 bytes_per_pixel,
                                 u_int32 red_mask, u_int32 green_mask,
                                 u_int32 blue_mask) const = 0;

        /// Internal format red mask value
        static const u_int32 RAW_RED_MASK = 0xF800;
        /// Internal format green mask value
        static const u_int32 RAW_GREEN_MASK = 0x07E0;
        /// Internal format blue mask value
        static const u_int32 RAW_BLUE_MASK = 0x001F;
        /// Internal format number of bytes per pixel
        static const u_int8 RAW_BYTES_PER_PIXEL = 2;

    private:
        /**
         * Forbid copy construction.
         * 
         */ 
        surface (const surface & src); 
    };
}

#endif
