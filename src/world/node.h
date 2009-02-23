/*
  $Id: node.h,v 1.1 2009/02/23 12:46:05 fr3dc3rv Exp $

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
 * @brief  Declares the node class
 */

#ifndef WORLD_NODE_H
#define WORLD_NODE_H
#include "world/chunk_info.h"
#include "world/coordinates.h"

namespace world
{
    /**
    * Container used to store Pathfinding info
    */

    class node
    {
    public:
        /// The total cost to move from one place to another
        s_int16 total;
        /// The cost of moving from the actual node to this one
        u_int8 moveCost;
        /// The list to which this node is assigned
        u_int8 listAssignedTo; // 0 - None, 1 - Open List, 2 - Closed List

        /// Its parent
        node * parent;
        /// The pixel position of this node
        coordinates pos;

    };

}


#endif // WORLD_NODE_H
