/*
   $Id: drawing_area.cc,v 1.5 2003/11/22 09:35:21 ksterker Exp $

   Copyright (C) 1999/2000/2001/2002   Alexandre Courbot <alexandrecourbot@linuxgames.com>
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
