/*
 Copyright (C) 1999/2000/2001/2002/2003 Alexandre Courbot <alexandrecourbot@linuxgames.com>
 Copyright (C) 2006 Tyler Nielsen
 
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
 * @file gfx/surface_ext.cc
 *
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * @author Tyler Nielsen
 *
 * @brief  Implements backend independent surface extensions.
 */

#include <cstdio>
#include <adonthell/base/logging.h>
#include "surface_ext.h"
#include "png_wrapper.h"

using gfx::surface_ext;

// This should probably be somewhere else... like a utils in base or something...
template <class type>
void reverseArray (type *orig, size_t len)
{
    size_t start=0;
    type swap;
    
    for(;start<--len;start++) //increment a and decrement b until they meet eachother
    {
        swap=orig[start];       //put what's in a into swap space
        orig[start]=orig[len];    //put what's in b into a
        orig[len]=swap;       //put what's in the swap (a) into b
    }
}

void surface_ext::mirror (bool x, bool y)
{
    if (x || y)
    {
        u_int32 bpp = alpha_channel_ ? 4 : 3;
        u_int32 alpha_mask = alpha_channel_ ? A_MASK : 0;
        u_int8 *rawdata = (u_int8 *)get_data(bpp, R_MASK, G_MASK, B_MASK, alpha_mask);
        u_int32 pitch = length() * bpp;

        if (x)
        {
            for (int idx = 0; idx < height(); idx++)
            {
                reverseArray (&rawdata[idx*pitch], pitch);
            }

            is_mirrored_x_ = !is_mirrored_x_;
        }
        
        if (y)
        {
            int half_height = height() / 2;
            u_int8* tmp = (u_int8*) malloc (pitch);

            for (int i = 0; i < half_height; i++)
            {
                memcpy (tmp, rawdata + pitch * i, pitch);
                memcpy (rawdata + pitch * i, rawdata + pitch * (height() - i - 1), pitch);
                memcpy (rawdata + pitch * (height() - i - 1), tmp, pitch);
            }

            free (tmp);
            is_mirrored_y_ = !is_mirrored_y_;
        }

        if (x)
        {
            if (!alpha_channel_)
            {
                // This is swapped (BGR) because we swapped at a byte level, not at a pixel level
                set_data(rawdata, length(), height(), 3, B_MASK, G_MASK, R_MASK, 0);
            }
            else
            {
                // This is swapped (BGRA) because we swapped at a byte level, not at a pixel level
#ifdef __BIG_ENDIAN__
                set_data(rawdata, length(), height(), 4, G_MASK, R_MASK, A_MASK, B_MASK);
#else
                set_data(rawdata, length(), height(), 4, A_MASK, B_MASK, G_MASK, R_MASK);
#endif
            }
        }
        else
        {
            // swapping in y only does not change the format
            set_data (rawdata, length(), height(), bpp, R_MASK, G_MASK, B_MASK, alpha_mask);
        }
    }
}

// load png into image
bool surface_ext::get_png (std::ifstream & file)
{
    void *rawdata;
    u_int16 l, h; 
    bool alpha = false;
    
    rawdata = png::get (file, l, h, &alpha);
    
    if (!rawdata) return false;
    
    clear ();
    
    set_data(rawdata, l, h, alpha ? 4 : 3,
             R_MASK, G_MASK, B_MASK, alpha ? A_MASK : 0);
    
    return true;
}


// save image data as png
bool surface_ext::put_png (std::ofstream & file) const
{
    void * rawdata = get_data(alpha_channel_ ? 4 : 3, R_MASK, G_MASK, B_MASK, alpha_channel_ ? A_MASK : 0);
    
    if (!rawdata) return false;
    
    png::put (file, (const char *)rawdata, length (), height (), alpha_channel_);
    
    free(rawdata);
    
    return true;
}
