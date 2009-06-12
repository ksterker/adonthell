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
     * Relation between a cubic area and a name
     */
    class zone
    {
    public:

        ///Ctor
        zone(std::string & name, world::vector3<s_int32> min, world::vector3<s_int32> max)
        {
            Name = name; Min = min; Max = max;
        }

        /**
         * Returns the name of the zone
         * @return the name
         */
        std::string & name()
        {
            return Name;
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
        u_int32 length()
        {
            return Max.x() - Min.x();
        }

        /**
         * Return the extent of the zone in y direction.
         * @return the extent of the zone in y direction.
         */
        u_int32 height()
        {
            return Max.y() - Min.y();
        }
    private:
        /// The position and length of the zone
        world::vector3<s_int32> Min, Max;

        /// The name of the zone
        std::string Name;
    };
}
#endif // ZONE_H
