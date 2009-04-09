/*
  $Id: pathfinding.cc,v 1.2 2009/04/09 14:43:18 fr3dc3rv Exp $

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
 * @file   world/pathfinding.cc
 * @author Frederico Cerveira <frederico.cerveira@gmail.com>
 *
 * @brief  Implements the pathfinding class
 */

#include "world/pathfinding.h"
#include "world/character.h"
#include "world/area.h"
#include "world/vector3.h"
#include "world/chunk_info.h"
#include "world/coordinates.h"
#include "world/character.h"

using world::character;
using world::pathfinding;
using world::coordinates;

u_int32 pathfinding::calc_heuristics(const coordinates & actual, const vector3<s_int32> & goal) const
{
    return  (abs(actual.x() * 20 - goal.x()) + abs(actual.y() * 20 - goal.y()));
        /*return (sqrt(((actual.x() * 20 - goal.x()) * (actual.x() * 20 - goal.x())) +
            ((actual.y() * 20 - goal.y()) * (actual.y() * 20 - goal.y()))
    ));*/
    /*u_int8 D = 20;
    u_int8 D2 = 28;
    u_int32 h_diagonal = min(abs(actual.x()-goal.x()), abs(actual.y()-goal.y()));
    u_int32 h_straight = (abs(actual.x()-goal.x()) + abs(actual.y()-goal.y()));
    return  (D2 * h_diagonal + D * (h_straight - 2*h_diagonal));*/


}


std::vector<coordinates> pathfinding::calc_adjacent_nodes(const coordinates & actual) const
{
    std::vector<coordinates> temp;
    coordinates temp_pos;

    temp.reserve(8);

    // TRICK: use the z as a pre-calculated cost to move
    // Left Center
    temp_pos.set(actual.x() - 1, actual.y(), 20);
    temp.push_back(temp_pos);

    //Left Top
    temp_pos.set(actual.x() - 1, actual.y() - 1, 28);
    temp.push_back(temp_pos);

    //Left Bottom
    temp_pos.set(actual.x() - 1, actual.y() + 1, 28);
    temp.push_back(temp_pos);

    // Right Bottom
    temp_pos.set(actual.x() + 1, actual.y() - 1, 28);
    temp.push_back(temp_pos);

    // Center Left
    temp_pos.set(actual.x(), actual.y() - 1, 20);
    temp.push_back(temp_pos);

    // Center Right
    temp_pos.set(actual.x(), actual.y() + 1, 20);
    temp.push_back(temp_pos);

    //Right Center
    temp_pos.set(actual.x() + 1, actual.y(), 20);
    temp.push_back(temp_pos);

    // Right Top
    temp_pos.set(actual.x() + 1, actual.y() + 1, 28);
    temp.push_back(temp_pos);

    return temp;
}

bool pathfinding::find_path(const character * chr, const vector3<s_int32> & goal, std::vector<coordinates> * path)
{
    // Verify pre condictions
    if (!(((goal.x() >= chr->map().Min.x()) && (goal.x() <= chr->map().Max.x())) &&
        (goal.y() >= chr->map().Min.y()) && (goal.y() <= chr->map().Max.y()))) {

        fprintf(stderr, "Goal is out of the map scope\n");
        return false;
    }

    // Constants regarding the lists to which a node can be assigned to
    const u_int8 OPEN_LIST = 1;
    const u_int8 CLOSED_LIST = 2;

    // Number of revolutions
    u_int16 rev = 0;

    // The max revolutions(iterations) (may need fine tuning)
    const float rev_per_dist = 5;
    const u_int16 max_rev = 1 + rev_per_dist * (abs(chr->x() - goal.x()) + abs(chr->y() - goal.y()));

    // Grid of the actual_node
    s_int32 grid_x = trunc(chr->x() / 20);
    s_int32 grid_y = trunc(chr->y() / 20);

    // Grid of the goal
    const s_int32 goal_grid_x = trunc(goal.x() / 20);
    const s_int32 goal_grid_y = trunc(goal.y() / 20);

    // Pos of the character
    const s_int32 init_chr_x = chr->x();
    const s_int32 init_chr_y = chr->y();

    // The node that's been choosen (with the lowest total cost)
    node * actual_node;

    // Temporary variables
    coordinates temp_pos;
    u_int16 temp_move_cost;
    node * temp_node;
    node * temp_node2;
    placeable * temp_placeable = dynamic_cast<placeable *>(const_cast<character *>(chr));
    /* -------------------------------------------------------- */

    // Creates and adds the base node to the open list
    temp_node = m_nodeBank.get_node();

    temp_node->parent = temp_node; //Loops back to itself
    temp_node->total = 0;
    temp_node->moveCost = 0;
    temp_node->pos.set(grid_x, grid_y, 0);

    temp_node->listAssignedTo = OPEN_LIST;
    m_nodeCache.add_node(temp_node);
    m_openList.add_node(temp_node);

    while (!m_openList.is_empty() && (rev < max_rev)) {

        // Get the lowest cost node in the Open List
        actual_node = m_openList.get_top();

        // Get the grid of the actual node
        grid_x = actual_node->pos.x();
        grid_y = actual_node->pos.y();
        temp_pos.set(grid_x, grid_y, 0);

        // Check if we've arrived at the target
        if ((grid_x == goal_grid_x) && (grid_y == goal_grid_y)) {

            // Recurse through the nodes and find the path
            path->insert(path->begin(), actual_node->pos);

            temp_node = actual_node->parent;

            while (temp_node != temp_node->parent) {

                path->insert(path->begin(), temp_node->pos);

                temp_node = temp_node->parent;
            }

            // Resets the node cache, the open list and the node bank
            reset();
            printf("Rev: %d\n", rev);

            return true;
        }

        // Change it from the open list to the closed list
        actual_node->listAssignedTo = CLOSED_LIST;

        // Gets a list of positions to visit
        std::vector<coordinates> pos_to_visit = calc_adjacent_nodes(temp_pos);

        // Add them to the open list
        std::vector<coordinates>::iterator i = pos_to_visit.begin();

        while (i != pos_to_visit.end())
        {
            temp_node = m_nodeBank.get_node();

            temp_node->pos = *i;

            temp_node->parent = actual_node;
            temp_node->moveCost = (*i).z() + temp_node->parent->moveCost;
            temp_node->total = calc_heuristics(temp_node->pos, goal) + temp_node->moveCost;

            // Check if this node is already in the open or closed list
            temp_node2 = m_nodeCache.search_node(temp_node);

            if (temp_node2 != NULL) {

                if (temp_node2->listAssignedTo == OPEN_LIST) {
                    // It's in the Open List, let's see if the move cost is lower now
                    //temp_move_cost = (*i).z() + temp_node2->parent->moveCost;

                    if (temp_node->total < temp_node2->total) {
                        // Updates the move cost and rebalances the Open List

                        temp_node2->moveCost = temp_node->moveCost;
                        temp_node2->total = temp_node->total;
                        temp_node2->parent = temp_node->parent;
                        //temp_node2->total = calc_heuristics(temp_node2->pos, goal) + temp_node2->moveCost;

                        m_openList.rebalance_node(temp_node2);

                    }
                }

            } else {
                // Check if the tile is a hole
                vector3<s_int32> min(temp_node->pos.x() * 20, temp_node->pos.y() * 20, chr->z() - 5);
                vector3<s_int32> max(temp_node->pos.x() * 20 + 20, temp_node->pos.y() * 20 + 20,
                    chr->z());

                std::list<chunk_info *> check_hole = chr->map().objects_in_bbox(min, max);

                if (check_hole.empty()) {
                    ++i;
                    continue;
                }

                // Check if there is an obstacle in this node
                vector3<s_int32> cmin(temp_node->pos.x() * 20, temp_node->pos.y() * 20, 10);
                vector3<s_int32> cmax(temp_node->pos.x() * 20 + 20, temp_node->pos.y() * 20 + 20, chr->placeable::height());
                /*int half_length = round(chr->placeable::length() / 2);
                int half_width = round(chr->placeable::width() / 2);
                vector3<s_int32> cmin(temp_node->pos.x() * 40 + half_length,
                        temp_node->pos.y() * 40 + half_width, 20);
                vector3<s_int32> cmax(temp_node->pos.x() * 40 + 40 - half_length,
                        temp_node->pos.y() * 40 + 40 - half_width, 50);*/

                std::list<chunk_info *> collisions = chr->map().objects_in_bbox(cmin, cmax);

                if (!collisions.empty()) {

                    if ((collisions.size() == 1) && (temp_placeable == (*collisions.begin())->get_object()))
                    {

                    } else {

                        ++i;
                        continue;
                    }

                }

                // Add node to the open list
                temp_node->listAssignedTo = OPEN_LIST;
                m_nodeCache.add_node(temp_node);
                m_openList.add_node(temp_node);
            }

        ++i;
        }

        ++rev;
    }

    // Resets the node cache, the open list and the node bank
    reset();

    return false;
}
