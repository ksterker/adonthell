/*
   $Id: drawable.h,v 1.3 2003/11/22 09:35:21 ksterker Exp $

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
 * @file   gfx/drawable.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the drawable class.
 *  
 */


#ifndef DRAWABLE_H_
#define DRAWABLE_H_

#include "gfx/drawing_area.h"

#include "python/callback_support.h"

namespace gfx
{

    class surface; 

    /**
     * Abstract class for drawable %objects manipulation.
     *
     * This class is designed to allow flexibility in
     * drawable %objects manipulation. It also serves as
     * a template when creating your own classes.
     *
     * It defines the basic virtual methods every drawable
     * object is required to have. When you design such drawable
     * object, make it inherit from this class and overload the virtual
     * functions you wish to use.
     *
     * The main advantage of this class is that it allows you to manipulate
     * any drawable object (image, animation, mapview...) without caring
     * about it's type, at the little cost of having to use virtual methods.
     *
     * There are a few methods that are required to be overloaded
     * in your class. The draw method is a must-have. Your object must also
     * take care to set the size of the drawable correctly (the best thing
     * being that it should use the drawable's size as it's own and don't
     * overload the length () and height () methods).
     * 
     */
    class drawable
    {
    public:
    
        /** 
         * Default constructor.
         * 
         */
        drawable ();
    
        /**
         * Destructor.
         */ 
        virtual ~drawable ();

        /** 
         * Returns the length of the drawable.
         * 
         * 
         * @return length of the drawable.
         */
        u_int16 length () const
        {
            return length_; 
        }

        /** 
         * Returns the height of the drawable.
         * 
         * 
         * @return height of the drawable.
         */
        u_int16 height () const
        {
            return height_; 
        }

        /** 
         * Virtual update function, provided for %objects which
         * doesn't need one.
         * 
         */
        virtual bool update ();

        /** 
         * Draw the object on the %screen.
         * 
         * @param x X position where to draw.
         * @param y Y position where to draw.
         * @param da_opt optional drawing_area to use during the drawing operation.
         * @param target pointer to the surface where to draw the drawable. If NULL,
         *               draw on the screen.
         */
        virtual void draw (s_int16 x, s_int16 y, const drawing_area * da_opt = NULL,
                           surface * target = NULL) const = 0;

    protected:
    
        /** 
         * Sets the length of the drawable.
         * 
         * @param l new length.
         */
        void set_length (u_int16 l)
        {
            length_ = l; 
        }

        /**
         * Sets the height of the drawable.
         *
         * @param h new height.
         */ 
        void set_height (u_int16 h)
        {
            height_ = h; 
        }
    
    private:
        u_int16 length_; 
        u_int16 height_; 
    };

}

#ifndef SWIG
PYTHON_AS_CALLBACK_ARGUMENT(gfx::drawable)
#endif // SWIG
#endif
