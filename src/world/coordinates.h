/*
 $Id: coordinates.h,v 1.3 2007/09/04 02:27:18 ksterker Exp $
 
 Copyright (C) 2002/2007 Alexandre Courbot <alexandrecourbot@linuxgames.com>
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
 * @file   world/coordinates.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the coordinates class.
 * 
 * 
 */

#ifndef WORLD_COORDINATES_H
#define WORLD_COORDINATES_H

#include "base/types.h"

namespace world
{
    /// size of a cell in the world grid
    const u_int16 SQUARE_SIZE = 40;
    
    /**
     * World space representation of any object.
     */
    class coordinates
    {
    public:
        /**
         * Create a coordinate pointing to the origin.
         * All values are set to zero.
         */
        coordinates ();
        
        /**
         * Create a coordinate with the given location.
         * @param x location on the x axis of the grid
         * @param y location on the y axis of the grid
         * @param z altitude
         * @param ox pixel offset from x location
         * @param oy pixel offset from y location
         */
        coordinates (u_int16 x, u_int16 y, u_int16 z, u_int16 ox = 0, u_int16 oy = 0); 

        /**
         * @name Member Access
         */
        //@{
        /**
         * Return x coordinate on the grid.
         * @return x coordinate
         */
        u_int16 x () const
        {
            return X; 
        }

        /**
         * Return y coordinate on the grid.
         * @return y coordinate
         */
        u_int16 y () const
        {
            return Y; 
        }

        /**
         * Return altitude of the object.
         * @return position along the z axis.
         */
        s_int32 z() const
        {
            return Z;
        }

        /**
         * Get pixel offset in x direction. This is the object's position 
         * relative to the upper left corner of it's cell on the grid.
         * @return pixel offset in x direction.
         */
        u_int16 ox () const
        {
            return Ox; 
        }

        /**
         * Get pixel offset in y direction. This is the object's position 
         * relative to the upper left corner of it's cell on the grid.
         * @return pixel offset in y direction.
         */
        u_int16 oy () const
        {
            return Oy; 
        }
        //@}
        
        /**
         * @name Coordinate Manipulation
         */
        //@{
        /**
         * Set position on the grid.
         * @param x new x coordinate
         * @param y new y coordinate
         */
        void set_position (u_int16 x, u_int16 y) 
        {
            X = x;
            Y = y; 
        }

        /**
         * Set altitude.
         * @param z new altitude.
         */
        void set_altitude(s_int32 z)
        {
            Z = z;
        }

        /**
         * Set pixel offset to the current location on the grid. 
         * @param ox offset in x direction
         * @param oy offset in y direction
         */
        void set_offset (u_int16 ox, u_int16 oy) 
        {
            Ox = ox;
            Oy = oy; 
        }

        /**
         * Positive offset by given coordinates.
         * @param mc offset.
         * @return this
         */
        coordinates & operator + (const coordinates & mc); 
        /**
         * Negative offset by given coordinates.
         * @param mc offset.
         * @return this
         */
        coordinates & operator - (const coordinates & mc); 
        //@}
        
        /**
         * @name Coordinate Comparison
         */
        //@{
        /**
         * Compare to sets of coordinates for equality.
         * @param mc coordinates to compare with this.
         * @return true if all members are equal, false otherwise.
         */
        bool operator == (const coordinates & mc); 

        /**
         * Check whether this is "smaller" than given coordinates.
         * @param mc coordinates to compare with this.
         * @return true if all members are smaller, false otherwise.
         */
        bool operator < (const coordinates & mc); 

        /**
         * Check whether this is "smaller" than given coordinates or equal.
         * @param mc coordinates to compare with this.
         * @return true if all members are smaller or equal, false otherwise.
         */
        bool operator <= (const coordinates & mc); 

        /**
         * Check whether this is "greater" than given coordinates.
         * @param mc coordinates to compare with this.
         * @return true if all members are greater, false otherwise.
         */        
        bool operator > (const coordinates & mc); 

        /**
         * Check whether this is "greater" than given coordinates or equal.
         * @param mc coordinates to compare with this.
         * @return true if all members are greater or equal, false otherwise.
         */
        bool operator >= (const coordinates & mc); 
        //@}

    protected:
        /// location on the grid
        u_int16 X, Y;
        /// altitude
        s_int32 Z;
        /// pixel(?) offset from x and y location
        u_int16 Ox, Oy;
    };
}

#endif
