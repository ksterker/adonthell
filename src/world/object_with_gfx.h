/*
 $Id: object_with_gfx.h,v 1.2 2007/05/21 04:44:12 ksterker Exp $
 
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
 * @file   world/object_with_gfx.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the object_with_gfx class.
 * 
 * 
 */


#ifndef WORLD_OBJECT_WITH_GFX
#define WORLD_OBJECT_WITH_GFX

#include "world/object.h"
#include "world/placeable_gfx.h"

namespace world
{
    /**
     * A object associated with its graphical representation.
     * 
     */
    class object_with_gfx : public object, public placeable_gfx
    {
    public:
        /**
         * Create %object with graphical representation on given map
         * @param mymap the area this %object belongs to.
         */
        object_with_gfx (area & mymap); 

        /**
         * Loading / Saving
         */
        //@{
        /**
         * Save %object state to stream. 
         * @param file stream to save %object to.
         * @return \b true if saving successful, \b false otherwise.
         */
        bool put_state (base::flat & file) const;
        
        /**
         * Load %object state from stream. 
         * @param file stream to load %object from.
         * @return \b true if loading successful, \b false otherwise.
         */
        bool get_state (base::flat & file);
        
        /**
         * Save %object state to file.
         * @param fname file name.
         * @return true on success, false otherwise.
         */
        bool save (const std::string & fname) const;
        
        /**
         * Load %object state from file.
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
         * Draw object graphics at given location.
         * @param x x offset
         * @param y y offset
         * @param da_opt clipping rectangel
         * @param target surface to draw on, NULL to draw on screen surface 
         */
        void draw(s_int16 x, s_int16 y, const gfx::drawing_area * da_opt = NULL,
                  gfx::surface * target = NULL) const;
        //@}
    }; 
}

#endif
