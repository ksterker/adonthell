/*
 $Id: character_with_gfx.h,v 1.3 2007/06/03 02:28:38 ksterker Exp $
 
 Copyright (C) 2002 Alexandre Courbot <alexandrecourbot@linuxgames.com>
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
 * @file   world/character_with_gfx.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the character_with_gfx class.
 * 
 * 
 */


#ifndef WORLD_CHARACTER_WITH_GFX_H
#define WORLD_CHARACTER_WITH_GFX_H

#include "gfx/image.h"
#include "world/character.h"
#include "world/placeable_gfx.h"

namespace world
{
    /**
     * A character along with its graphical representation.
     */
    class character_with_gfx : public character, public placeable_gfx
    {
    public:
        /**
         * Create a character with graphics.
         * @param mymap map this character is on.
         */
        character_with_gfx (area & mymap); 

        /**
         * Loading / Saving
         */
        //@{
        /**
         * Save %character state to stream. 
         * @param file stream to save %character to.
         * @return \b true if saving successful, \b false otherwise.
         */
        bool put_state (base::flat & file) const;
        
        /**
         * Load %character state from stream. 
         * @param file stream to load %character from.
         * @return \b true if loading successful, \b false otherwise.
         */
        bool get_state (base::flat & file);
        
        /**
         * Save character state to file.
         * @param fname file name.
         * @return true on success, false otherwise.
         */
        bool save (const std::string & fname) const;
        
        /**
         * Load character state from file.
         * @param fname file name.
         * @return true on success, false otherwise.
         */
        bool load (const std::string & fname);
        //@}
        
        /**
         * @name Rendering
         */
        //@{
        /**
         * Draw the current frame of the character's sprite. 
         * @param x x location
         * @param y y location
         * @param da_opt clipping rectangle
         * @param target surface or NULL for screen
         */
        void draw(s_int16 x, s_int16 y, const gfx::drawing_area * da_opt = NULL,
                  gfx::surface * target = NULL);
        
        /**
         * Draw the character's shadow. It will be drawn on the ground
         * beneath the character, regardless of its current z position.
         * @param x x location
         * @param y y location
         * @param da_opt clipping rectangle
         * @param target surface or NULL for screen
         */
        void draw_shadow(s_int16 x, s_int16 y, const gfx::drawing_area * da_opt = NULL,
                         gfx::surface * target = NULL);
        //@}
        
    private:
        /// character shadow
        gfx::image shadow;
    }; 
}

#endif
