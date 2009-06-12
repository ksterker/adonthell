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
        typedef enum { SUCCESS = 1, FAILURE = 0, ACTIVE = -1 } state;

        /**
         * Adds a task
         * @param chr the character to be moved
         * @param target the target coordinates
         * @param finalDir the direction the character will have after finishing moving
         * @return the id of the task, which can then be used to pause, resume, etc it
         *         -1 on error
         */
        static s_int16 add_task(character * chr, const world::vector3<s_int32> & target,
                                const character::direction finalDir = character::NONE);
        /**
         * Adds a task
         * @param chr the character to be moved
         * @param target1 the target area's top-rightmost coordinates
         * @param target2 the target area's bottom-leftmost coordinates
         * @param finalDir the direction the character will have after finishing moving
         * @return the id of the task, which can then be used to pause, resume, etc it
         *         -1 on error
         */
        static s_int16 add_task(character * chr, const world::vector3<s_int32> & target1,
                                const world::vector3<s_int32> & target2,
                                const character::direction finalDir = character::NONE);
        /**
         * Adds task
         * @param chr the character to be moved
         * @param target the character to where we will move
         * @param finalDir the direction the character will have after finishing moving
         * @return the id of the task, which can then be used to pause, resume, etc it
         *         -1 on error
         */
        static s_int16 add_task(character * chr, character * target,
                                const character::direction finalDir = character::NONE);

        /**
         * Adds task
         * @param chr the character to be moved
         * @param target the name of the zone to where we will move
         * @param finalDir the direction the character will have after finishing moving
         * @return the id of the task, which can then be used to pause, resume, etc it
         *         -1 on error
         */
        static s_int16 add_task(character * chr, std::string & target,
                                const character::direction finalDir = character::NONE);

        /**
         * Adds a callback to the task that will return failure or success on completion
         * of the task. Note that the callback will not be saved, so it will have to be
         * reset after loading the entity that has an active pathfinding task.
         *
         * @param id the id of the task the callback is added to
         * @param callback the callback to run on task completion or
         *        NULL to clear a previously set callback.
         */
        static void set_callback (const s_int16 id, base::functor_1<s_int32> * callback);

        /**
         * Set the direction to where the character will point after finishing moving
         * @param id the id of the task to be altered
         * @param finalDir the direction the character will point after finishing moving
         */
        static void set_final_direction(const s_int16 id, const character::direction finalDir);

        /**
         * Pauses a task
         * @param the id of the task to be paused
         */
        static void pause_task(const s_int16 id);

        /**
         * Resumes a paused task
         * @param id the id of the paused task to be resumed
         */
        static void resume_task(const s_int16 id);

        /**
         * Deletes a task. When deleting a task the slot it used will be freed and open to reuse,
         * whereas when pausing, the slot will remain intact and blocked.
         * @param id the id of the task to be deleted
         * @return \b true on success, \b false otherwise
         */
        static bool delete_task(const s_int16 id);

        /**
         * Returns the state of the task. Can be useful for knowing wether the target has been
         * reached or not.
         * @param id the id of the task
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
         * @param file stream to save to
         */
        static void put_state(base::flat & file);

        /**
         * Load state from stream
         * @param file stream to load from
         * @param map area where the character are supposed to exist (ie: actual map)
         */
        static void get_state(base::flat & file, world::area & map);

        /**
         * Save state to file
         * @param fname file to save to
         * @param map area where the character are supposed to exist (ie: actual map)
         */
        static bool load(std::string & fname, world::area & map);

        /**
         * Load state from stream
         * @param fname stream to load from
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
        static void add_task_ll(const s_int16 id, character * chr, const world::vector3<s_int32> & target,
                         const world::vector3<s_int32> & target2,
                         const u_int8 phase, const u_int8 actualNode, const u_int8 actualDir,
                         const u_int8 pixMoved = 0, const u_int8 pixToMove = 0);

        /**
         * Verify if we can add the task and in which slot
         * @param chr the character
         * @return a free slot where we can add the task
         */
        static s_int16 add_task_sec(const character * chr);

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
