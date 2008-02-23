/*
 $Id: square.h,v 1.6 2008/02/23 20:51:17 ksterker Exp $
 
 Copyright (C) 2002 Alexandre Courbot <alexandrecourbot@linuxgames.com>
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
 * @file   world/square.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Declares the square class.
 * 
 * 
 */

#ifndef WORLD_SQUARE_H
#define WORLD_SQUARE_H

#include "world/moving.h"

namespace world
{
    /**
     * Information about an object on the world grid. This includes 
     * - a pointer to the object itself
     * - the offset to the object's top left corner and
     * - whether the object's base tile is on that grid cell.
     */
    class square_info : public coordinates
    {
    public:
        /// object the information belongs to
        placeable * obj;
        
        /**
         * Create new cell content information.
         * @param pos the position of the object.
         * @param ground_z position of ground under object
         * @param is_base whether this cell contains object's base tile.
         */
        square_info (coordinates & pos, const s_int32 & ground_z, const bool & is_base);
        
        /**
         * Check two square_info instances for the order they appear on
         * a square. Z and Y coordinates are used to determine order.
         * @param mi object to compare to
         * @return true if this is smaller than mi, false otherwise.
         */
        bool operator < (const square_info & mi) const;
        
        /**
         * Check two square_info instances for equality. They are 
         * equal if their position and object are the same.
         * @param mi object to compare to
         * @return true if this and mi are equal, false otherwise. 
         */
        bool operator == (const square_info & mi) const
        {
            // We call coordinates::operator == here
            return ((coordinates)*this == (coordinates)mi && 
                    obj == mi.obj);
        }
        
        /**
         * Return the Z coordinate of the object.
         * @param the Z position of the object.
         */
        s_int32 z_pos () const
        {
            return Z + obj->current_shape()->z() + obj->current_shape()->height();
        }
        
        /**
         * Check whether this is the base tile of the object.
         * @return true if it is the case, false otherwise.
         */
        bool is_base () const
        {
            return IsBase;
        }
        
    private:
        /// objects vertical position
        s_int32 GroundZ;
        /// whether this is an objects base tile.
        bool IsBase;
        
    }; 
    
    /**
     * A cell in the world grid. It contains a list of square_info objects
     * with information about the scenery and characters on that specific cell.
     * These are ordered by their z position, from lowest to highest.
     */
    class square
    {
    private:
        /// objects at this location.
        std::vector <square_info> objects; 
        
    public:
        /// an object vector iterator
        typedef std::vector <square_info>::iterator iterator;
        
        /**
         * Return iterator pointing to first object on that tile.
         * @return iterator pointing to front of object vector.
         */
        iterator begin ()
        {
            return objects.begin (); 
        }
        
        /**
         * Return iterator pointing after the last object on that tile.
         * @return iterator pointing after last element of object vector.
         */
        iterator end ()
        {
            return objects.end (); 
        }
        
        /**
         * Add a static object to this square.
         * @param obj the object to add.
         * @param pos the location of this object
         * @param ground_z position of ground under object
         * @param is_base whether object's base tile is on this square.
         */
        bool add (placeable * obj, coordinates & pos, const s_int32 & ground_z, const bool & is_ase); 
        
        /**
         * Remove a static object from this square.
         * @param obj the object to removes.
         * @param pos the object's location.
         */
        bool remove (placeable * obj, coordinates & pos);
        
        /**
         * Check whether given object exists in this square.
         * @param obj the object.
         * @param pos its location.
         */
        bool exist (placeable * obj, coordinates & pos); 
    };
}

#endif /* WORLD_SQUARE_H */
