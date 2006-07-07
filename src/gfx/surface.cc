/*
   $Id: surface.cc,v 1.9 2006/07/07 17:34:46 Mithander Exp $

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
 * @file gfx/surface.cc
 *
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 *
 * @brief  Defines the surface global interface.
 */

#include "gfx/surface.h"
#include "gfx/png_wrapper.h"

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

using namespace std;

namespace gfx
{
    surface::surface()
        : is_masked_(false), alpha_(255)
    {
    }

    surface::~surface()
    {
    }

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

    void surface::mirror (bool x, bool y)
    {
        if (x)
        {
            u_int8 *rawdata = (u_int8 *)get_data(3, R_MASK, G_MASK, B_MASK);
            for(int idx = 0; idx < height(); idx++)
                reverseArray(&rawdata[idx*length()*3], length()*3);
            //This is swaped (BGR) because we swaped at a byte level, not at a pixel level
            set_data(rawdata, length(), height(), 3, B_MASK, G_MASK, R_MASK);
        }

        if (y)
        {
            cout << "Mirroring in y is not supported yet." << endl;
        }
    }

    bool surface::get_png (ifstream & file)
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

    bool surface::load_png (const string & fname)
    {
        ifstream file(fname.c_str());
        bool ret = true;

        if (!file.is_open()) {
            cout << "Unable to open: '" << fname << "'" << endl;
            return false;
        }
        ret = get_png (file);
        file.close();
        return ret;
    }

    bool surface::put_png (ofstream & file) const
    {
        void * rawdata = get_data(3, R_MASK, G_MASK, B_MASK);

        if (!rawdata) return false;

        png::put (file, (const char *)rawdata, length (), height (), false);

        free(rawdata);

        return true;
    }

    bool surface::save_png (const string & fname) const
    {
        ofstream file(fname.c_str());
        bool ret = true;

        if (!file.is_open())
            return false;
        ret = put_png (file);
        file.close();
        return true;
    }
}
