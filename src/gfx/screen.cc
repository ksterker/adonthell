/*
   $Id: screen.cc,v 1.7 2009/04/25 13:17:50 ksterker Exp $

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


#include <cstdio>
#include "gfx/gfx.h"
#include "gfx/screen.h"
#include "base/logging.h"

namespace gfx
{
    /// Red componant
    const u_int8 screen::TRANS_RED = 0xff;
    /// Green componant
    const u_int8 screen::TRANS_GREEN = 0x00;
    /// Blue componant
    const u_int8 screen::TRANS_BLUE = 0xff;    
    
    // All the static data for the screen.
    u_int16 screen::length_ = 0, screen::height_ = 0;
    u_int8 screen::bytes_per_pixel_;
    bool screen::fullscreen_;
    surface *screen::ShadowSurface;
    
    void (*screen::get_video_mode_p) (u_int16 *l, u_int16 *h, u_int8 *depth) = NULL;
    bool (*screen::set_video_mode_p) (u_int16 nl, u_int16 nh, u_int8 depth) = NULL;
    void (*screen::update_p)() = NULL;
    u_int32 (*screen::trans_color_p)() = NULL;
    void (*screen::clear_p)() = NULL;
    surface * (*screen::get_surface_p)() = NULL;
    std::string (*screen::info_p)() = NULL;
    
    void screen::setup (base::configuration & cfg)
    {
        base::cfg_option *opt;

        // option to toggle fullscreen on/off
        screen::set_fullscreen (cfg.get_int ("Video", "Fullscreen", 1) == 1);
        cfg.option ("Video", "Fullscreen", base::cfg_option::BOOL);

        opt = cfg.option ("Video", "Scale", base::cfg_option::UNDEF);
        if (opt != NULL)
        {
        	base::Scale = cfg.get_int ("Video", "Scale", 1);
        }

        opt = cfg.option ("Video", "Width", base::cfg_option::UNDEF);
        if (opt != NULL)
        {
        	length_ = cfg.get_int ("Video", "Width", 0);
        }

        opt = cfg.option ("Video", "Height", base::cfg_option::UNDEF);
        if (opt != NULL)
        {
        	height_ = cfg.get_int ("Video", "Height", 0);
        }
    }

    bool screen::set_native_mode (const u_int16 & min_x, const u_int16 & max_x, const u_int16 & min_y, const u_int16 & max_y)
    {
    	u_int16 length, height;

    	if (length_ != 0 && height_ != 0)
    	{
        	// configuration override
    		length = length_ * base::Scale;
    		height = height_ * base::Scale;
    	}
    	else if (is_fullscreen())
    	{
    		// scale to desktop size
    		get_video_mode_p (&length, &height, &bytes_per_pixel_);
    		for (int i = 1; true; i++)
    		{
    			length_ = length/i;
    			height_ = height/i;

    			if (length_ < min_x && height_ < min_y)
    			{
    				LOG(FATAL) << "*** error: Failed setting a valid video mode. Please configure your own!";
    			}

    			if (length_ <= max_x && height_ <= max_y)
    			{
    				base::Scale = i;
    				break;
    			}
    		}
    	}
    	else
    	{
    		// use maximum allowed view size for windowed mode
    		length_ = max_x;
    		height_ = max_y;

    		length = length_ * base::Scale;
    		height = height_ * base::Scale;
    	}

    	if (!set_video_mode_p (length, height, bytes_per_pixel_*8))
    	{
			LOG(FATAL) << "*** error: Failed setting video mode to " << length << " x " << height << " @ " << (int) bytes_per_pixel_*8 << " bpp!";
    	}

		if (base::Scale > 1)
		{
			ShadowSurface = gfx::create_surface();
			ShadowSurface->set_alpha(255, 0);
			ShadowSurface->resize (length_, height_);
		}

    	LOG(ERROR) << "*** info: Set internal view to " << length_ << " x " << height_ << ".";
    	LOG(ERROR) << "*** info: Set display size to " << length << " x " << height << ".";
    	return true;
    }

    bool screen::set_video_mode(u_int16 nl, u_int16 nh, u_int8 depth)
    {
        bool res = set_video_mode_p(nl, nh, depth); 
        
        if (res)
        {
            length_ = nl;
            height_ = nh;
            bytes_per_pixel_ = depth;
        }

        LOG(INFO) << info();
        
        return res;
    }    
}
