/*
   $Id: sprite.h,v 1.1 2008/07/10 20:19:37 ksterker Exp $

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
 * @file   gfx/sprite.h
 * @author Tyler Nielsen <tyler.nielsen@gmail.com>
 *
 * @brief  Declares the sprite interface.
 *
 *
 */


#ifndef GFX_SPRITE_H
#define GFX_SPRITE_H

#include "event/listener_cxx.h"

#include "gfx/surface_cacher.h"
#include "gfx/surface.h"
#include <queue>

namespace gfx
{
    /**
     * Class to handle storing several surfaces in an sprite sequence.
     *
     */
    class sprite : public drawable
    {
    public:
        /**
         * Default constructor.
         */
        sprite ();

        /**
         * Destructor.
         */
        virtual ~sprite ();

        /**
         * Reset the %sprite.
         */
        void clear ();
        
        /**
         * @name Animation Control
         */
        //@{
        /**
         * Change the current animation we are using
         *
         * @param new_animation string that represents the new animation we want to switch to
         * @return true if new_animation was found.
         */
        bool change_animation (const std::string & new_animation);
        
        /**
         * Whether all image data has been loaded for the sprite.
         * @return true if sprite is fully initialized, false otherwise.
         */
        bool is_valid () const
        {
            return m_valid;
        }
        
        /**
         * Update sprite. Called once per game cycle to advance the
         * current frame of the current sprite, if neccessary.
         * @return true.
         */
        virtual bool update ();
        
        /**
         * Start playing animation
         */
        virtual void play ();
        
        /**
         * Stop playing animation.
         */
        virtual void stop ();
        
        /**
         * Set sprite back to first frame.
         */
        virtual void rewind ();
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
        virtual void draw (const s_int16 & x, const s_int16 & y, const drawing_area * da_opt = NULL,
                           surface * target = NULL) const
        {
            draw (x, y, 0, 0, length(), height(), da_opt, target);
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
        */
        virtual void draw (s_int16 x, s_int16 y, s_int16 sx, s_int16 sy, u_int16 sl,
                           u_int16 sh, const drawing_area * da_opt = NULL,
                           surface * target = NULL) const
        {
            if (m_valid) (*m_surface)->image->s->draw(x, y, sx, sy, sl, sh, da_opt, target);
        }
        //@}
        
        /**
         * @name Loading / Saving
         */
        //@{
        /**
         * Get file name the %sprite was loaded from, if any.
         * @return file name or empty string, if %sprite was loaded elsewhere.
         */
        std::string filename () const; 
        
        /**
         * Set file name to load the %sprite from.
         * @param filename path to file containing %sprite data.
         */
        void set_filename (const std::string & filename)
        {
            m_filename = filename;
        }
        
        /**
         * Save %sprite state to stream. 
         * @param file stream to save %sprite to.
         * @return \b true if saving successful, \b false otherwise.
         */
        bool put_state (base::flat & file) const;
        
        /**
         * Load %sprite state from stream. 
         * @param file stream to load %sprite from.
         * @return \b true if loading successful, \b false otherwise.
         */
        bool get_state (base::flat & file);        
        
        /**
         * Load %sprite from a xml file
         * 
         * @param filename xml file to load
         * @return true if successful
         */
        bool load (const std::string & filename = "");
        
        /**
         * Save %sprite to a xml file
         * 
         * @param filename xml file to save
         * @return true if successful
         */
        bool save (const std::string & filename) const;
        //@}
        
#ifndef SWIG
        GET_TYPE_NAME_VIRTUAL(gfx::sprite)
#endif // SWIG

    protected:
        /**
         * data representing one frame of animation
         *
         */
        struct animation_frame
        {
            /**
             * Create a new frame
             * @param img the image data
             * @param dly the delay before switching to the next frame
             */
            animation_frame(const surface_ref *img, u_int16 dly = 0):
            image(img), delay(dly) { }
            
            /**
             * Destroy frame and free reference to associated surface.
             */
            ~animation_frame()
            {
                delete image;
            }
            
            /// what image to draw for this frame
            const surface_ref *image;
            /// how long before we switch to the next frame
            u_int16 delay;              
        };

        /**
         * @name Animation Storage
         * Typedefs for the animation storage
         */
        //@{
        /// The different frames in an animation
        typedef std::deque<const animation_frame *> animation_list;
        /// A named animation sequence (one state of a sprite)
        typedef std::map<std::string, animation_list> animation_map;
        //@}
        
        /// states of the sprite
        animation_map m_states;
        /// an animation
        animation_map::iterator m_animation;
        /// a frame
        animation_list::iterator m_surface;
        
        /// event listener for animation playback
        events::listener_cxx *m_listener;
        /// all images are loaded
        bool m_valid;
        /// animation is playing
        bool m_playing;
        /// name of the sprite
        std::string m_filename;
        
    private:
        /**
         * Forbid copy construction.
         *
         */
        sprite (const sprite & src);
    };
}

#endif
