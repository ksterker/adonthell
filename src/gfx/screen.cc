/*
   $Id: screen.cc,v 1.2 2003/07/24 12:57:58 gnurou Exp $

   Copyright (C) 1999/2000/2001/2002/2003  Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
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
    u_int16 screen::length_, screen::height_;
    u_int8 screen::bytes_per_pixel_;
    bool screen::fullscreen_;
    
    bool (*screen::set_video_mode_p) (u_int16 nl, u_int16 nh, u_int8 depth) = NULL;
    void (*screen::update_p)() = NULL;
    u_int32 (*screen::trans_color_p)() = NULL;
    void (*screen::clear_p)() = NULL;
    
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
