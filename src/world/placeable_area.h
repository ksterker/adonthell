/*
 $Id: placeable_area.h,v 1.3 2007/06/10 03:58:22 ksterker Exp $
 
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
 * @file   world/placeable_area.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the placeable_area class.
 * 
 * 
 */


#ifndef WORLD_PLACEABLE_AREA_H_
#define WORLD_PLACEABLE_AREA_H_

#include <vector>

#include "world/coordinates.h"
#include "world/square_walkable_info.h"

namespace world
{
    /**
     * Area of squares that represents the occupation of a placeable
     * on the map. Used for collision detection.
     * 
     * FIXME: is that per pixel or per grid square?
     */
    class placeable_area
    {
    public:
        /**
         * Create new collision information for a placeable.
         */
        placeable_area()
        {
            zsize = 0;
        }

        /**
         * Get extension of area in x direction
         * @return extension in x direction.
         */
        u_int16 area_length() const
        {
            return area.size ();
        }
    
        /**
         * Get extension of area in y direction
         * @return extension in y direction.
         */
        u_int16 area_height() const;

        /**
         * Set extension of area
         * @param nx extension in x direction.
         * @param ny extension in y direction.
         */
        void set_area_size(u_int16 nx, u_int16 ny);

        /**
         * Get collision information for a particular location of the area.
         * @param x x coordinate
         * @param y y coordinate
         */
        square_walkable_info & get(u_int16 x, u_int16 y)
        {
            return area[x][y]; 
        }

        /**
         * Called once per game cycle by the engine to update state. 
         */
        bool update ()
        {
            return true; 
        }

        /**
         * Loading / Saving
         */
        //@{
        /**
         * Save %area state to stream. 
         * @param file stream to save %area to.
         * @return \b true if saving successful, \b false otherwise.
         */
        bool put_state (base::flat & file) const;
        /**
         * Load %area state from stream. 
         * @param file stream to load %area from.
         * @return \b true if loading successful, \b false otherwise.
         */        
        bool get_state (base::flat & file);
        //@}
        
        /// object height
        u_int16 zsize;
        /// object length and width
        coordinates base;
        
    private:
        /// collision information  
        std::vector <std::vector <square_walkable_info> > area;
    }; 
}

#endif
