/*
   $Id: animation.h,v 1.1 2006/06/03 04:20:27 Mithander Exp $

   Copyright (C) 1999/2000/2001/2002/2003   Alexandre Courbot <alexandrecourbot@linuxgames.com>
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
 * @file   gfx/animation.h
 * @author Tyler Nielsen <tyler.nielsen@gmail.com>
 *
 * @brief  Declares the animation interface.
 *
 *
 */


#ifndef ANIMATION_H_
#define ANIMATION_H_

#include "gfx/drawable.h"
#include "gfx/gfx.h"
#include <map>
#include <queue>

namespace gfx
{

    /**
     * Class to handle storing several surfaces in an animation sequence.
     *
     */
    class animation : public drawable
    {
    public:
        /**
         * Default constructor.
         *
         */
        animation () : m_valid(false), m_x_offset(0), m_y_offset(0) { }

        /**
         * Destructor.
         *
         */
        virtual ~animation () { }

        /**
         * @name Settings
         * These methods sets the parameters of the surface.
         *
         */
        //@{

        /**
         * Load the available animtions from a xml file
         * 
         * @todo Implement xml loading...
         * @param filename xml file to load
         * @return true if successful
         */
        bool load_animation (const std::string & filename);

        /**
         * Change the current animation we are using
         *
         * @param new_animation string that represents the new animation we want to switch to
         * @return true if new_animation was found.
         */
        bool change_animation (const std::string & new_animation);


        //@}


        /**
         * @name Drawing Methods
         *
         */
        //@{

        /**
         * Draw the surface.
         *
         * @param x X position where to draw.
         * @param y Y position where to draw.
         * @param da_opt optional drawing_area to use during the drawing operation.
         * @param target pointer to the surface where to draw the drawable. If NULL, draw on the screen.
         */
        void draw (s_int16 x, s_int16 y, const drawing_area * da_opt = NULL,
                   surface * target = NULL) const
        {
            draw (x, y, 0, 0, length (), height (), da_opt, target);
        }

        /**
         * Draw a part of the surface.
         *
         * @param x X position where to draw.
         * @param y Y position where to draw.
         * @param sx X position where to start drawing from this image.
         * @param sy Y position where to start drawing from this image.
         * @param sl length of the part of this image to draw.
         * @param sh height of the part of this image to draw.
         * @param da_opt optional drawing_area to use during the drawing operation.
         * @param target pointer to the surface where to draw the drawable. If NULL, draw on the screen.
         *
         * @attention Not accessible from Python. Use draw_part () from Python instead.
         * @sa draw_part ()
         *
         */
        virtual void draw (s_int16 x, s_int16 y, s_int16 sx, s_int16 sy, u_int16 sl,
                           u_int16 sh, const drawing_area * da_opt = NULL,
                           surface * target = NULL) const
        {
            if (m_valid) (*m_surface)->image->draw(x + m_x_offset, y + m_y_offset, sx, sy, sl, sh, da_opt, target);
        }

        //@}

        /**
         * Update function, lets target know it is time to update animation
         *
         */
        virtual bool update ();

#ifndef SWIG
        GET_TYPE_NAME_VIRTUAL(gfx::animation)
#endif // SWIG

    protected:
        /**
         * data representing one frame of animation
         *
         */
        struct animation_frame
        {
            const surface *image;       //what image to draw for this frame
            u_int16 delay;              //how long before we switch to the next frame
            s_int16 x_offset;           //what is the x_offset for this frame
            s_int16 y_offset;           //what is the y_offset for this frame

            /**
             * Constructor to set the values
             */
            animation_frame(const surface *img, u_int16 dly = 0, s_int16 xOffset = 0, s_int16 yOffset = 0):
                image(img), delay(dly), x_offset(xOffset), y_offset(yOffset) { }
        };

        /**
         * typedefs for the animation storage
         *
         */

        typedef std::deque<const animation_frame *> animation_list;
        typedef std::map<std::string, animation_list> animation_map;
        typedef std::map<std::string, animation_map> animation_cache;

        /**
         * All data is stored here.  One huge map allowing for access to different parts through iterators.
         *
         */
        static animation_cache m_allAnimations;

        animation_cache::iterator m_sprite;
        animation_map::iterator m_animation;
        animation_list::iterator m_surface;

        u_int16 m_delay;
        bool m_valid;

        s_int16 m_x_offset;           //what is the x offset that we must still keep track of
        s_int16 m_y_offset;           //what is the y offset that we must still keep track of

    private:
        /**
         * Forbid copy construction.
         *
         */
        animation (const animation & src);
    };
}

#endif
