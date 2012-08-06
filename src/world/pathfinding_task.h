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
 * @file   world/pathfinding_task.h
 * @author Frederico Cerveira <frederico.cerveira@gmail.com>
 *
 * @brief  Declares the pathfinding_task class.
 *
 */

#ifndef PATHFINDING_TASK_H
#define PATHFINDING_TASK_H

#include <adonthell/base/callback.h>
#include "character.h"
#include "coordinates.h"

namespace world
{
    /**
     * Holds the data about a current pathfinding task
     */
    class pathfinding_task
    {
    public:
        pathfinding_task()
        {
            chr = NULL;
            callback = NULL;
        }

        /// The character being moved
        world::character * chr;
        /// Callback to execute when task completes/fails
        base::functor_1<const s_int32> * callback;
        /// The lower position of the goal area (in pixels)
        world::vector3<s_int32> target;
        /// The upper position of the goal area (in pixels)
        world::vector3<s_int32> target2;

        /// The path to the target, as a group of nodes
        std::vector<coordinates> path;
        /// The character's position in the last frame
        world::coordinates lastPos;
        /// The phase where this task is
        u_int8 phase;
        /// Number of iterations left to calculate the path
        u_int16 iterations;
        /// The actual node being moved to
        u_int16 actualNode;
        /// Actual Direction
        u_int8 actualDir;
        /// Final Direction to where a character points after finishing moving
        u_int8 finalDir;
        /// Executes the search
        world::pathfinding m_pathfinding;
    private:
        /// forbid copy construction
        pathfinding_task (const pathfinding_task & t);
    };
}

#endif // PATHFINDING_TASK_H
