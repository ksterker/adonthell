/*
   $Id: screen.cc,v 1.4 2003/11/22 09:35:21 ksterker Exp $

   Copyright (C) 1999/2000/2001/2002/2003  Alexandre Courbot <alexandrecourbot@linuxgames.com>
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
 * @file   gfx/screen.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the screen class.
 * 
 * 
 */


#include "gfx/screen.h"

namespace gfx
{
    // All the static data for the screen.
    u_int16 screen::length_, screen::height_;
    u_int8 screen::bytes_per_pixel_;
    bool screen::fullscreen_;
    
    bool (*screen::set_video_mode_p) (u_int16 nl, u_int16 nh, u_int8 depth) = NULL;
    void (*screen::update_p)() = NULL;
    u_int32 (*screen::trans_color_p)() = NULL;
    void (*screen::clear_p)() = NULL;
    surface * (*screen::get_surface_p)() = NULL;
    
    bool screen::set_video_mode(u_int16 nl, u_int16 nh, u_int8 depth)
    {
        bool res = set_video_mode_p(nl, nh, depth); 
        
        if (res)
        {
            length_ = nl;
            height_ = nh;
            bytes_per_pixel_ = depth;
        }
        
        return res;
    }    
}
