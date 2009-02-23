/*
  $Id: node_cache.h,v 1.1 2009/02/23 12:46:05 fr3dc3rv Exp $

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

#if __GNUG__ > 2
#include <ext/hash_map>
using namespace __gnu_cxx;
#else
#include <hash_map>
using namespace std;
#endif

#include "world/node.h"
#include "world/chunk_info.h"
#include "world/coordinates.h"
#include "base/hash_map.h"

namespace world
{
    struct eqstr
    {
        bool operator()(std::string s1, std::string s2) const
        {
            return (s1 == s2);
        }
    };

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
        void add_node(const node * nd)
        {
            std::stringstream ss;
            std::string temp;
            ss << nd->pos.x() << ":" << nd->pos.y();
            ss >> temp;

            m_usedNodes[temp] = nd;
        }

       /**
        * Checks wether a node already exists
        * @param the node to be checked
        * @return NULL if it doesn't exists, itself otherwise
        */
        node * search_node(const node * nd)
        {

            std::stringstream ss;
            std::string temp;
            ss << nd->pos.x() << ":" << nd->pos.y();
            ss >> temp;

            return const_cast<node *>(m_usedNodes[temp]);
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

        /// The hash map
        hash_map<std::string, const node *, hash<std::string>, eqstr> m_usedNodes;

    };
}

#endif // WORLD_NODE_CACHE_H
