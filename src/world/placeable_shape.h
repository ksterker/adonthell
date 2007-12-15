/*
 $Id: placeable_shape.h,v 1.4 2007/12/15 23:15:10 ksterker Exp $
 
 Copyright (C) 2007 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file   world/placeable_shape.h
 * @author  Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Declares the placeable_shape class.
 * 
 * 
 */


#ifndef WORLD_PLACEABLE_AREA_H_
#define WORLD_PLACEABLE_AREA_H_

#include "world/cube3.h"

namespace world
{
    /**
     * A rough 3D representation of a placeable on the map. It consists
     * of a number of hexahedrons and a bounding box. It is used for 
     * collision detection and rendering order.
     * 
     * All values returned are pixel based.
     */
    class placeable_shape
    {
    public:
        /**
         * Create new collision information for a placeable.
         */
        placeable_shape()
        {
        }

        /**
         * Add a part to this shape.
         * @param part the part to add to the shape.
         */
        void add_part (cube3 * part);
        
        /**
         * Perform collision against this object. Result is stored in
         * given collisionData parameter.
         * @param collisionData information about the performed move.
         * @param offset position of shape on the world map
         */
        void collide (collision * collisionData, const vector3<s_int16> & offset) const;
        
        /**
         * @name Extension of Shape
         */
        //@{
        /**
         * Get extension of shape in x direction
         * @return extension in x direction.
         */
        u_int16 length () const
        {
            return Max.x() - Min.x();
        }

        /**
         * Get extension of shape in y direction
         * @return extension in y direction.
         */
        u_int16 width () const
        {
            return Max.y() - Min.y();
        }

        /**
         * Get extension of shape in z direction
         * @return extension in z direction.
         */
        u_int16 height () const
        {
            return Max.z() - Min.z();
        }
        //@}
        
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

        /// minimum of object bounding box
        vector3<s_int16> Min;
        /// maximum of object bounding box
        vector3<s_int16> Max;
        
    private:
        /// collision information  
        std::vector <cube3*> Parts;
    }; 
}

#endif
