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
 * @file   world/pathfinding.h
 * @author Frederico Cerveira <frederico.cerveira@gmail.com>
 *
 * @brief  Declares the pathfinding class
 */

#ifndef WORLD_PATHFINDING_H
#define WORLD_PATHFINDING_H

#include "node_cache.h"
#include "node_bank.h"
#include "open_list.h"
#include "character.h"
#include "coordinates.h"

namespace world
{
    /**
     * Handles the pathfinding search stuff
     */

    class pathfinding
    {
    public:

        /**
         * Inits path calculator.
         * @param chr character to move
         * @param goal1 lower position of goal area
         * @param goal2 upper position of goal area
         * @return number of game cycles to use for updating path
         */
        u_int16 init (character *chr, const vector3<s_int32> & goal1, const vector3<s_int32> & goal2);

        /**
         * Calculates the path, if possible, and adds it to the vector passed
         * @param chr character to move
         * @param goal1 lower position of goal area
         * @param goal2 upper position of goal area
         * @param path (empty) vector to be filled with the path
         * @return \b true on success, \b false on failure
         */
        bool find_path(character *chr, const vector3<s_int32> & goal1, const vector3<s_int32> & goal2,
                       std::vector<coordinates> * path);

        /**
         * Resets the node bank, node cache and open list, so that it can be used again
         * for the next path calculation.
         */
        void reset()
        {
            m_openList.reset();
            m_nodeCache.reset();
            m_nodeBank.reset();
        }

    private:

        /**
         * Verifies if the goal has been completed
         * @param actual coordinate with the actual position
         * @param p1 vector3 representing the left-top-most point of the goal area
         * @param p2 vector3 representing the right-bottom-most point of the goal area
         * @return \b true if the goals has been completed, \b false otherwise
         */
         bool verify_goal(const coordinates & actual, const vector3<s_int32> & p1,
                          const vector3<s_int32> & p2);

        /**
         * Calculates the heuristics of two points
         * @param actual coordinate with the actual position
         * @param goal vector3 with the goal position
         * @return the heuristic
         */
        u_int32 calc_heuristics(const coordinates & actual, const vector3<s_int32> & goal) const;

        /**
         * Returns the 8 adjacent nodes to the one given
         * @param goal the coordinates of the central node
         * @param chr the character used in the pathfinding search
         * @return a vector with the coordinates of the adjacent nodes
         */
        std::vector<path_coordinate> calc_adjacent_nodes(const coordinates & goal, const character * chr) const;

        /**
         * Checks if the node is acceptable for use depending on the character's pathfinding_costs
         * @param temp position of the node
         * @param chr the character used in the pathfinding search
         * @return \b true if the node is valid, \b false otherwise
         */
        bool check_node(path_coordinate & temp, const character * chr) const;

        /**
         * Removes all completely non-solid object from the given list
         * and returns whether the list is empty afterwards.
         * @param objects list of map objects
         * @return true if all objects in the list were non-solid (or if
         *  the list was empty to begin with).
         */
        bool discard_non_solid(std::list<chunk_info*> & objects);

        /**
         * Check if the given ground tiles form a stair (or slope) in the
         * direction the path takes.
         * @param ground_tiles the list of ground tiles at path position
         * @param current the node the path extends to.
         * @return true if stairs are found, false otherwise.
         */
        bool check_stairs (std::list<chunk_info*> & ground_tiles, node *current);

        /**
         * Get the ground position from the list of tiles below the current path
         * @param ground_tiles list of tiles at the current node of the path
         * @param x x coordinate of nodes center
         * @param y y coordinate of nodes center
         * @return the ground level
         */
        s_int32 get_ground_pos (std::list<chunk_info*> & ground_tiles, const s_int32 & x, const s_int32 & y);

        /// The node bank
        node_bank m_nodeBank;
        /// The node cache
        node_cache m_nodeCache;
        /// The open list
        open_list m_openList;
    };
};

#endif // WORLD_PATHFINDING_H
