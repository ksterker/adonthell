/*
  $Id: node_cache.h,v 1.2 2009/04/09 14:43:18 fr3dc3rv Exp $

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
 * @brief  Declares the node cache class
 */

#ifndef WORLD_NODE_CACHE_H
#define WORLD_NODE_CACHE_H

#include "node.h"
#include "chunk_info.h"
#include "coordinates.h"
#include <adonthell/base/hash_map.h>

namespace world
{
    /**
     * Keeps a hash map of every used node.
     */
    class node_cache
    {
    public:

       /**
        * Adds a node to the hash map
        * @param the node to be added
        */
        void add_node(node * nd)
        {
            std::stringstream ss;
            std::string temp;
            ss << nd->pos.x() << ":" << nd->pos.y() << ":" << nd->pos.z();
            ss >> temp;

            m_usedNodes[temp] = nd;
        }

       /**
        * Checks whether a node already exists
        * @param the node to be checked
        * @return NULL if it doesn't exists, itself otherwise
        */
        node * search_node(const node * nd)
        {
            std::stringstream ss;
            std::string temp;
            ss << nd->pos.x() << ":" << nd->pos.y() << ":" << nd->pos.z();
            ss >> temp;

            return m_usedNodes[temp];
        }

       /**
        * Clears the hash map
        * @note MUST be called after the end of every pathfinding search
        */
        void reset()
        {
            m_usedNodes.clear();
        }

    private:

        typedef std::hash_map<std::string, node *> nodeHash;
        /// The hash map of nodes already in the path
        nodeHash m_usedNodes;
    };
}

#endif // WORLD_NODE_CACHE_H
