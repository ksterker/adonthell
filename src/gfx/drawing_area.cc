/*
   $Id: drawing_area.cc,v 1.9 2009/02/23 14:16:01 ksterker Exp $

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
        for(const drawing_area* it = this->assigned_drawing_area(); it; it = it->assigned_drawing_area())
        {
            s_int16 x = std::max(ret.x(), it->x());
            s_int16 y = std::max(ret.y(), it->y());
            s_int32 w = std::min(it->x() + it->length(), ret.x() + ret.length()) - x;
            s_int32 h = std::min(it->y() + it->height(), ret.y() + ret.height()) - y;
            
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
    
    /*
     * rect_compare (A, B) returns X??|Y??.
     */
    #define XLL (5 << 3) /* Bx1 <  Bx2 <  Ax1               <  Ax2              */
    #define XLM (4 << 3) /*        Bx1 <= Ax1 <= Bx2        <  Ax2              */
    #define XLR (3 << 3) /*        Bx1 <= Ax1               <  Ax2 <= Bx2       */
    #define XMM (2 << 3) /*               Ax1 <  Bx1 <  Bx2 <  Ax2              */
    #define XMR (1 << 3) /*               Ax1 <  Bx1        <= Ax2 <= Bx2       */
    #define XRR (0 << 3) /*               Ax1               <  Ax2 <  Bx1 < Bx2 */
    #define YOO (5 << 0) /* By1 <  By2 <  Ay1               <  Ay2              */
    #define YOM (4 << 0) /*        By1 <= Ay1 <= By2        <  Ay2              */
    #define YOU (3 << 0) /*        By1 <= Ay1               <  Ay2 <= By2       */
    #define YMM (2 << 0) /*               Ay1 <  By1 <  By2 <  Ay2              */
    #define YMU (1 << 0) /*               Ay1 <  By1        <= Ay2 <= By2       */
    #define YUU (0 << 0) /*               Ay1               <  Ay2 <  By1 < By2 */

    int drawing_area::compare (const drawing_area & da) const
    {
        int flags;
        
        if (da.x() <= x()) 
        {
            if (da.x() + da.length() - 1 < x()) 
            {
                flags = XLL;
            } 
            else if (da.x() + da.length() - 1 < x() + length() - 1) 
            {
                flags = XLM;
            } 
            else 
            {
                flags = XLR;
            }
        } 
        else if (da.x() <= x() + length() - 1) 
        {
            if (da.x() + da.length() - 1 < x() + length() - 1) 
            {
                flags = XMM;
            } 
            else 
            {
                flags = XMR;
            }
        } 
        else 
        {
            flags = XRR;
        }
        
        if (da.y() <= y()) 
        {
            if (da.y() + da.height() - 1 < y()) 
            {
                flags |= YOO;
            } 
            else if (da.y() + da.height() - 1 < y() + height() - 1) 
            {
                flags |= YOM;
            } 
            else 
            {
                flags |= YOU;
            }
        } 
        else if (da.y() <= y() + height() - 1) 
        {
            if (da.y() + da.height() - 1 < y() + height() - 1) 
            {
                flags |= YMM;
            } 
            else 
            {
                flags |= YMU;
            }
        } 
        else 
        {
            flags |= YUU;
        }
        
        return flags;
    }
    
    void drawing_area::subtract_from (std::list<drawing_area> & parts) const
    {
        for (std::list<gfx::drawing_area>::iterator area = parts.begin(); area != parts.end(); /* nothing */)
        {
            drawing_area::subtract_area (parts, *area, *this);
            area = parts.erase (area);
        }
    }

    #define XL(da)   (da.x())
    #define XR(da)   (da.x() + da.length())
    #define YT(da)   (da.y())
    #define YB(da)   (da.y() + da.height())

    // remove area b from a
    void drawing_area::subtract_area (std::list<drawing_area> & parts, const drawing_area & a, const drawing_area & b)
    {
        int flags = a.compare (b);

        // gcc generates a nice jump table for the following switch...
        //
        // the dotted lines in the drawings indicated how `A' is split
        // into smaller rectangles.
        switch (flags)
        {
            case XMR|YMU:
            {
                // +---+
                // | A |
                // |.+-+-+
                // | | | |
                // +-+-+ |
                //   | B |
                //   +---+
                parts.push_front (drawing_area (XL (a), YT (a), a.length(), YT (b) - YT (a)));
                parts.push_front (drawing_area (XL (a), YT (b), XL (b) - XL (a), YB (a) - YT (b)));
                break;
            }
            case XMR|YOM:
            {
                //   +---+
                //   | B |
                // +-+-+ |
                // | | | |
                // |.+-+-+
                // | A |
                // +---+
                parts.push_front (drawing_area (XL (a), YT (a), XL (b) - XL (a), YB (b) - YT(a)));
                parts.push_front (drawing_area (XL (a), YB (b), a.length(), YB (a) - YB (b)));
                break;
            }
            case XLM|YOM:
            {
                // +---+
                // | B |
                // | +-+-+
                // | | | |
                // +-+-+.|
                //   | A |
                //   +---+
                parts.push_front (drawing_area (XR (b), YT (a), XR (a) - XR (b), YB (b) - YT (a)));
                parts.push_front (drawing_area (XL (a), YB (b), a.length(), YB (a) - YB (b)));
                break;
            }
            case XLM|YMU:
            {
                //   +---+
                //   | A |
                // +-+-+.|
                // | | | |
                // | +-+-+
                // | B |
                // +---+
                parts.push_front (drawing_area (XL (a), YT (a), a.length(), YT (b) - YT (a)));
                parts.push_front (drawing_area (XR (b), YT (b), XR (a) - XR (b), YB (a) - YT (b)));
                break;
            }
            case XMR|YMM:
            {
                // +---+
                // |.+-+---+
                // |A| | B |
                // |.+-+---+
                // +---+
                parts.push_front (drawing_area (XL (a), YT (a), a.length(), YT (b) - YT (a)));
                parts.push_front (drawing_area (XL (a), YT (b), XL (b) - XL (a), b.height()));
                parts.push_front (drawing_area (XL (a), YB (b), a.length(), YB (a) - YB (b)));
                break;
            }
            case XMM|YOM:
            {
                //   +---+
                //   | B |
                // +-+---+-+
                // | +---+ |
                // | : A : |
                // +-------+
                parts.push_front (drawing_area (XL (a), YT (a), XL (b) - XL (a), a.height()));
                parts.push_front (drawing_area (XL (b), YB (b), b.length(), YB (a) - YB (b)));
                parts.push_front (drawing_area (XR (b), YT (a), XR (a) - XR (b), a.height()));
                break;
            }
            case XLM|YMM:
            {
                //     +---+
                // +---+-+.|
                // | B | |A|
                // +---+-+.|
                //     +---+
                parts.push_front (drawing_area (XL (a), YT (a), a.length(), YT (b) - YT (a)));
                parts.push_front (drawing_area (XR (b), YT (b), XR (a) - XR (b), b.height()));
                parts.push_front (drawing_area (XL (a), YB (b), a.length(), YB (a) - YB (b)));
                break;
            }
            case XMM|YMU:
            {
                // +-------+
                // | : A : |
                // | +---+ |
                // +-+---+-+
                //   | B |
                //   +---+
                parts.push_front (drawing_area (XL (a), YT (a), XL (b) - XL (a), a.height()));
                parts.push_front (drawing_area (XL (b), YT (a), b.length(), YT (b) - YT (a)));
                parts.push_front (drawing_area (XR (b), YT (a), XR (a) - XR (b), a.height()));
                break;
            }
            case XMR|YOU:
            {
                //     +---+
                // +---+-+ |
                // | A | |B|
                // +---+-+ |
                //     +---+
                parts.push_front (drawing_area (XL (a), YT (a), XL (b) - XL (a), a.height()));
                break;
            }
            case XLR|YOM:
            {
                // +-------+
                // |   B   |
                // | +---+ |
                // +-+---+-+
                //   | A |
                //   +---+
                parts.push_front (drawing_area (XL (a), YB (b), a.length(), YB (a) - YB (b)));
                break;
            }
            case XLM|YOU:
            {
                // +---+
                // | +-+---+
                // |B| | A |
                // | +-+---+
                // +---+
                parts.push_front (drawing_area (XR (b), YT (a), XR (a) - XR (b), a.height()));
                break;
            }
            case XLR|YMU:
            {
                //   +---+
                //   | A |
                // +-+---+-+
                // | +---+ |
                // |   B   |
                // +-------+
                parts.push_front (drawing_area (XL (a), YT (a), a.length(), YT (b) - YT (a)));
                break;
            }
            case XLR|YMM:
            {
                //   +---+
                // +-+---+---+
                // | | A | B |
                // +-+---+---+
                //   +---+
                parts.push_front (drawing_area (XL (a), YT (a), a.length(), YT (b) - YT (a)));
                parts.push_front (drawing_area (XL (a), YB (b), a.length(), YB (a) - YB (b)));
                break;
            }
            case XMM|YOU:
            {
                //   +---+
                // +-+---+---+
                // | | B | A |
                // +-+---+---+
                //   +---+
                parts.push_front (drawing_area (XL (a), YT (a), XL (b) - XL (a), a.height()));
                parts.push_front (drawing_area (XR (b), YT (a), XR (a) - XR (b), a.height()));
                break;
            }
            case XLR|YOU:
            {
                // +---------+
                // | +---+   |
                // | | A | B |
                // | +---+   |
                // +---------+
                break;
            }
            case XMM|YMM:
            {
                // +---------+
                // |.+---+...|
                // | | B | A |
                // |.+---+...|
                // +---------+
                parts.push_front (drawing_area (XL (a), YT (a), a.length(), YT (b) - YT (a)));
                parts.push_front (drawing_area (XL (a), YT (b), XL (b) - XL (a), b.height()));
                parts.push_front (drawing_area (XR (b), YT (b), XR (a) - XR (b), b.height()));
                parts.push_front (drawing_area (XL (a), YB (b), a.length(), YB (a) - YB (b)));
                break;
            }
            default:
            {
                // no overlap
                parts.push_front (a);
                break;
            }
        }
    }
}
