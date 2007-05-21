/*
 $Id: placeable_area_gfx.h,v 1.2 2007/05/21 04:44:12 ksterker Exp $
 
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
 * @file   world/placeable_area_gfx.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the placeable_area_gfx class.
 * 
 * 
 */

#ifndef WORLD_PLACEABLE_AREA_GFX
#define WORLD_PLACEABLE_AREA_GFX

#include "gfx/animation.h"
#include "world/placeable_area.h"

namespace world
{
    /**
     * Graphics representing the state of a placeable_model. Each placeable is
     * backed by a sprite consisting of one or more animations.
     */
    class placeable_area_gfx : public gfx::animation
    {
    public:
        /**
         * Create an empty placeable.
         */
        placeable_area_gfx ();

        /**
         * Destructor.
         */
        virtual ~placeable_area_gfx ();
        
        /**
         * Set size of area occupied by the placeable in number of squares.
         * @param nx nubmer of squares in x dimension
         * @param ny number of squares in y dimension
         */
        void set_area_size (u_int16 nx, u_int16 ny);
        
        /**
         * @name Loading / Saving
         */
        //@{
        /**
         * Save %animation state to stream. 
         * @param file stream to save %animation to.
         * @return \b true if saving successful, \b false otherwise.
         */
        bool put_state (base::flat & file) const
        {
            return gfx::animation::put_state (file);
        }

        /**
         * Load %animation state from stream. 
         * @param file stream to load %animation from.
         * @return \b true if loading successful, \b false otherwise.
         */
        bool get_state (base::flat & file)
        {
            return gfx::animation::get_state (file);
        }
        //@}
        
#ifndef SWIG
        GET_TYPE_NAME_VIRTUAL(world::placeable_area_gfx)
#endif // SWIG            
    }; 
}

#endif
