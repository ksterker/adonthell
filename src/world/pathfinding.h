/*
  $Id: pathfinding.h,v 1.1 2009/02/23 12:46:05 fr3dc3rv Exp $

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
 * @file   world/pathfinding.h
 * @author Frederico Cerveira <frederico.cerveira@gmail.com>
 *
 * @brief  Declares the pathfinding class
 */

#ifndef WORLD_PATHFINDING_H
#define WORLD_PATHFINDING_H

#include "world/node_cache.h"
#include "world/node_bank.h"
#include "world/open_list.h"
#include "world/character.h"
#include "world/coordinates.h"

namespace world
{
    /**
     * Handles the pathfinding stuff
     */

    class pathfinding
    {
    public:

        /**
         * Finds the path and move to a certain place in the map
         * @param character to move, and position to move to
         * @note z is still unimplemented
         * @return \b true if move was sucessful, \b false otherwise
         */
        //bool move_to(character * chr, vector3<s_int32> & target);

        /**
         * Finds the path and return it
         * @param character to move, and position to move to
         * @note this method will become private in the future
         *       and you should used the move_to instead
         * @return a list with the coordinates of the path
         */
        std::list<coordinates> find_path(character * chr, const vector3<s_int32> & target);

    private:

        /**
         * Calculates the heuristics of two pointa
         * @param a coordinate with the actual position and a
         *        vector3 with the target position
         * @return the heuristic
         */
        u_int32 calc_heuristics(const coordinates & actual, const vector3<s_int32> & target);

        /**
         * Returns the 8 adjacent nodes to the one given
         * @param the coordinates of the central node
         * @return a list with the coordinates of the adjacent nodes
         */
        std::vector<coordinates> calc_adjacent_nodes(const coordinates & target);

        /// Constants regarding the lists to which a node can be assigned to
        static const u_int8 OPEN_LIST = 1;
        static const u_int8 CLOSED_LIST = 2;

        /// The node bank
        node_bank m_nodeBank;
        /// The node cache
        node_cache m_nodeCache;
        /// The open list
        open_list m_openList;
    };


};

#endif // WORLD_PATHFINDING_H
