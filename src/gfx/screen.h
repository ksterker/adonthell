/*
   $Id: screen.h,v 1.3 2003/11/22 09:35:21 ksterker Exp $

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
 * @file   gfx/screen.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the screen class.
 * 
 * 
 */
 
#ifndef SCREEN_H_
#define SCREEN_H_

#include "base/types.h"

#include <string>

#include "gfx/surface.h"

namespace gfx
{

    /** Screen access is made through this class.
     *  This static class sets video modes, flush the frame buffer to the physical
     *  screen and make abstraction of the real screen depth to ease the 
     *  programmer's task.
     */
    class screen
    {        
    public:
        /** Sets the video mode.
         *  @param nl X screen resolution.
         *  @param nh Y screen resolution.
         *  @param depth desired screen depth.
         */ 
        static bool set_video_mode(u_int16 nl, u_int16 nh, u_int8 depth = 0);
        
        /**
         * Returns the length of the screen.
         * @return length of the screen.
         */  
        static u_int16 length () 
        {
            return length_; 
        }

        /** 
         * Returns the height of the screen.
         * @return height of the screen.
         */ 
        static u_int16 height () 
        {
            return height_;
        }

        /** 
         * Returns the screen depth, in bytes per pixel.
         * @return screen depth, in bytes per pixel.
         */ 
        static u_int8 bytes_per_pixel () 
        {
            return bytes_per_pixel_; 
        }
            
        /** 
         * Ensures the framebuffer is copied to the physical screen.
         *
         */ 
        static void update () { update_p(); }

        /** 
         * Returns the display's transparent color. (i.e. the color
         * that won't be displayed for surfaces drawn when 
         * surface::set_mask() is set to \e true.
         * 
         * 
         * @return The display's transparent color, in the display format.
         */
        static u_int32 trans_color() { return trans_color_p(); }

        static surface * get_surface() { return get_surface_p(); }

        /** Returns whether the current mode is fullscreen or windowed.
         *  @return
         *     - true: fullscreen.
         *     - false: windowed.
         */ 
        static bool is_fullscreen ()
        {
            return fullscreen_; 
        }

        /** 
         * Totally clears the screen with black.
         * 
         */
        static void clear() { clear_p(); }
        
        /** Sets fullscreen/windowed mode.
         *  @param mode
         *     - true: fullscreen mode.
         *     - false: windowed mode.
         *  @return
         *    @li true if the operation succeed.
         *    @li false if the mode is already set, or the system doesn't support
         *        this mode.
         */ 
        //         static bool set_fullscreen (bool m); 
        
        /** 
         * Returns information about the current screen settings,
         * suitable for being displayed to the user.
         * 
         * 
         * @return printable information about the current screen settings.
         */
        //         static std::string info (); 
        
        /**
         * @name Transparent color settings
         * 
         */
        ///@{
        
        /// Red componant
        static const u_int8 TRANS_RED = 0xff;
        /// Green componant
        static const u_int8 TRANS_GREEN = 0x00;        
        /// Blue componant
        static const u_int8 TRANS_BLUE = 0xff;
        
        ///@}
        
    protected:
        static bool fullscreen_; 
        
        static u_int16 length_;
        static u_int16 height_;
        static u_int8 bytes_per_pixel_; 
        
        static bool (*set_video_mode_p) (u_int16 nl, u_int16 nh, u_int8 depth = 0);
        static void (*update_p)();
        static u_int32 (*trans_color_p)();
        static void (*clear_p)();
        static surface * (*get_surface_p)();

        friend bool gfx::init(const std::string &);
    };
}

#endif
