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
 * @file gfx/surface.cc
 *
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 *
 * @brief  Defines the surface global interface.
 */

#include <cstdio>
#include <stdlib.h>
#include "base/base.h"
#include "base/logging.h"
#include "gfx/screen.h"

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

    void surface::draw_line(const s_int16 x1, const s_int16 y1, const s_int16 x2, const s_int16 y2, 
                            const u_int32 color, const drawing_area * da_opt)
    {
        int i;
        int x,y;
        int IncX,IncY;
        int Dx,Dy;
        int Err;
        int inc1,inc2;

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

    // tile this surface onto the given one
    void surface::tile (const drawing_area *da_opt, surface *target) const
    {
        drawing_area da;
        
        if (!target) target = gfx::screen::get_surface();

        if (da_opt) da = da_opt->setup_rects();
        else da.resize (target->length(), target->height());
        
        for (u_int16 posy = 0; posy < target->height(); posy += height ())
            for (u_int16 posx = 0; posx < target->length(); posx += length ())
                draw (da.x() + posx, da.y() + posy, &da, target);
    }

    // adjust brightness
    void surface::set_brightness (const u_int8 & level)
    {
        const s_int32 trans_col = screen::trans_color ();
        const s_int8 mod = level - 127;
        u_int8 ir, ig, ib, ia;
        
        lock();
        
        if (mod > 0)
        {
            for (int l = 0; l < length(); l++)
            {
                for (int h = 0; h < height(); h++)
                {
                    u_int32 pix = get_pix (l, h);
                    if (!is_masked() || pix != trans_col)
                    {
                        unmap_color (pix, ir, ig, ib, ia);
                        if (ia != 0)
                        {
                            ir = (ir + mod) > 255 ? 255 : ir + mod;
                            ig = (ig + mod) > 255 ? 255 : ig + mod;
                            ib = (ib + mod) > 255 ? 255 : ib + mod;
                            put_pix (l, h, map_color (ir, ig, ib, ia));
                        }
                    }
                }
            }
        }
        else
        {
            for (int l = 0; l < length(); l++)
            {
                for (int h = 0; h < height(); h++)
                {
                    u_int32 pix = get_pix (l, h);
                    if (!is_masked() || pix != trans_col)
                    {
                        unmap_color (pix, ir, ig, ib, ia);
                        if (ia != 0)
                        {
                            ir = (ir + mod) < 0 ? 0 : ir + mod;
                            ig = (ig + mod) < 0 ? 0 : ig + mod;
                            ib = (ib + mod) < 0 ? 0 : ib + mod;
                            put_pix (l, h, map_color (ir, ig, ib, ia));
                        }
                    }
                }
            }
        }
        
        unlock();        
    }
    
    // save meta data to stream
    bool surface::put_state (base::flat & file) const
    {
        file.put_bool ("mask", is_masked_);
        file.put_uint8 ("alpha", alpha_);
        file.put_bool ("mirror_x", is_mirrored_x_);
        file.put_bool ("mirror_y", is_mirrored_y_);
        
        return true;
    }
    
    // load meta data from stream
    bool surface::get_state (base::flat & file)
    {
        is_masked_ = file.get_bool ("mask");
        alpha_ = file.get_uint8 ("alpha");
        is_mirrored_x_ = file.get_bool ("mirror_x");
        is_mirrored_y_ = file.get_bool ("mirror_y");
        
        return file.success();
    }

    // load png into image from file
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
        std::ifstream file (path.c_str(), std::ifstream::binary);
        if (!file.is_open()) 
        {
            LOG(ERROR) << logging::indent()
                       << "*** surface::load_png: unable to open '" << path << "'";
            return false;
        }
        
        bool ret = get_png (file);
        file.close ();
        filename_ = path;
        return ret;
    }

    // save image data as png in given file
    bool surface::save_png (const string & fname) const
    {
        std::ofstream file(fname.c_str(), std::ios::binary);
        bool ret = true;

        if (!file.is_open())
            return false;
        
        ret = put_png (file);
        file.close();
        return true;
    }
}
