/*
 $Id: character_with_gfx.h,v 1.1 2007/05/19 07:42:08 ksterker Exp $
 
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
         * @name Loading / Saving
         */
        //@{
        void put(base::ogzstream & file) const;
        void get(base::igzstream & file);

        s_int8 save(const std::string fname) const;
        s_int8 load(const std::string fname);
        //@}
        
        /**
         * @name Rendering
         */
        //@{        
        void draw(s_int16 x, s_int16 y, const gfx::drawing_area * da_opt = NULL,
                  gfx::surface * target = NULL);
        void draw_shadow(s_int16 x, s_int16 y, const gfx::drawing_area * da_opt = NULL,
                         gfx::surface * target = NULL);
        //@}
        
    private:
        /// character shadow
        gfx::image shadow;
    }; 
}

#endif
