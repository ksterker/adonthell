/*
 $Id: square.h,v 1.2 2007/10/15 02:19:34 ksterker Exp $
 
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
     * Information about an object on the world grid.
     */
    class square_info : public coordinates
    {
    public:
        /// objects vertical position
        s_int32 zground;
        /// object the information belongs to
        placeable * obj;
        
        /**
         * Create new object information.
         * @param pos the position of the object.
         */
        square_info (coordinates & pos);
        
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
    }; 
    
    /**
     * A cell in the world grid. It contains a list of objects that
     * are located at this location.
     */
    class square
    {
    private:
        /// objects at this location.
        std::vector <square_info> objects; 
        
    public:
        /// an object vector iterator
        typedef std::vector <square_info>::iterator iterator;
        
        iterator begin ()
        {
            return objects.begin (); 
        }
        
        iterator end ()
        {
            return objects.end (); 
        }
        
        /**
         * Add a static object to this square.
         * @param obj the object to add.
         * @param pos the location of this object
         */
        bool add (placeable * obj, coordinates & pos); 
        
        /**
         * Add a moving object to this square.
         * @param obj the object to add.
         */
        bool add (moving * obj);
        
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
        
        /**
         * ???
         * @param obj
         * @return
         */
        square_info * project_moving (moving & obj)
        {
            //        placeable_shape * movstat = obj.current_shape();
            
            square::iterator it = begin();
            while (it != end())
            {
                placeable_shape * objstat = it->obj->current_shape();
                if (obj.z() + obj.climb_capability() > it->z() + objstat->height()) break;
                ++it;
            }
            
            if (it == end()) return NULL;
            // FIXME: that doesn't look right
            return NULL;
        }
    };
}

#endif /* WORLD_SQUARE_H */
