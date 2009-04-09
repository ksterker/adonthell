/*
  $Id: pathfinding.h,v 1.2 2009/04/09 14:43:18 fr3dc3rv Exp $

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
     * Handles the pathfinding search stuff
     */

    class pathfinding
    {
    public:

        /**
         * Finds the path, if possible, and adds it to the vector passed
         * @param character to move, goal position, and (empty) vector to be filled with the path
         * @return \b true on success, \b false on failure
         */
        bool find_path(const character * chr, const vector3<s_int32> & goal, std::vector<coordinates> * path);

    private:

        /**
         * Resets the node bank, node cache and open list. So that it can be used again.
         */
        void reset()
        {
            m_nodeBank.reset();
            m_nodeCache.reset();
            m_openList.reset();
        }

        /**
         * Calculates the heuristics of two points
         * @param a coordinate with the actual position and a
         *        vector3 with the goal position
         * @return the heuristic
         */
        u_int32 calc_heuristics(const coordinates & actual, const vector3<s_int32> & goal) const;

        /**
         * Returns the 8 adjacent nodes to the one given
         * @param the coordinates of the central nodes
         * @return a list with the coordinates of the adjacent nodes
         */
        std::vector<coordinates> calc_adjacent_nodes(const coordinates & goal) const;

        /// The node bank
        node_bank m_nodeBank;
        /// The node cache
        node_cache m_nodeCache;
        /// The open list
        open_list m_openList;
    };


};

#endif // WORLD_PATHFINDING_H
