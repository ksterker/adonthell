/*
  $Id: pathfinding.cc,v 1.1 2009/02/23 12:46:04 fr3dc3rv Exp $

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

/*
bool pathfinding::move_to(character * chr, vector3<s_int32> & target)
{
    vector3<float> vel(50, 50, 50);
    vector3<float> sphere(20, 12.5 ,40);
    vector3<float> pos_to_move;

    std::list<coordinates> path_to_follow = find_path(chr, target);

    if (path_to_follow.empty()) //Failed to find the path
        return false;

    while (!path_to_follow.empty())
    {
        pos_to_move.set(path_to_follow.front().x() * 40,
                         path_to_follow.front().y() * 40,
                         0);

        path_to_follow.pop_front();

        collision col_data(pos_to_move, vel, sphere);
        chr->set_position(pos_to_move.x(), pos_to_move.y());

        //chr->execute_move(&col_data, 0);
        //chr->update_position();
        printf("Go one\n");
    }
    return true;
}
*/
u_int32 pathfinding::calc_heuristics(const coordinates & actual, const vector3<s_int32> & target)
{
    return (abs(actual.x() * 40 - target.x()) + abs(actual.y() * 40 - target.y())) * 40;
}

std::vector<coordinates> pathfinding::calc_adjacent_nodes(const coordinates & actual)
{
    std::vector<coordinates> temp;
    coordinates temp_pos;

    temp.reserve(8);

    // TRICK: use the z as a pre-calculated cost to move
    // Left Center
    temp_pos.set(actual.x() - 1, actual.y(), 40);
    temp.push_back(temp_pos);
    //Left Top
    temp_pos.set(actual.x() - 1, actual.y() - 1, 56);
    temp.push_back(temp_pos);
    //Left Bottom
    temp_pos.set(actual.x() - 1, actual.y() + 1, 56);
    temp.push_back(temp_pos);
    // Center Left
    temp_pos.set(actual.x(), actual.y() - 1, 40);
    temp.push_back(temp_pos);
    // Center Right
    temp_pos.set(actual.x(), actual.y() + 1, 40);
    temp.push_back(temp_pos);
    //Right Center
    temp_pos.set(actual.x() + 1, actual.y(), 40);
    temp.push_back(temp_pos);
    // Right Top
    temp_pos.set(actual.x() + 1, actual.y() + 1, 56);
    temp.push_back(temp_pos);
    // Right Bottom
    temp_pos.set(actual.x() + 1, actual.y() - 1, 56);
    temp.push_back(temp_pos);

    return temp;
}

std::list<coordinates> pathfinding::find_path(character * chr, const vector3<s_int32> & target)
{
    // Grid of the actual_node
    s_int32 grid_x = trunc(chr->x() / 40);
    s_int32 grid_y = trunc(chr->y() / 40);

    // Grid of the target
    const s_int32 target_grid_x = trunc(target.x() / 40);
    const s_int32 target_grid_y = trunc(target.y() / 40);

    // Pos of the character
    const s_int32 init_chr_x = chr->x();
    const s_int32 init_chr_y = chr->y();

    // The node that's been choosen with the lowest
    // total cost
    node * actual_node;

    // Temporary stuff
    coordinates temp_pos;
    u_int16 temp_move_cost;
    node * temp_node;
    node * temp_node2;
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

    while (!m_openList.is_empty()) {

        // Get the lowest cost node in the Open List
        actual_node = m_openList.get_top();

        // Get the grid of the actual node
        grid_x = actual_node->pos.x();
        grid_y = actual_node->pos.y();
        temp_pos.set(grid_x, grid_y, 0);

        // Check if we've arrived at the target
        if ((grid_x == target_grid_x) && (grid_y == target_grid_y)) {
            //printf("arrived");

            //Place the char in its proper place
            chr->set_position(init_chr_x, init_chr_y);

            // Recurse through the nodes and find the path
            std::list<coordinates> path;
            path.push_front(actual_node->pos);

            temp_node = actual_node->parent;

            while (temp_node != temp_node->parent) {
                path.push_front(temp_node->pos);

                temp_node = temp_node->parent;
            }

            // Resets the node cache, the open list and the node bank
            // NOTE: should be in the move_to func instead
            m_nodeBank.reset();
            m_nodeCache.reset();
            m_openList.reset();

            return path;
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

            // Check if this node is already in the open or closed list
            temp_node2 = const_cast<node *>(m_nodeCache.search_node(temp_node));

            if (temp_node2 != NULL) {

                if (temp_node2->listAssignedTo == OPEN_LIST) {
                    // It's in the Open List, let's see if the move cost is lower now
                    temp_move_cost = (*i).z() + temp_node2->parent->moveCost;

                    if (temp_move_cost < temp_node2->moveCost) {
                        // Updates the move cost and rebalances the Open List

                        temp_node2->moveCost = temp_move_cost;
                        temp_node2->total = calc_heuristics(temp_node2->pos, target) + temp_node2->moveCost;

                        m_openList.rebalance_node(temp_node2);

                    }
                }

            } else {
                // Check to see whether it is empty
                vector3<s_int32> min(temp_node->pos.x() * 40, temp_node->pos.y() * 40, 0);
                vector3<s_int32> max(temp_node->pos.x() * 40 + 40, temp_node->pos.y() * 40 + 40,
                    1);

                std::list<chunk_info *> check_empty = chr->map().objects_in_bbox(min, max);

                if (check_empty.size() == 0) {
                    ++i;
                    continue;
                }

                // Check if it has a obstacle in it
                vector3<s_int32> cmin(temp_node->pos.x() * 40, temp_node->pos.y() * 40, 20);
                vector3<s_int32> cmax(temp_node->pos.x() * 40 + 40, temp_node->pos.y() * 40 + 40, 50);

                std::list<chunk_info *> collisions = chr->map().objects_in_bbox(cmin, cmax);

                if (collisions.size() > 0) {
                    ++i;
                    continue;
                }


                // Ask for free node, initialize, add to open list
                temp_node->parent = actual_node;
                temp_node->moveCost = (*i).z() + temp_node->parent->moveCost;
                temp_node->total = calc_heuristics(temp_node->pos, target) + temp_node->moveCost;

                temp_node->listAssignedTo = OPEN_LIST;
                m_nodeCache.add_node(temp_node);
                m_openList.add_node(temp_node);
            }

        ++i;
        }
    }

    //Place the char in its proper place
    chr->set_position(init_chr_x, init_chr_y);

    // Resets the node cache, the open list and the node bank
    m_nodeBank.reset();
    m_nodeCache.reset();
    m_openList.reset();

    std::list<coordinates> empty_list;
    return empty_list;
}
