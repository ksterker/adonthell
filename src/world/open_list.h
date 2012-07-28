/*
  $Id: open_list.h,v 1.2 2009/04/09 14:43:18 fr3dc3rv Exp $

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
 * @brief  Declares the open list class
 */

#ifndef WORLD_OPEN_LIST_H
#define WORLD_OPEN_LIST_H
#include "node.h"

using namespace std;

namespace world
{
    struct cmp
    {
        bool operator()(node * s1, node * s2) const
        {
            return (s1->total > s2->total);
        }
    };

    /**
     * Priority queue holding the nodes in the open list
     */
    class open_list
    {
    public:

        open_list()
        {
            m_list.reserve(INITIAL_SIZE);
        }

        /**
         * Adds a node to the open list
         * @return /b true if everything went ok, /b false on error
         * @note the error message will be sent to stderr
         * @param the node
         */
        void add_node(node * nd)
        {
            verify_capacity();

            m_list.push_back(nd);
            push_heap(m_list.begin(), m_list.end(), cmp());
        }

        /**
         * Gets the node with the lower total cost
         * @return node
         */
        node * get_top()
        {

            if (m_list.empty())
                return NULL;

            node * temp = m_list.front();

            pop_heap(m_list.begin(), m_list.end(), cmp());
            m_list.pop_back();

            return temp;
        }

        /**
         * Rebalances the position of a node in the priorty queue,
         * according to the alterations done to its total cost
         * @param the node to be updated
         */
        void rebalance_node(node * nd)
        {
            // O(log(N) + N)

            verify_capacity();

            vector<node *>::iterator i = m_list.begin();

            while (i != m_list.end())
            {
                if (*i == nd)
                {
                    push_heap(m_list.begin(), i + 1, cmp());
                }

                ++i;
            }

        }

        /**
         * Check if the priority queue is empty
         * @return \b true if it is, \b false otherwise
         */
        bool is_empty()
        {
            return m_list.empty();
        }

        /**
         * Clears the priority queue
         * @note MUST be called after the end of every pathfinding search
         */
        void reset()
        {
            m_list.clear();
        }

    private:

        /**
         * Verifies if the maximum capacity as been exceded and then resizes
         * the vector to accomodate more nodes
         */
        void verify_capacity()
        {
            if (m_list.size() == m_list.capacity() - 1)
            {
                safe_resize(m_list.capacity() + REALLOC_SIZE);
            }

        }

        /**
         * Resizes the open list
         */
        void safe_resize(u_int16 n)
        {
            m_list.reserve(n);
        }

        /// Priority queue capacity constants
        static const u_int16 INITIAL_SIZE = 200;
        static const u_int8 REALLOC_SIZE = 60;

        /// The priority queue, implemented using a vector
        vector<node *> m_list;
    };
}

#endif // WORLD_OPEN_LIST_H
