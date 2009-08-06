/*
 $Id: surface_ext.h,v 1.1 2009/03/06 22:53:29 ksterker Exp $
 
 Copyright (C) 1999/2000/2001/2002/2003 Alexandre Courbot <alexandrecourbot@linuxgames.com>
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
 * @file   gfx/surface_ext.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 *
 * @brief  Backend independent surface manipulation.
 *
 *
 */


#ifndef SURFACE_EXT_H
#define SURFACE_EXT_H

#include "gfx/surface.h"

namespace gfx
{
/**
 * This class extends surface with a number of methods that are independent
 * from the used backend. However, some backends might implement those functions
 * more efficiently, in which case they should directly extend the surface class
 * instead.
 */
class surface_ext : public surface
{
public:
    /**
     * Constructor.
     */
    surface_ext () : surface () {}
    
    /**
     * Destructor.
     */
    virtual ~surface_ext() {}
    
    /**
     * Swaps the pixels to make a mirror image
     *
     * \bug Mirror does not yet work on surfaces with alpha channel! 
     *      I.e. alpha channel information is lost on mirroring.
     *
     * \bug Mirroring in y direction not implemented yet.
     *
     * @param x  Invert x axis
     * @param y  Invert y axis
     */
    virtual void mirror (bool x, bool y);
    
    /**
     * @name Loading/saving Methods
     * Load and save surface meta and image data in various formats.
     *
     */
    //@{    
    /** Loads an image from an opened file, in PNG format, without
     *  alpha and mask values.
     *  @param file the opened file from which to read.
     *  @return \e true in case of success, false otherwise.
     *  @sa load_png ()
     */
    virtual bool get_png (std::ifstream & file);
    
    /** Saves an image into an opened file, in PNG format, without
     *  alpha and mask values.
     *
     *  @param file opened file where to save into.
     *  @return \e true in case of success, false otherwise.
     *  @sa save_png ()
     */
    virtual bool put_png (std::ofstream & file) const;
    //@}

#ifndef SWIG
    GET_TYPE_NAME_VIRTUAL(gfx::surface_ext)
#endif // SWIG
    
protected:
    /**
     * Sets the surface pixel data from a given memory zone.
     * \e data must point to a memory area of size
     * l * h * bytes_per_pixel, containing the pixel data in
     * the true color format, with the following masks:
     * - Red: red_mask
     * - Green: green_mask
     * - Blue: blue_mask
     * - Alpha: alpha_mask option (0 => none)
     * 
     * @param data Pointer to the new pixel data.
     * @param l length of the image (in pixels)
     * @param h height of the image (in pixels)
     * @param bytes_per_pixel Number of data bytes per pixel.
     * @param red_mask Red color mask.
     * @param green_mask Green color mask.
     * @param blue_mask Blue color mask.
     * @param alpha_mask Alpha channel mask, or zero if disabled (default).
     *
     * set_data takes responsibility of memory management for 'data', which is
     * expected to have been allocated with malloc() and/or calloc().
     */
    virtual void set_data (void * data, u_int16 l, u_int16 h,
                           u_int8 bytes_per_pixel = BYTES_PER_PIXEL,
                           u_int32 red_mask = R_MASK, u_int32 green_mask = G_MASK,
                           u_int32 blue_mask = B_MASK, u_int32 alpha_mask = 0) = 0;
    
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
     * @param alpha_mask Alpha color mask.
     * @return Image data converted to the requested format.
     */
    virtual void * get_data (u_int8 bytes_per_pixel,
                             u_int32 red_mask, u_int32 green_mask,
                             u_int32 blue_mask, u_int32 alpha_mask) const = 0;
        
};
    
}

#endif
