/*
   $Id: drawing_area.cc,v 1.3 2003/07/25 11:26:57 gnurou Exp $

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
#include <algorithm>

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
        drawing_area ret = *this;
        ret.assign_drawing_area(NULL);
        for(drawing_area* it = this->assigned_drawing_area(); it; it = it->assigned_drawing_area())
	{
            s_int32 x = std::max(ret.x(), it->x());
            s_int32 y = std::max(ret.y(), it->y());
            s_int32 w = std::min(it->x() + it->length(), ret.x() + ret.length()) - x;
            s_int32 h = std::min(it->y() + it->length(), ret.y() + ret.length()) - y;
            
            ret.move(x, y);
            ret.resize(w > 0 ? w : 0, 
                       h > 0 ? h : 0);
        }
        return ret;
    }
    
    bool drawing_area::point_belong (s_int16 px, s_int16 py)
    {
        return (px >= x () && px <= x () + length () &&
                py >= y () && py <= y () + height ()); 
    }
}
