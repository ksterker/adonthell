/*
   $Id: drawing_area.cc,v 1.2 2003/07/24 12:57:58 gnurou Exp $

   Copyright (C) 1999/2000/2001/2002   Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

/**
 * @file   gfx/drawing_area.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the drawing_area class.
 * 
 * 
 */

#include "drawing_area.h"

namespace gfx
{
    
    drawing_area::drawing_area ()
        : x_(0), y_(0),
          w_(0), h_(0),
          draw_to(NULL)
    {
    }
    
    drawing_area::drawing_area (s_int16 px, s_int16 py, u_int16 pw, u_int16 ph) : 
        x_(px), y_(py),
        w_(pw), h_(ph),
        draw_to(NULL)
    {
    }
    
    drawing_area drawing_area::setup_rects () const
    {
        drawing_area ret;
        if (draw_to)
        {
            drawing_area temp = draw_to->setup_rects ();
            
            ret.move(temp.x() > x ()? temp.x() : x (),
                     temp.y() > y ()? temp.y() : y ());
            
            // Precalculated for faster operation.
            s_int32 xpl = x () + length (); 
            s_int32 txw = temp.x() + temp.length();
            s_int32 txwmrx = txw - ret.x();
            s_int32 xplmrx = xpl - ret.x();  
            s_int32 yph = y () + height (); 
            s_int32 tyh = temp.y() + temp.height();
            s_int32 tyhmry = tyh - ret.y();
            s_int32 yphmry = yph - ret.y(); 
            
            
            ret.resize(txw < xpl ? txwmrx > 0 ? txwmrx : 0 : xplmrx > 0 ? xplmrx : 0,
                       tyh < yph ? tyhmry > 0 ? tyhmry : 0 : yphmry > 0 ? yphmry : 0);
            
        }
        else 
        {
            ret = *this;
            ret.assign_drawing_area(NULL);
        }
        
        return ret;
        
    }
    
    bool drawing_area::point_belong (s_int16 px, s_int16 py)
    {
        return (px >= x () && px <= x () + length () &&
                py >= y () && py <= y () + height ()); 
    }
    
}
