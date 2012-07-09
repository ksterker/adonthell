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
#include "rpg/character.h"

using world::character;
using world::pathfinding;
using world::coordinates;

bool pathfinding::verify_goal(const coordinates & actual, const vector3<s_int32> & p1, const vector3<s_int32> & p2)
{
    vector3<s_int32> tP1(p1.x() / 20, p1.y() / 20, 0);
    vector3<s_int32> tP2(p2.x() / 20, p2.y() / 20, 0);
    if (((actual.x() >= tP1.x()) && (actual.x() <= tP2.x())) &&
        ((actual.y() >= tP1.y()) && (actual.y() <= tP2.y())))
        return true;
    else return false;
}

u_int32 pathfinding::calc_heuristics(const coordinates & actual, const vector3<s_int32> & goal) const
{
    return  (20 * (abs(actual.x() * 20 - goal.x()) + abs(actual.y() * 20 - goal.y())));
}

bool pathfinding::check_node(coordinates & temp, const character * chr) const
{
    // Bypass check if the character is using the Default pathfinding_type
    if (chr->mind()->get_pathfinding_type() == "Default") return true;

    // Analyse the terrain
    world::vector3<s_int32> min_pos(temp.x() * 20, temp.y() * 20, chr->z() - 10);
    world::vector3<s_int32> max_pos(temp.x() * 20 + 20, temp.y() * 20 + 20, chr->z() + 10);

    const std::list<world::chunk_info *> actual_chunk = chr->map().objects_in_bbox(min_pos, max_pos);

    std::list<chunk_info *>::const_iterator g = actual_chunk.begin();

    float temp_terrainCost = 0;
    for (; g != actual_chunk.end(); g++)
    {
        if ((*g)->get_object()->get_terrain() != NULL)
        {
            temp_terrainCost = chr->mind()->get_pathfinding_cost(*(*g)->get_object()->get_terrain());

            // Check if we have to ignore this node
            if ((temp_terrainCost == 0) && (chr->mind()->has_forced_impassable()))
                return false;
        }
    }

    //Update move cost
    temp.set_z(temp.z() - temp_terrainCost);
    return true;
}

std::vector<coordinates> pathfinding::calc_adjacent_nodes(const coordinates & actual, const character * chr) const
{
    std::vector<coordinates> temp;
    coordinates temp_pos;

    temp.reserve(8);

    // TRICK: use the z as a pre-calculated cost to move
    // Left Center
    temp_pos.set(actual.x() - 1, actual.y(), 20);
    if (check_node(temp_pos, chr)) temp.push_back(temp_pos);

    //Left Top
    temp_pos.set(actual.x() - 1, actual.y() - 1, 28);
    if (check_node(temp_pos, chr)) temp.push_back(temp_pos);

    //Left Bottom
    temp_pos.set(actual.x() - 1, actual.y() + 1, 28);
    if (check_node(temp_pos, chr)) temp.push_back(temp_pos);

    // Right Bottom
    temp_pos.set(actual.x() + 1, actual.y() - 1, 28);
    if (check_node(temp_pos, chr)) temp.push_back(temp_pos);

    // Center Left
    temp_pos.set(actual.x(), actual.y() - 1, 20);
    if (check_node(temp_pos, chr)) temp.push_back(temp_pos);

    // Center Right
    temp_pos.set(actual.x(), actual.y() + 1, 20);
    if (check_node(temp_pos, chr)) temp.push_back(temp_pos);

    //Right Center
    temp_pos.set(actual.x() + 1, actual.y(), 20);
    if (check_node(temp_pos, chr)) temp.push_back(temp_pos);

    // Right Top
    temp_pos.set(actual.x() + 1, actual.y() + 1, 28);
    if (check_node(temp_pos, chr)) temp.push_back(temp_pos);

    return temp;
}

bool pathfinding::find_path(const character * chr, const vector3<s_int32> & goal1, const vector3<s_int32> & goal2,
                            std::vector<coordinates> * path)
{
    // Verify pre condictions
    if (!(((goal1.x() >= chr->map().min().x()) && (goal1.x() <= chr->map().max().x())) &&
        (goal1.y() >= chr->map().min().y()) && (goal1.y() <= chr->map().max().y()))) {

        fprintf(stderr, "*** Goal (%i, %i) is out of the map scope (%i, %i) - (%i, %i)\n",
                goal1.x(), goal1.y(), chr->map().min().x(), chr->map().min().y(),
                chr->map().max().x(), chr->map().min().y());
        return false;
    }

    // Constants regarding the lists to which a node can be assigned to
    const u_int8 OPEN_LIST = 1;
    const u_int8 CLOSED_LIST = 2;

    // Number of revolutions
    u_int16 rev = 0;

    // The max revolutions(iterations) (may need fine tuning)
    const float rev_per_dist = 5;
    const u_int16 max_rev = 1 + rev_per_dist * (abs(chr->x() - goal1.x()) + abs(chr->y() - goal1.y()));

    // Length of the character
    const u_int8 chr_length = chr->placeable::length();
    const u_int8 chr_width = chr->placeable::width();

    // Grid of the actual_node, centered on character
    s_int32 grid_x = trunc((chr->x() + chr_length / 2) / 20);
    s_int32 grid_y = trunc((chr->y() + chr_width / 2 ) / 20);

    // Middle position of the goal area
    vector3<s_int32> goal ((goal1.x() + goal2.x()) / 2, (goal1.y() + goal2.y()) / 2, 0);

    // The node that's been choosen (with the lowest total cost)
    node * actual_node;

    // Temporary variables
    coordinates temp_pos;
    node * temp_node;
    node * temp_node2;

    /* -------------------------------------------------------- */

    // Creates and adds the base node to the open list
    temp_node = m_nodeBank.get_node();

    temp_node->parent = temp_node; //Loops back to itself
    temp_node->total = 0;
    temp_node->moveCost = 0;
    temp_node->pos.set(grid_x, grid_y, chr->z());

    temp_node->listAssignedTo = OPEN_LIST;
    m_nodeCache.add_node(temp_node);
    m_openList.add_node(temp_node);

    while (!m_openList.is_empty() && (rev < max_rev))
    {
        // Get the lowest cost node in the Open List
        actual_node = m_openList.get_top();

        // Get the grid of the actual node
        grid_x = actual_node->pos.x();
        grid_y = actual_node->pos.y();
        temp_pos.set(grid_x, grid_y, 0);

        // Check if we've arrived at the target
        if (verify_goal(temp_pos, goal1, goal2) == true)
        {
            // Recurse through the nodes and find the path
            path->insert(path->begin(), actual_node->pos);

            temp_node = actual_node->parent;
            while (temp_node != temp_node->parent)
            {
                path->insert(path->begin(), temp_node->pos);
                temp_node = temp_node->parent;
            }

            // Resets the node cache, the open list and the node bank
            reset();
            return true;
        }

        // Change it from the open list to the closed list
        actual_node->listAssignedTo = CLOSED_LIST;

        // Gets a list of positions to visit
        std::vector<coordinates> pos_to_visit = calc_adjacent_nodes(temp_pos, chr);

        // Add them to the open list
        std::vector<coordinates>::iterator i = pos_to_visit.begin();

        while (i != pos_to_visit.end())
        {
            temp_node = m_nodeBank.get_node();

            temp_node->pos = *i;
            temp_node->parent = actual_node;
            temp_node->moveCost = (*i).z() + temp_node->parent->moveCost;
            temp_node->total = calc_heuristics(temp_node->pos, goal) + temp_node->moveCost;
            temp_node->pos.set_z(chr->z());

            // Check if this node is already in the open or closed list
            temp_node2 = m_nodeCache.search_node(temp_node);

            if (temp_node2 != NULL)
            {
                if (temp_node2->listAssignedTo == OPEN_LIST)
                {
                    // It's in the Open List, let's see if the move cost is lower now
                    if (temp_node->total < temp_node2->total)
                    {
                        // Updates the move cost and rebalances the Open List
                        temp_node2->moveCost = temp_node->moveCost;
                        temp_node2->total = temp_node->total;
                        temp_node2->parent = temp_node->parent;
                        m_openList.rebalance_node(temp_node2);
                    }
                }
            }
            else
            {
                // Check if the tile is a hole
                vector3<s_int32> min(temp_node->pos.x() * 20 + 9, temp_node->pos.y() * 20 + 9, temp_node->pos.z() - 20);
                vector3<s_int32> max(temp_node->pos.x() * 20 + 11, temp_node->pos.y() * 20 + 11, temp_node->pos.z());

                std::list<chunk_info *> check_hole = chr->map().objects_in_bbox(min, max, world::OBJECT);

                if (check_hole.empty())
                {
                    ++i;
                    continue;
                }

                // Check if there is an obstacle in this node
                min = vector3<s_int32>(temp_node->pos.x() * 20, temp_node->pos.y() * 20, temp_node->pos.z() + 1);
                max = vector3<s_int32>(temp_node->pos.x() * 20 + chr_length, temp_node->pos.y() * 20 + chr_width, temp_node->pos.z() + chr->height() - 1);

                std::list<chunk_info *> collisions = chr->map().objects_in_bbox(min, max);

                if (!collisions.empty())
                {
                    if (!((collisions.size() == 1) && (chr == (*collisions.begin())->get_object())))
                    {
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
