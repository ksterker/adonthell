/*
   $Id: drawable.h,v 1.1 2003/07/18 15:16:09 gnurou Exp $

   Copyright (C) 1999/2000/2001/2002   Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
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

PYTHON_AS_CALLBACK_ARGUMENT(gfx::drawable)

#endif
