/*
  $Id: node_bank.h,v 1.1 2009/02/23 12:46:05 fr3dc3rv Exp $

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
#include "world/node.h"

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
            m_freeNodes.reserve(START_NODES);
            m_availableNodes = START_NODES;
        }

        /**
         * Gets a new node
         * @return the node
         */
        node * get_node()
        {
            ++m_nodesTaken;

            //Verify if we've exceeded the available free nodes
            if (m_nodesTaken > m_availableNodes)
            {
                m_availableNodes += REALLOC_NODES;
                m_freeNodes.resize(m_availableNodes); //Add more free nodes
            }

            return &m_freeNodes[m_nodesTaken - 1];
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

        /// Constants regarding the pre-allocated nodes
        static const u_int16 START_NODES = 200;
        static const u_int16 REALLOC_NODES = 60;

        /// A vector holding the nodes
        vector<node> m_freeNodes;

        /// Counts how many nodes were taken and how many are free
        u_int16 m_nodesTaken, m_availableNodes;
    };
}

#endif // WORLD_NODE_BANK_H
