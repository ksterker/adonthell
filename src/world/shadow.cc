/*
 $Id: shadow.cc,v 1.3 2009/02/01 15:18:27 ksterker Exp $
 
 Copyright (C) 2009 Kai Sterker <kai.sterker@gmail.com>
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
 * @file   world/shadow.cc
 * @author Kai Sterker <kai.sterker@gmail.com>
 * 
 * @brief  Implements the shadow class.
 * 
 */

#include "gfx/surface_cacher.h"
#include "world/shadow.h"

using gfx::drawing_area;
using world::shadow;

// ctor
shadow::shadow (const std::string & shadow, const coordinates *pos, const vector3<s_int32> & offset)
{
    Shadow = (gfx::surface*) gfx::surfaces->get_surface_only (shadow, true, false);
    Offset = offset;
    Pos = pos;
    reset();
}

// dtor
shadow::~shadow ()
{
    gfx::surfaces->free_surface (Shadow);
    Shadow = NULL;
}

void shadow::reset ()
{
    Areas.clear();
    drawing_area area (Pos->x() + Offset.x(), Pos->y() + Offset.y(), Shadow->length(), Shadow->height());
    Areas.push_back (area);
}

void shadow::draw (const vector3<s_int32> & pos, const drawing_area * da, gfx::surface * target)
{
    // set shadow opacity according to distance above ground
    u_int32 distance = Pos->z() - pos.z();
    Shadow->set_alpha (192 - (distance > 192 ? 32 : distance));
    
    // draw those parts of the shadow that haven't been rendered yet
    for (std::list<drawing_area>::iterator area = Areas.begin(); area != Areas.end(); /* nothing */)
    {
        area->move (pos.x() + area->x(), pos.y() + area->y() - pos.z());
        area->assign_drawing_area (da);
        Shadow->draw (pos.x() + Pos->x() + Offset.x(), pos.y() + Pos->y() + Offset.y() - pos.z(), &(*area), target);
        area->detach_drawing_area ();
        
        // remove piece we've just drawn from shadow
        subtract_area (*area, *da);
        area = Areas.erase (area);
    }
    
    // "normalize" shadow area
    for (std::list<drawing_area>::iterator area = Areas.begin(); area != Areas.end(); area++)
    {
        area->move (area->x() - pos.x(), area->y() - pos.y() - pos.z());
    }
}

#define XL(da)   (da.x())
#define XR(da)   (da.x() + da.length())
#define YT(da)   (da.y())
#define YB(da)   (da.y() + da.height())

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

static int rect_compare (const drawing_area & a, const drawing_area & b)
{
    int flags;
    
    if (XL (b) <= XL (a)) {
        if (XR (b) - 1 < XL (a)) {
            flags = XLL;
        } else if (XR (b) - 1 < XR (a) - 1) {
            flags = XLM;
        } else {
            flags = XLR;
        }
    } else if (XL (b) <= XR (a) - 1) {
        if (XR (b) - 1 < XR (a) - 1) {
            flags = XMM;
        } else {
            flags = XMR;
        }
    } else {
        flags = XRR;
    }
    
    if (YT (b) <= YT (a)) {
        if (YB (b) - 1 < YT (a)) {
            flags |= YOO;
        } else if (YB (b) - 1 < YB (a) - 1) {
            flags |= YOM;
        } else {
            flags |= YOU;
        }
    } else if (YT (b) <= YB (a) - 1) {
        if (YB (b) - 1 < YB (a) - 1) {
            flags |= YMM;
        } else {
            flags |= YMU;
        }
    } else {
        flags |= YUU;
    }
    return flags;
}


void shadow::subtract_area (const drawing_area & a, const drawing_area & b)
{
    int flags = rect_compare (a, b);
    
    /*
     * gcc generates a nice jump table for the following switch...
     *
     * the dotted lines in the drawings indicated how `A' is splitted
     * into smaller rectangles.
     */
    switch (flags) 
    {
        case XMR|YMU:
        {
            /*
             * +---+
             * | A |
             * |.+-+-+
             * | | | |
             * +-+-+ |
             *   | B |
             *   +---+
             */
            Areas.push_front (drawing_area (XL (a), YT (a), a.length(), YT (b) - YT (a)));
            Areas.push_front (drawing_area (XL (a), YT (b), XL (b) - XL (a), YB (a) - YT (b)));
            break;
        }
        case XMR|YOM:
        {
            /*
             *   +---+
             *   | B |
             * +-+-+ |
             * | | | |
             * |.+-+-+
             * | A |
             * +---+
             */
            Areas.push_front (drawing_area (XL (a), YT (a), XL (b) - XL (a), YB (b) - YT(a)));
            Areas.push_front (drawing_area (XL (a), YB (b), a.length(), YB (a) - YB (b)));
            break;
        }
        case XLM|YOM:
        {
            /*
             * +---+
             * | B |
             * | +-+-+
             * | | | |
             * +-+-+.|
             *   | A |
             *   +---+
             */
            Areas.push_front (drawing_area (XR (b), YT (a), XR (a) - XR (b), YB (b) - YT (a)));
            Areas.push_front (drawing_area (XL (a), YB (b), a.length(), YB (a) - YB (b)));
            break;
        }
        case XLM|YMU:
        {
            /*
             *   +---+
             *   | A |
             * +-+-+.|
             * | | | |
             * | +-+-+
             * | B |
             * +---+
             */
            Areas.push_front (drawing_area (XL (a), YT (a), a.length(), YT (b) - YT (a)));
            Areas.push_front (drawing_area (XR (b), YT (b), XR (a) - XR (b), YB (a) - YT (b)));
            break;
        }
        case XMR|YMM:
        {
            /*
             * +---+
             * |.+-+---+
             * |A| | B |
             * |.+-+---+
             * +---+
             */
            Areas.push_front (drawing_area (XL (a), YT (a), a.length(), YT (b) - YT (a)));
            Areas.push_front (drawing_area (XL (a), YT (b), XL (b) - XL (a), b.height()));
            Areas.push_front (drawing_area (XL (a), YB (b), a.length(), YB (a) - YB (b)));
            break;
        }
        case XMM|YOM:
        {
            /*
             *   +---+
             *   | B |
             * +-+---+-+
             * | +---+ |
             * | : A : |
             * +-------+
             */
            Areas.push_front (drawing_area (XL (a), YT (a), XL (b) - XL (a), a.height()));
            Areas.push_front (drawing_area (XL (b), YB (b), b.length(), YB (a) - YB (b)));
            Areas.push_front (drawing_area (XR (b), YT (a), XR (a) - XR (b), a.height()));
            break;
        }
        case XLM|YMM:
        {
            /*
             *     +---+
             * +---+-+.|
             * | B | |A|
             * +---+-+.|
             *     +---+
             */
            Areas.push_front (drawing_area (XL (a), YT (a), a.length(), YT (b) - YT (a)));
            Areas.push_front (drawing_area (XR (b), YT (b), XR (a) - XR (b), b.height()));
            Areas.push_front (drawing_area (XL (a), YB (b), a.length(), YB (a) - YB (b)));
            break;
        }
        case XMM|YMU:
        {
            /*
             * +-------+
             * | : A : |
             * | +---+ |
             * +-+---+-+
             *   | B |
             *   +---+
             */
            Areas.push_front (drawing_area (XL (a), YT (a), XL (b) - XL (a), a.height()));
            Areas.push_front (drawing_area (XL (b), YT (a), b.length(), YT (b) - YT (a)));
            Areas.push_front (drawing_area (XR (b), YT (a), XR (a) - XR (b), a.height()));
            break;
        }
        case XMR|YOU:
        {
            /*
             *     +---+
             * +---+-+ |
             * | A | |B|
             * +---+-+ |
             *     +---+
             */
            Areas.push_front (drawing_area (XL (a), YT (a), XL (b) - XL (a), a.height()));        
            break;
        }   
        case XLR|YOM:
        {
            /*
             * +-------+
             * |   B   |
             * | +---+ |
             * +-+---+-+
             *   | A |
             *   +---+
             */
            Areas.push_front (drawing_area (XL (a), YB (b), a.length(), YB (a) - YB (b)));
            break;
        }
        case XLM|YOU:
        {
            /*
             * +---+
             * | +-+---+
             * |B| | A |
             * | +-+---+
             * +---+
             */
            Areas.push_front (drawing_area (XR (b), YT (a), XR (a) - XR (b), a.height()));
            break;
        }
        case XLR|YMU:
        {
            /*
             *   +---+
             *   | A |
             * +-+---+-+
             * | +---+ |
             * |   B   |
             * +-------+
             */
            Areas.push_front (drawing_area (XL (a), YT (a), a.length(), YT (b) - YT (a)));
            break;
        }
        case XLR|YMM:
        {
            /*
             *   +---+
             * +-+---+---+
             * | | A | B |
             * +-+---+---+
             *   +---+
             */
            Areas.push_front (drawing_area (XL (a), YT (a), a.length(), YT (b) - YT (a)));
            Areas.push_front (drawing_area (XL (a), YB (b), a.length(), YB (a) - YB (b)));
            break;
        }   
        case XMM|YOU:
        {
            /*
             *   +---+
             * +-+---+---+
             * | | B | A |
             * +-+---+---+
             *   +---+
             */
            Areas.push_front (drawing_area (XL (a), YT (a), XL (b) - XL (a), a.height()));
            Areas.push_front (drawing_area (XR (b), YT (a), XR (a) - XR (b), a.height()));
            break;
        }   
        case XLR|YOU:
        {
            /*
             * +---------+
             * | +---+   |
             * | | A | B |
             * | +---+   |
             * +---------+
             */
            break;
        }   
        case XMM|YMM:
        {
            /*
             * +---------+
             * |.+---+...|
             * | | B | A |
             * |.+---+...|
             * +---------+
             */
            Areas.push_front (drawing_area (XL (a), YT (a), a.length(), YT (b) - YT (a)));
            Areas.push_front (drawing_area (XL (a), YT (b), XL (b) - XL (a), b.height()));
            Areas.push_front (drawing_area (XR (b), YT (b), XR (a) - XR (b), b.height()));
            Areas.push_front (drawing_area (XL (a), YB (b), a.length(), YB (a) - YB (b)));
            break;
        }
        default:
        {
            // no overlap
            Areas.push_front (a);
            break;
        }
    }
}
