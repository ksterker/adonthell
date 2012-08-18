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

#include "pathfinding.h"
#include "character.h"
#include "area.h"
#include "vector3.h"
#include "chunk_info.h"
#include "coordinates.h"
#include <adonthell/rpg/character.h>

#if 0
#include "area_manager.h"
#include <adonthell/gfx/screen.h>
#endif

using world::character;
using world::chunk_info;
using world::pathfinding;
using world::coordinates;
using world::path_coordinate;

// Constants regarding the lists to which a node can be assigned to
static const u_int8 OPEN_LIST = 1;
static const u_int8 CLOSED_LIST = 2;

// number of path nodes to search per game cycle
static const u_int32 MAX_REV_PER_FRAME = 1000;

/// sort chunk_info objects according to the z-position of their surface
struct z_order : public std::binary_function<const chunk_info *, const chunk_info *, bool>
{
    bool operator() (const chunk_info * a, const chunk_info * b)
    {
        //Here we want to sort by the top of each solid.  However, if they have the same top,
        //then we want the one with the highest bottom (which is the smallest height).  The reason
        //for this is we have tiles and walls with the same height, but we want to prefer to draw on
        //the tile if the player is standing on it.
        s_int32 atop = a->center_min().z() + a->get_object()->get_surface_pos ();
        s_int32 btop = b->center_min().z() + b->get_object()->get_surface_pos ();
        return (atop > btop) || (atop == btop && a->get_object()->solid_height () < b->get_object()->solid_height ());
    }
};

bool pathfinding::verify_goal(const coordinates & actual, const vector3<s_int32> & p1, const vector3<s_int32> & p2)
{
    vector3<s_int32> tP1(p1.x() / 20, p1.y() / 20, p1.z());
    vector3<s_int32> tP2(p2.x() / 20, p2.y() / 20, p2.z());

    if (((actual.x() >= tP1.x()) && (actual.x() <= tP2.x())) &&
        ((actual.y() >= tP1.y()) && (actual.y() <= tP2.y())) &&
        ((actual.z() >= tP1.z()) && (actual.z() <= tP2.z())))
        return true;

    return false;
}

u_int32 pathfinding::calc_heuristics(const coordinates & actual, const vector3<s_int32> & goal) const
{
    // standard heuristic when diagonal movement is allowed
    u_int32 dist_x = abs(actual.x() * 20 - goal.x());
    u_int32 dist_y = abs(actual.y() * 20 - goal.y());

    if(dist_x > dist_y)
    {
       return 1.4 * dist_y + (dist_x - dist_y);
    }
    else
    {
       return 1.4 * dist_x + (dist_y - dist_x);
    }
}

bool pathfinding::check_node(path_coordinate & temp, const character * chr) const
{
    // Bypass check if the character is using the Default pathfinding_type
    if (chr->mind()->get_pathfinding_type() == "Default") return true;

    // Analyze the terrain
    world::vector3<s_int32> min_pos(temp.x() * 20, temp.y() * 20, temp.z() - 10);
    world::vector3<s_int32> max_pos(temp.x() * 20 + 20, temp.y() * 20 + 20, temp.z() + 10);

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

    // Update move cost
    temp.moveCost -= temp_terrainCost;
    return true;
}

std::vector<path_coordinate> pathfinding::calc_adjacent_nodes(const node & actual, const character * chr) const
{
    const coordinates & pos = actual.pos;
    std::vector<path_coordinate> temp;
    path_coordinate temp_pos;

    temp.reserve(8);

    s_int32 dx = pos.x() - actual.parent->pos.x();
    s_int32 dy = pos.y() - actual.parent->pos.y();

    if (dx == 0 && dy == 0)
    {
        // north-east
        temp_pos.set(pos.x() + 1, pos.y() + 1, pos.z(), 28);
        if (check_node(temp_pos, chr)) temp.push_back(temp_pos);

        // east
        temp_pos.set(pos.x() + 1, pos.y(), pos.z(), 20);
        if (check_node(temp_pos, chr)) temp.push_back(temp_pos);

        // south-east
        temp_pos.set(pos.x() + 1, pos.y() - 1, pos.z(), 28);
        if (check_node(temp_pos, chr)) temp.push_back(temp_pos);

        // south
        temp_pos.set(pos.x(), pos.y() - 1, pos.z(), 20);
        if (check_node(temp_pos, chr)) temp.push_back(temp_pos);

        // south-west
        temp_pos.set(pos.x() - 1, pos.y() + 1, pos.z(), 28);
        if (check_node(temp_pos, chr)) temp.push_back(temp_pos);

        // west
        temp_pos.set(pos.x() - 1, pos.y(), pos.z(), 20);
        if (check_node(temp_pos, chr)) temp.push_back(temp_pos);

        // north-west
        temp_pos.set(pos.x() - 1, pos.y() - 1, pos.z(), 28);
        if (check_node(temp_pos, chr)) temp.push_back(temp_pos);

        // north
        temp_pos.set(pos.x(), pos.y() + 1, pos.z(), 20);
        if (check_node(temp_pos, chr)) temp.push_back(temp_pos);
    }
    else if (dx != 0 && dy != 0)
    {
        temp_pos.set(pos.x(), pos.y() + dy, pos.z(), 20);
        if (check_node(temp_pos, chr)) temp.push_back(temp_pos);

        temp_pos.set(pos.x() + dx, pos.y() + dy, pos.z(), 20);
        if (check_node(temp_pos, chr)) temp.push_back(temp_pos);

        temp_pos.set(pos.x() + dx, pos.y(), pos.z(), 20);
        if (check_node(temp_pos, chr)) temp.push_back(temp_pos);

        temp_pos.set(pos.x() + dx, pos.y() - dy, pos.z(), 20);
        if (check_node(temp_pos, chr)) temp.push_back(temp_pos);

        temp_pos.set(pos.x() - dx, pos.y() + dy, pos.z(), 20);
        if (check_node(temp_pos, chr)) temp.push_back(temp_pos);
    }
    else if (dx != 0)
    {
        temp_pos.set(pos.x() + dx, pos.y() - 1, pos.z(), 20);
        if (check_node(temp_pos, chr)) temp.push_back(temp_pos);

        temp_pos.set(pos.x() + dx, pos.y(), pos.z(), 20);
        if (check_node(temp_pos, chr)) temp.push_back(temp_pos);

        temp_pos.set(pos.x() + dx, pos.y() + 1, pos.z(), 20);
        if (check_node(temp_pos, chr)) temp.push_back(temp_pos);
    }
    else
    {
        temp_pos.set(pos.x() - 1, pos.y() + dy, pos.z(), 20);
        if (check_node(temp_pos, chr)) temp.push_back(temp_pos);

        temp_pos.set(pos.x(), pos.y() + dy, pos.z(), 20);
        if (check_node(temp_pos, chr)) temp.push_back(temp_pos);

        temp_pos.set(pos.x() + 1, pos.y() + dy, pos.z(), 20);
        if (check_node(temp_pos, chr)) temp.push_back(temp_pos);
    }

    return temp;
}

// calculate number of frames allowed to calculate path
u_int16 pathfinding::init(character *chr, const vector3<s_int32> & goal1, const vector3<s_int32> & goal2)
{
    // Middle position of the goal area
    const vector3<s_int32> goal ((goal1.x() + goal2.x()) / 2, (goal1.y() + goal2.y()) / 2, goal1.z());

    // Verify preconditions
    if (!(((goal.x() >= chr->map().min().x()) && (goal.x() <= chr->map().max().x())) &&
        (goal.y() >= chr->map().min().y()) && (goal.y() <= chr->map().max().y())))
    {
        LOG(WARNING) << "goal " << goal << " is out of the map scope "
                     << chr->map().min() << " - " << chr->map().max();
        return 0;
    }

    // The max number of iterations (may need fine tuning)
    const u_int32 rev_per_dist = 5 + abs(chr->z() - goal.z()) / 40;
    const double max_rev = 1 + rev_per_dist * (abs(chr->x() - goal.x()) + abs(chr->y() - goal.y()));

    // Half length of the character
    const u_int8 chr_length = chr->placeable::length() / 2;
    const u_int8 chr_width = chr->placeable::width() / 2;

    // Grid of the actual_node, centered on character
    const s_int32 grid_x = trunc((chr->x() + chr_length) / 20);
    const s_int32 grid_y = trunc((chr->y() + chr_width) / 20);

    /* -------------------------------------------------------- */

    // Creates and adds the base node to the open list
    node *temp_node = m_nodeBank.get_node();

    temp_node->parent = temp_node; //Loops back to itself
    temp_node->total = 0;
    temp_node->moveCost = 0;
    temp_node->pos.set(grid_x, grid_y, chr->ground_pos());
    temp_node->levelDist = abs(goal.z() - temp_node->pos.z()) / 40;

    temp_node->listAssignedTo = OPEN_LIST;
    m_nodeCache.add_node(temp_node);
    m_openList.add_node(temp_node);

    return ceil(max_rev / MAX_REV_PER_FRAME);
}

bool pathfinding::find_path(character *chr, const vector3<s_int32> & goal1, const vector3<s_int32> & goal2, std::vector<coordinates> * path)
{
    // Middle position of the goal area
    const vector3<s_int32> goal ((goal1.x() + goal2.x()) / 2, (goal1.y() + goal2.y()) / 2, goal1.z());

    // make sure character does not appear as obstacle during pathfinding
    bool is_solid = chr->is_solid();
    chr->set_solid (false);

    // Half length of the character
    const u_int8 chr_length = chr->placeable::length() / 2;
    const u_int8 chr_width = chr->placeable::width() / 2;

    // The node that's been chosen (with the lowest total cost)
    node * actual_node;

    // Temporary variables
    path_coordinate temp_pos;
    node *temp_node;
    node *temp_node2;

    s_int32 grid_x;
    s_int32 grid_y;

    // number of iterations
    u_int32 rev = 0;

    while (!m_openList.is_empty() && (rev < MAX_REV_PER_FRAME))
    {
        // Get the lowest cost node in the Open List
        actual_node = m_openList.get_top();

        // Get the grid of the actual node
        grid_x = actual_node->pos.x();
        grid_y = actual_node->pos.y();
        temp_pos.set(grid_x, grid_y, actual_node->pos.z(), 0);

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

            // clear the node bank, cache and open list
            reset();

            // restore previous solid state
            chr->set_solid (is_solid);
            return true;
        }

        // Change it from the open list to the closed list
        actual_node->listAssignedTo = CLOSED_LIST;

#if 0
        const world::vector3<s_int32> pos = world::area_manager::get_mapview()->get_position();
        s_int16 x = grid_x*20 - pos.x() + gfx::screen::length ()/2;
        s_int16 y = grid_y*20 - pos.y() + gfx::screen::height ()/2 - (actual_node->pos.z() - pos.z());
        gfx::drawing_area da(0, 0, gfx::screen::length (), gfx::screen::height ());
        gfx::screen::get_surface()->draw_line(x,y,x+20,y,0xFFFFFFFF,&da);
        gfx::screen::get_surface()->draw_line(x,y,x,y+20,0xFFFFFFFF,&da);
        gfx::screen::get_surface()->draw_line(x,y+20,x+20,y+20,0xFFFFFFFF,&da);
        gfx::screen::get_surface()->draw_line(x+20,y,x+20,y+20,0xFFFFFFFF,&da);
        base::timer::sleep (50);
        gfx::screen::update ();
#endif

        // Gets a list of positions to visit
        std::vector<path_coordinate> pos_to_visit = calc_adjacent_nodes(*actual_node, chr);

        // Add them to the open list
        std::vector<path_coordinate>::iterator i = pos_to_visit.begin();

        while (i != pos_to_visit.end())
        {
            temp_node = m_nodeBank.get_node();

            temp_node->pos = *i;
            temp_node->parent = actual_node;
            temp_node->moveCost = (*i).moveCost + temp_node->parent->moveCost;
            temp_node->total = calc_heuristics(temp_node->pos, goal) + temp_node->moveCost;

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
                vector3<s_int32> min(temp_node->pos.x() * 20 + 8, temp_node->pos.y() * 20 + 8, temp_node->pos.z() - 20);
                vector3<s_int32> max(temp_node->pos.x() * 20 + 12, temp_node->pos.y() * 20 + 12, temp_node->pos.z());

                std::list<chunk_info *> check_hole = chr->map().objects_in_bbox(min, max, world::OBJECT);
                if (discard_non_solid (check_hole))
                {
                    temp_node->listAssignedTo = CLOSED_LIST;
                    m_nodeCache.add_node(temp_node);

                    ++i;
                    continue;
                }

                // Check if there is an obstacle in this node
                min = vector3<s_int32>(temp_node->pos.x() * 20 + 11 - chr_length, temp_node->pos.y() * 20 + 11 - chr_width, temp_node->pos.z() + 1);
                max = vector3<s_int32>(temp_node->pos.x() * 20 + 9 + chr_length, temp_node->pos.y() * 20 + 9 + chr_width, temp_node->pos.z() + chr->height() - 1);

                std::list<chunk_info *> collisions = chr->map().objects_in_bbox(min, max, world::OBJECT | world::CHARACTER);
                if (!discard_non_solid (collisions))
                {
                    if (!check_stairs (collisions, temp_node))
                    {
#if 0
        const world::vector3<s_int32> pos = world::area_manager::get_mapview()->get_position();
        s_int16 x = i->x()*20 - pos.x() + gfx::screen::length ()/2;
        s_int16 y = i->y()*20 - pos.y() + gfx::screen::height ()/2 - (i->z() - pos.z());
        gfx::drawing_area da(0, 0, gfx::screen::length (), gfx::screen::height ());
        gfx::screen::get_surface()->draw_line(x,y,x+20,y,0xFF0000FF,&da);
        gfx::screen::get_surface()->draw_line(x,y,x,y+20,0xFF0000FF,&da);
        gfx::screen::get_surface()->draw_line(x,y+20,x+20,y+20,0xFF0000FF,&da);
        gfx::screen::get_surface()->draw_line(x+20,y,x+20,y+20,0xFF0000FF,&da);
        base::timer::sleep (50);
        gfx::screen::update ();
#endif

                        ++i;
                        continue;
                    }
                }
                else
                {
                    // update z-position of node
                    temp_node->pos.set_z(get_ground_pos (check_hole, temp_node->pos.x() * 20 + 10, temp_node->pos.y() * 20 + 10));
                }

                // calculate difference between node and goal level
                temp_node->levelDist = abs(goal.z() - temp_node->pos.z()) / 40;

                // Add node to the open list
                temp_node->listAssignedTo = OPEN_LIST;
                m_nodeCache.add_node(temp_node);
                m_openList.add_node(temp_node);
            }

            ++i;
        }

        ++rev;
    }

    // restore previous solid state
    chr->set_solid (is_solid);
    return false;
}

bool pathfinding::discard_non_solid(std::list<chunk_info*> & objects)
{
    std::list<chunk_info*>::iterator ci = objects.begin();
    while (ci != objects.end())
    {
        if ((*ci)->get_object()->is_solid())
        {
            ci++;
        }
        else
        {
            ci = objects.erase (ci);
        }
    }

    return objects.empty();
}

bool pathfinding::check_stairs (std::list<chunk_info*> & ground_tiles, node *current)
{
    ground_tiles.sort(z_order());

    s_int32 level, prev_level = current->pos.z();

    // center of the tile where movement begins
    s_int32 start_x = current->parent->pos.x() * 20 + 10;
    s_int32 start_y = current->parent->pos.y() * 20 + 10;

    // center of the tile where movement ends
    s_int32 end_x = current->pos.x() * 20 + 10;
    s_int32 end_y = current->pos.y() * 20 + 10;

    // number of probes
    s_int8 ox = (end_x - start_x) / 5;
    s_int8 oy = (end_y - start_y) / 5;

    // we skip the start point, because we're already there
    for (int i = 1; i < 10; i++)
    {
        std::list<chunk_info*>::iterator ci = ground_tiles.begin ();
        while (ci != ground_tiles.end())
        {
            // find the tile at given position and get its level
            s_int32 px = start_x - (*ci)->center_min().x();
            s_int32 py = start_y - (*ci)->center_min().y();

            if (px >= 0 && py >= 0 && px <= (*ci)->get_object()->solid_max_length() && py <= (*ci)->get_object()->solid_max_width())
            {
                level = (*ci)->center_min().z() + (*ci)->get_object()->get_surface_pos (px, py);
                break;
            }

            ci++;
        }
        if (ci == ground_tiles.end())
        {
            level = current->pos.z();
        }

        // level differences of 10 or more cannot be scaled by character
        if (level > prev_level + 10) return false;

        prev_level = level;
        start_x += ox;
        start_y += oy;
    }

    // update z-position of current node to actual ground position
    current->pos.set_z(level);
    return true;
}

s_int32 pathfinding::get_ground_pos (std::list<chunk_info*> & ground_tiles, const s_int32 & x, const s_int32 & y)
{
    // sort according to their z-Order
    ground_tiles.sort (z_order());

    // the highest object will be our ground pos
    std::list<chunk_info*>::iterator ci = ground_tiles.begin ();

    // position of character relative to tile
    s_int32 px = x - (*ci)->center_min().x();
    s_int32 py = y - (*ci)->center_min().y();

    // calculate ground position
    return (*ci)->center_min().z() + (*ci)->get_object()->get_surface_pos (px, py);
}
