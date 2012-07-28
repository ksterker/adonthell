/*
  $Id: node_bank.h,v 1.3 2009/04/12 09:10:37 ksterker Exp $

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
 * @brief  Declares the node bank class
 */

#ifndef WORLD_NODE_BANK_H
#define WORLD_NODE_BANK_H
#include "node.h"

using namespace std;

namespace world
{
    /**
     * A bank where all the nodes are hold
     * When you want a free node you have to ask it to this
     * class
     * @note the content of the nodes returned is not guaranteed
     *       to be empty, however it is guaranteed that any change
     *       you do to the node won't have any negative effect.
     */
    class node_bank
    {
    public:

        node_bank()
        {
            m_nodesTaken = 0;
            m_freeNodes.reserve(MAX_NODES);

            for (u_int16 i = 0; i < MAX_NODES; i++)
            {
                node * nd = new node;
                m_freeNodes.push_back(nd);
            }
        }

        ~node_bank()
        {
            std::vector<node *>::iterator i = m_freeNodes.begin();

            while (i != m_freeNodes.end())
            {
                delete *i;
                *i = NULL;

                ++i;
            }
        }

        /**
         * Gets a new node
         * @return the node
         */
        node * get_node()
        {
            verify_capacity();

            return m_freeNodes[m_nodesTaken++];
        }

        /**
         * Resets everything to zero
         * @note MUST be called after the end of every pathfinding search
         */
         void reset()
         {
             m_nodesTaken = 0;
         }

    private:

        /**
         * Verifies if the maximum capacity as been exceded and then resizes
         * the vector to accomodate more nodes
         */
         void verify_capacity()
         {
            if (m_nodesTaken == m_freeNodes.capacity() - 1)
                safe_resize(m_freeNodes.capacity() + REALLOC_NODES);
         }

        /**
         * Safely resizes the node vector
         * @param capacity after resize
         */
        void safe_resize(u_int16 n)
        {
            vector<node *> temp;
            vector<node *>::iterator i = m_freeNodes.begin();

            while (i != m_freeNodes.end())
            {
                temp.push_back(*i);
                ++i;
            }

            m_freeNodes.reserve(n);
            // Do I need to clear
            m_freeNodes.clear();

            for (u_int16 a = 0; a < n; a++)
            {
                if (a < temp.size())
                    m_freeNodes.push_back(temp.at(a));
                else {
                    node * nd = new node;
                    m_freeNodes.push_back(nd);
                }
            }

        }



        /// Constants regarding the allocation of new nodes
        static const u_int16 REALLOC_NODES = 60;
        static const u_int16 MAX_NODES = 20;

        /// A vector holding the nodes
        vector<node *> m_freeNodes;

        /// The last node in used
        u_int16 m_nodesTaken;
    };
}

#endif // WORLD_NODE_BANK_H
