/*
   $Id: surface.h,v 1.20 2009/04/04 19:10:40 ksterker Exp $

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

#include "base/flat.h"
#include "gfx/drawable.h"
#include <fstream>

#ifdef __BIG_ENDIAN__
#   define R_MASK 0x00ff0000
#   define G_MASK 0x0000ff00
#   define B_MASK 0x000000ff
#   define A_MASK 0xff000000
#else
#   define R_MASK 0x000000ff
#   define G_MASK 0x0000ff00
#   define B_MASK 0x00ff0000
#   define A_MASK 0xff000000
#endif
#define BYTES_PER_PIXEL 4

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
         * Sets the mask parameter of the surface. If set to true, all
         * parts of the image colored with the mask color will be 
         * transparent. The mask color can be obtained by a call to 
         * gfx::screen::trans_color(), as it depends on color depth.
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
         * Returns whether the surface has an alpha channel.
         * @return true if it has an alpha channel, false otherwise.
         */
        bool has_alpha_channel () const
        {
            return alpha_channel_;
        }
        
        /**
         * Sets the alpha value of the surface. If alpha_channel is set to true,
         * then the surface_alpha value will be silently ignored. Needs to
         * be called before surface::resize to take effect.
         *
         * @param surface_alpha The new alpha value for this surface.
         * @param alpha_channel Whether to enable per-pixel alpha for the surface.
         */
        virtual void set_alpha (const u_int8 & surface_alpha, const bool & alpha_channel = false) = 0;

        /**
         *
         * Returns whether a surface is mirrored in x or not.
         *
         * @return true if the surface is mirred, false if it isn't.
         */
        bool is_mirrored_x(void) const
        {
            return is_mirrored_x_;
        }

        /**
         *
         * Returns whether a surface is mirrored in x or not.
         *
         * @return true if the surface is mirred, false if it isn't.
         */
        bool is_mirrored_y(void) const
        {
            return is_mirrored_y_;
        }

        /**
         *
         * Return the filename associated with this surface
         *
         * @return filename if known otherwise "".
         */
        const std::string &filename(void) const
        {
            return filename_;
        }

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
        void draw (const s_int16 & x, const s_int16 & y, const drawing_area * da_opt = NULL,
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
         * @param sx x coordinate of starting point 
         * @param sy y coordinate of starting point
         * @param ex x coordinate of end point
         * @param ey y coordinate of end point  
         * @param col color as returned by map_color
         * @param da_opt optional clipping rectangle
         */
        virtual void draw_line (const s_int16 sx, const s_int16 sy, const s_int16 ex, const s_int16 ey,
                                const u_int32 col, const drawing_area * da_opt = NULL);

        //@}

        /**
         * @name Special FX Methods.
         * Allows you to put fantasy in your image manipulations! Can
         * eventually even be usefull...
         *
         */
        //@{

        /**
         * Swaps the pixels to make a mirror image
         *
         * @param x  Invert x axis
         * @param y  Invert y axis
         */
        virtual void mirror (bool x, bool y) = 0;

        /**
         * Tile this %surface onto the given %surface. The location
         * tiled can be constrained by an optional drawing_area. If
         * omitted, the complete surface will be filled.
         * 
         * @param da_opt clipping rectangle.
         * @param target the target image.
         */
        void tile (const drawing_area *da_opt = NULL, surface *target = NULL) const;
        
        /**
         * Scale this image onto the target image with a given factor.
         * The target image must be large enough to hold the scaled source
         * image.
         *
         * @param target the target image.
         * @param factor the scaling factor.
         */
        virtual void scale (surface *target, const u_int32 & factor) const = 0;

        /**
         * Adjust image brightness. Values < 127 will darken
         * the image, values > 127 brighten the image.
         *
         * @param level the new brightness adjustment value.
         */
        void set_brightness (const u_int8 & level);
        //@}

        /**
         * @name Pixel manipulation Methods
         * Use these methods to directly and quickly manipulate
         * pixels from a surface.
         *
         */
        //@{


        /**
         * Maps the color triplet made of (r, g, b, alpha) into the
         * same color for the surface format.
         *
         * @param r Red componant.
         * @param g Green componant.
         * @param b Blue componant.
         * @param a Alpha component (optional, default = 255).
         *
         * @return The corresponding color for the hex triplet (r, g, b, alpha),
         *         but in the actual surface format.
         */
        virtual u_int32 map_color(const u_int8 & r, const u_int8 & g, const u_int8 & b, const u_int8 & a = 255) const = 0;

        /**
         * Unmaps the color col (in the surface format) into the hex
         * triplet of its red, green, blue and (if supported by the surface)
         * alpha componants.
         *
         * @param col The color to unmap.
         * @param r Red componant.
         * @param g Green componant.
         * @param b Blue componant.
         * @param a Alpha component.
         *
         */
        virtual void unmap_color(u_int32 col, u_int8 & r, u_int8 & g, u_int8 & b, u_int8 & a) const = 0;

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
         * Unlock the surface after you've worked on its pixels with the
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
         * Get the size of this surface
         *
         * @return the size in bytes
         */
        u_int32 size() const 
        { 
            return size_;
        }
        
        /**
         * Resets the surface to it's initial state, that is totally
         * empty.
         *
         */
        virtual void clear () = 0;

        /**
         * @name Loading/saving Methods
         * Load and save surface meta and image data in various formats.
         *
         */
        //@{
        /**
         * Save image meta data to stream. 
         * @param file stream to save meta date to.
         * @return \b true if saving successful, \b false otherwise.
         */
        bool put_state (base::flat & file) const;
        
        /**
         * Load image meta data from stream. 
         * @param file stream to load meta data from.
         * @return \b true if loading successful, \b false otherwise.
         */
        bool get_state (base::flat & file);   
        
        /** Loads an image from an opened file, in PNG format, without
         *  alpha and mask values.
         *  @param file the opened file from which to read.
         *  @return \e true in case of success, false otherwise.
         *  @sa load_png ()
         */
        virtual bool get_png (std::ifstream & file) = 0;

        /** Loads an image from a file name, in PNG format, without
         *  alpha and mask values.
         *  @param fname the name of the file to load.
         *  @return \e true in case of success, false otherwise.
         *  @sa get_png ()
         */
        bool load_png (const std::string & fname);

        /** Saves an image into an opened file, in PNG format, without
         *  alpha and mask values.
         *
         *  @param file opened file where to save into.
         *  @return \e true in case of success, false otherwise.
         *  @sa save_png ()
         */
        virtual bool put_png (std::ofstream & file) const = 0;

        /** Saves an image into an file, in PNG format, without
         *  alpha and mask values.
         *
         *  @param fname file name where to save into.
         *  @return \e true in case of success, false otherwise.
         *  @sa put_png ()
         */
        bool save_png (const std::string & fname) const;
        //@}

#ifndef SWIG
        GET_TYPE_NAME_VIRTUAL(gfx::surface)
#endif // SWIG

    protected:
        /// Mask
        bool is_masked_;

        /// Per-Surface Alpha value
        u_int8 alpha_;

        /// Whether Per-Pixel alpha is enabled
        bool alpha_channel_;
        
        /// Mirrored in x
        bool is_mirrored_x_;

        /// Mirrored in y
        bool is_mirrored_y_;

        /// Filename
        std::string filename_;

        /// Surface size in bytes
        u_int32 size_;
        
    private:
        /**
         * Forbid copy construction.
         *
         */
        surface (const surface & src);
    };
}

#endif
