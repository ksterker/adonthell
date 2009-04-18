/*
  $Id: pathfinding_manager.h,v 1.2 2009/04/18 21:54:59 ksterker Exp $

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
 * @file   world/pathfinding_manager.h
 * @author Frederico Cerveira <frederico.cerveira@gmail.com>
 *
 * @brief  Declares the pathfinding_manager class
 */

#ifndef PATHFINDING_MANAGER_H
#define PATHFINDING_MANAGER_H

#if __GNUG__ > 2
#include <ext/slist>
using namespace __gnu_cxx;
#else
#include <slist>
using namespace std;
#endif
#include "world/pathfinding.h"
#include "world/pathfinding_task.h"
#include "world/coordinates.h"
#include "world/vector3.h"

namespace world
{
    /**
     * This is the class that ought to be called when you want to add a new pathfinding search.
     * It handles all the details. Executes the search, moves the character and handles unexpected
     * collisions with other moving (and static) objects. It also gives you the possibility to
     * pause, resume, delete and return the state of an ongoing search(known as task).
     */
    class pathfinding_manager
    {
    public:
        /**
         * @name Initialization and cleanup.
         */
        //@{
        /**
         * Init path finding manager.
         */
        static void init();
        /**
         * Shut down path finding manager.
         */
        static void cleanup();
        //@}
        
        /// Various states a task can have
        typedef enum { SUCCESS, FAILURE, ACTIVE } state;

        /**
         * Adds a task
         * @param the character to be moved, the target coordinates
         * @return the id of the task, which can then be used to pause, resume, etc it
         *         -1 on error
         */
        static s_int16 add_task(character * chr, const world::vector3<s_int32> & target);

        /**
         * Pauses a task
         * @param the id of the task to be paused
         */
        static void pause_task(const s_int16 id);

        /**
         * Resumes a paused task
         * @param the id of the paused task to be resumed
         */
        static void resume_task(const s_int16 id);

        /**
         * Deletes a task. When deleting a task the slot it used will be freed and open to reuse,
         * whereas when pausing, the slot will remain intact and blocked.
         * @param the id of the task to be deleted
         * @return \b true on success, \b false otherwise
         */
        static bool delete_task(const s_int16 id);

        /**
         * Returns the state of the task. Can be useful for knowing wether the target has been
         * reached or not.
         * @param the id of the task
         * @return the state
         */
        static state return_state(const s_int16 id);

        /**
         * Updates every task as needed.
         * HAS to be called every frame.
         */
        static void update();

        /**
         * Save state to stream
         * @param stream to save to
         */
        static void put_state(base::flat & out);

        /**
         * Load state from stream
         * @param stream to load from, area where the character are supposed to exist (ie: actual map)
         */
        static void get_state(base::flat & in, world::area & map);

        /**
         * Save state to file
         * @param file to save to
         */
        static bool load(std::string & fname, world::area & map);

        /**
         * Load state from stream
         * @param stream to load from, area where the character are supposed to exist (ie: actual map)
         */
        static bool save(std::string & fname);

    private:
        /// forbid instantiation of static class
        pathfinding_manager () {}
        
        /**
         * Cleans everything
         */
        static void reset();

        /**
         * Handles the low-level stuff of adding tasks
         * @param all the necessary stuff
         */
        static void add_task_sp(const s_int16 id, character * chr, const world::vector3<s_int32> & target,
                         const u_int8 phase, const u_int8 actualNode, const u_int8 actualDir,
                         const u_int8 pixMoved = 0, const u_int8 pixToMove = 0);

        /**
         * Handles the movement of the character
         * @param id of the task
         * @return \b true when all the movement has finished, \b false otherwise
         * @note when \b true the task has done everything and will delete itself
         */
        static bool move_chr(const s_int16 id);

        /**
         * Calcs the 2D euclidean distance from 2 points
         * @param the coordinates of the a node, the character
         * @return the distance
         * @note the return value is of type u_int8 because the distance shouldn't
         * exceed 29 (on a 20x20 grid)
         */
        static u_int8 calc_distance(const world::coordinates & node, const world::character * chr);

        /// A vector with the tasks
        static std::vector<world::pathfinding_task> m_task;

        /// Task slot
        static s_int16 m_taskCount;

        /// A vector that quickly tells when a slot is locked(under use), or unlocked(free to be used)
        static std::vector<bool> m_locked;

        /// A list containing all the characters in movement
        static slist<world::character *> m_chars;

        /// Executes the searchs
        static world::pathfinding m_pathfinding;
    };
}

#endif // PATHFINDING_MANAGER_H
