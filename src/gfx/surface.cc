/*
   $Id: surface.cc,v 1.16 2008/02/16 19:08:43 ksterker Exp $

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

#include "base/base.h"
#include "gfx/surface.h"
#include "gfx/png_wrapper.h"


using namespace std;

namespace gfx
{
    // ctor
    surface::surface()
        : is_masked_(false), alpha_(255), alpha_channel_(false), is_mirrored_x_(false), is_mirrored_y_(false)
    {
    }

    // dtor
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
            //This is swapped (BGR) because we swapped at a byte level, not at a pixel level
            set_data(rawdata, length(), height(), 3, B_MASK, G_MASK, R_MASK);
            is_mirrored_x_ = !is_mirrored_x_;
        }

        if (y)
        {
            cout << "Mirroring in y is not supported yet." << endl;
            is_mirrored_y_ = !is_mirrored_y_;
        }
    }

    void surface::draw_line(const s_int16 x1, const s_int16 y1, const s_int16 x2, const s_int16 y2, 
                            const u_int32 color, const drawing_area * da_opt)
    {
        int i;
        int x,y;
        int IncX,IncY;
        int Dx,Dy;
        int Err;
        int inc1,inc2;
        int offset;

        drawing_area da;

        if (da_opt) da = da_opt->setup_rects();

        x=x1;
        y=y1;

        if(x1<=x2)
        {
            IncX=1;
            Dx=x2-x1;
        }
        else
        {
            IncX=-1;
            Dx=x1-x2;
        }

        if(y1<=y2)
        {
            IncY=1;
            Dy=y2-y1;
        }
        else
        {
            IncY=-1;
            Dy=y1-y2;
        }

        // put_pix() requires locking of surface
        lock();
        
        if(Dy<Dx)
        {
            inc1=(Dy-Dx)<<1;
            inc2=Dy<<1;
            Err=inc2-Dx;

            for(i=0;i<Dx;i++)
            {
                if (!da_opt || (x >= da.x() && x < da.x() + da.length() &&
                                y >= da.y() && y < da.y() + da.height()))
                    put_pix (x, y, color);

                if(Err>0)
                {
                    y+=IncY;
                    Err+=inc1;
                }
                else
                    Err+=inc2;

                x+=IncX;
                offset+=IncX;
            }
        }
        else
        {
            inc1=(Dx-Dy)<<1;
            inc2=Dx<<1;
            Err=inc2-Dy;

            for(i=0;i<Dy;i++)
            {
                if (!da_opt || (x >= da.x() && x < da.x() + da.length() &&
                                y >= da.y() && y < da.y() + da.height()))
                    put_pix(x, y, color);

                if(Err>0)
                {
                    x+=IncX;
                    Err+=inc1;
                }
                else
                    Err+=inc2;

                y+=IncY;
            }
        }
        
        unlock();
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
        // find file in adonthell's search path
        std::string path = fname;
        if (!base::Paths.find_in_path (path))
        {
            fprintf (stderr, "*** surface::load_png: unable to open '%s'!\n", fname.c_str());
            return false; 
        }
        
        // try to open file
        ifstream file (path.c_str(), ifstream::binary);
        if (!file.is_open()) 
        {
            cout << "*** surface::load_png: unable to open '" << path << "'" << endl;
            return false;
        }
        
        bool ret = get_png (file);
        file.close ();
        filename_ = path;
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
        ofstream file(fname.c_str(), ios::binary);
        bool ret = true;

        if (!file.is_open())
            return false;
        ret = put_png (file);
        file.close();
        return true;
    }
}
