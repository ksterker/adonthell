/*
  Copyright (C) 2009   Frederico Cerveira
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
  along with Adonthell; if not, write to the Free Software Foundation,
  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA
*/

/**
 * @file   world/zone.h
 * @author Frederico Cerveira <frederico.cerveira@gmail.com>
 *
 * @brief  Declares the zone class.
 *
 */

#ifndef ZONE_H
#define ZONE_H

#include "world/vector3.h"

namespace world
{
    /**
     * Marker for a certain area on the map, used for multiple
     * purposes. See zone types for details. Note that a single
     * zone can serve multiple purposes at the same time.
     */
    class zone
    {
    public:
        enum
        {
            /** Zone is used to name a location */
            TYPE_META = 1,
            /** Zone is used to render interiors */
            TYPE_RENDER = 2
        };

        /**
         * Ctor for a zone.
         * @param type any combination of zone types.
         * @param min the lower coordinates of the zone.
         * @param max the upper coordinates of the zone.
         */
        zone(u_int32 & type, world::vector3<s_int32> min, world::vector3<s_int32> max)
        {
            Type = type;
            Name = "";
            Min = min; 
            Max = max;
        }
        
        /** 
         * Ctor for a meta zone used to name a certain location.
         * @param name the (unique) name for a location.
         * @param min the lower coordinates of the zone.
         * @param max the upper coordinates of the zone.
         */
        zone(std::string & name, world::vector3<s_int32> min, world::vector3<s_int32> max)
        {
            Type = TYPE_META;
            Name = name; 
            Min = min; 
            Max = max;
        }

        /**
         * Set the name of zone.
         * @param name the name of the zone.
         */
        void set_name (const std::string & name)
        {
            Name = name;
        }
        
        /**
         * Returns the name of the zone
         * @return the name
         */
        const std::string & name() const
        {
            return Name;
        }

        /**
         * Returns the type of the zone. Can be a combination 
         * of valid zone types.
         * @return a bitmask of zone types set for this zone.
         */
        u_int32 type() const
        {
            return Type;
        }
        
        /**
         * Return a vector3 with the minimum point of the zone
         * @return the minimum point
         */
        world::vector3<s_int32> & min()
        {
            return Min;
        }

        /**
         * Return a vector3 with the maximum point of the chunk
         * @return the maximum point
         */
        world::vector3<s_int32> & max()
        {
            return Max;
        }

        /**
         * Return the extent of the zone in x direction.
         * @return extent of the zone in x direction.
         */
        u_int32 length() const
        {
            return Max.x() - Min.x();
        }

        /**
         * Return the extent of the zone in y direction.
         * @return the extent of the zone in y direction.
         */
        u_int32 height() const
        {
            return Max.y() - Min.y();
        }
        
    private:
        /// The position and length of the zone
        world::vector3<s_int32> Min, Max;

        /// The name of the zone
        std::string Name;
        
        /// The type of the zone
        u_int32 Type;
    };
}
#endif // ZONE_H
