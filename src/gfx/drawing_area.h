/*
   $Id: drawing_area.h,v 1.9 2009/02/07 21:47:09 ksterker Exp $

   Copyright (C) 1999/2000/2001/2002 Alexandre Courbot <alexandrecourbot@linuxgames.com>
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
 * @file   gfx/drawing_area.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the drawing_area class.
 * 
 * 
 */

#ifndef DRAWING_AREA_H_
#define DRAWING_AREA_H_

#include <iostream>
#include "base/types.h"

#include "python/callback_support.h"

namespace gfx
{

   /**
     * Implements "drawing zones" for drawing operations.
     * An object which is drawn into a drawing_area will only appear in
     * the rectangular zone it covers.
     * During some drawing operations, you may want to limit the blits to a limited
     * area of the %screen. For example, if you want to draw an image into a
     * window, and that image is larger than this window, you don't want the
     * entire image to appear, only the part that fits into the window. This is
     * exactly what drawing areas are for. A drawing area is a square that can be
     * any size and located anywhere on the %screen. If you assign a drawing area
     * to a drawable object (for example, an image), and then draw the image on
     * the %screen, the part of the image that doesn't fit the drawing area limits
     * isn't displayed. A drawing area can be assigned to any drawable, but also
     * to another drawing area, in which case the result of blitting operations
     * from objects that are assigned to the second drawing area will be limited
     * to the intersection of the two drawing areas. Recursively, you can use as many
     * drawing_areas as you wish for drawing operations.
     */ 
    class drawing_area
    { 
    public:
        /** Default constructor.
         *  The drawing_area is then located at (0, 0) and is (0, 0) sized.
         */  
        drawing_area ();
 
#ifndef SWIG
        /** Builds a drawing_area from the parameters.
         *  @param px X position.
         *  @param py Y position.
         *  @param pw Length.
         *  @param ph Height.
         *
         *  @note Not available from Python.
         */  
        drawing_area (s_int16 px, s_int16 py, u_int16 pw, u_int16 ph); 
#endif
        
        /**
         * Destructor.
         */
        virtual ~drawing_area () { }
        
        /** Returns the horizontal position of the drawing_area.
         *  @return horizontal position of the drawing_area.
         */  
        s_int16 x () const
        {
            return x_; 
        }

        /** Returns the vertical position of the drawing_area.
         *  @return vertical position of the drawing_area.
         */  
        s_int16 y () const
        {
            return y_; 
        }

        /** Returns the length of the drawing_area.
         *  @return length of the drawing_area.
         */  
        u_int16 length () const
        {
            return w_; 
        }

        /** Returns the height of the drawing_area.
         *  @return height of the drawing_area.
         */  
        u_int16 height () const
        {
            return h_; 
        }

        /** Relocate the drawing_area.
         *  @param nx new horizontal position.
         *  @param ny new vertical position.
         */  
        void move (s_int16 nx, s_int16 ny) 
        {
            x_ = nx;
            y_ = ny;
        }

        /** Resize the drawing_area.
         *  @param nl new length.
         *  @param nh new height.
         */ 
        void resize (u_int16 nl, u_int16 nh) 
        {
            w_ = nl;
            h_ = nh; 
        }

        /**
         * Expand the drawing area by the
         * values specified in the given object.
         * @param da amount to grow in each direction.
         */
        void grow (const gfx::drawing_area & da)
        {
        	x_ -= da.x();
        	y_ -= da.y();
        	w_ += da.x() + da.length();
        	h_ += da.y() + da.height();
        }

        /**
         * Shrink the drawing area by the
         * values specified in the given object.
         * @param da amount to shrink in each direction.
         */
        void shrink (const gfx::drawing_area & da)
        {
        	x_ += da.x();
        	y_ += da.y();
        	w_ = da.x() + da.length() < w_ ? w_ - da.x() - da.length() : 0;
        	h_ = da.y() + da.height() < h_ ? h_ - da.y() - da.height() : 0;
        }

        /** Assign a drawing_area to this drawing_area.
         *  If a drawing area is assigned to another one, the zone covered
         *  by the drawing_area is the intersection of the two.
         *  @param da the drawing_area to assign.
         */  
        void assign_drawing_area (const drawing_area * da) 
        {
            draw_to = da; 
        }

        /** 
         * Returns a pointer to the drawing_area assigned to this one.
         * 
         * 
         * @return pointer to the assigned drawing_area, NULL if none.
         */
        const drawing_area * assigned_drawing_area () const
        {
            return draw_to; 
        }
    
        /** Detach (if needed) the drawing_area which was attached to this
         *  one.
         */ 
        void detach_drawing_area () 
        {
            draw_to = NULL;
        }
     
        /** 
         * Gets the real parameters of this drawing_area.
         * 
         * 
         * @return drawing_area which is the intersection of this drawing area and
         * all the drawing areas assigned to it.
         */
        drawing_area setup_rects () const; 

        /**
         * Returns whether a given point belongs to this drawing area.
         *
         * @param px X position of the point to test
         * @param py X position of the point to test
         *
         * @return true if the point is in this drawing area, false otherwise.
         *
         */ 
        bool point_belong (s_int16 px, s_int16 py); 

        
#ifndef SWIG        
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
        
        /**
         * Compare positions of two drawing areas in relation to each other.
         * @param the drawing_area to compare.
         * @param a combination of the flags defined above.
         */
        int compare (const drawing_area & da) const;
        
        GET_TYPE_NAME_VIRTUAL(gfx::drawing_area)
#endif // SWIG
    
    private:
        /// drawing_area location and size.
        s_int16 x_, y_;
        u_int16 w_, h_;

        /// Attached drawing_area.
        const drawing_area *draw_to; 
    }; 
}

#endif
