/*
   $Id: surface.cc,v 1.3 2003/11/22 09:35:21 ksterker Exp $

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
 * @file gfx/surface.cc
 *
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the surface global interface.
 */ 

#include "surface.h"
#include "gfx/pnm.h"

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
    
    bool surface::get_pnm (ifstream & file)
    {
        void *rawdata;
        u_int16 l, h; 
        
        rawdata = pnm::get (file, l, h);
        
        if (!rawdata) return false;
        
        clear (); 
        /**
         * @bug We don't take care of endianness here! Colors
         * won't be rendered correctly on big-endian machines.
         */
        set_data(rawdata, l, h, 3, 0x0000ff, 0x00ff00, 0xff0000);
        
        free (rawdata);
        
        return true;
    }
    
    bool surface::load_pnm (const string & fname)
    {
        ifstream file(fname.c_str());
        bool ret = true;
        
        if (!file.is_open())
            return false;
        ret = get_pnm (file);
        file.close();
        return ret;
    }
    
    bool surface::put_pnm (ofstream & file) const
    {
        /**
         * @bug We don't take care of endianness here!
         * 
         */
        void * rawdata = get_data(3, 0x0000ff, 0x00ff00,
                                  0xff0000);
        
        if (!rawdata) return false;
        
        pnm::put (file, (const char *)rawdata, length (), height ()); 
        
        free(rawdata);
        
        return true;
    }
    
    bool surface::save_pnm (const string & fname) const
    {
        ofstream file(fname.c_str());
        bool ret = true;
        
        if (!file.is_open())
            return false;
        ret = put_pnm (file);
        file.close();
        return true;
    }   
}
