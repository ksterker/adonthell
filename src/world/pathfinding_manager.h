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

#ifdef __clang__
#include <forward_list>
using std::forward_list;
#define slist forward_list
#else
#if __GNUG__ > 2
#include <ext/slist>
using namespace __gnu_cxx;
#else
#include <slist>
using namespace std;
#endif
#endif // CLANG

#include "pathfinding.h"
#include "pathfinding_task.h"
#include "coordinates.h"
#include "vector3.h"

namespace world
{
    /**
     * This is the class that ought to be called when you want to add a new pathfinding search.
     * It handles all the details. Executes the search, moves the character and handles unexpected
     * collisions with other moving (and ) objects. It also gives you the possibility to
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
        pathfinding_manager ();
        /**
         * Shut down path finding manager.
         */
        ~pathfinding_manager ();
        //@}

        /// Various states a task can have
        typedef enum { SUCCESS = 1, FAILURE = 0, ACTIVE = -1 } state;

        /**
         * Reset to initial state.
         */
        void clear ();
        
        /**
         * Adds a task
         * @param chr the character to be moved
         * @param target the target coordinates
         * @param finalDir the direction the character will have after finishing moving
         * @return the id of the task, which can then be used to pause, resume, etc it
         *         -1 on error
         */
        s_int16 add_task(character * chr, const world::vector3<s_int32> & target,
                                const character::direction & finalDir = character::NONE);
        /**
         * Adds a task
         * @param chr the character to be moved
         * @param target1 the target area's top-rightmost coordinates
         * @param target2 the target area's bottom-leftmost coordinates
         * @param finalDir the direction the character will have after finishing moving
         * @return the id of the task, which can then be used to pause, resume, etc it
         *         -1 on error
         */
        s_int16 add_task(character * chr, const world::vector3<s_int32> & target1,
                                const world::vector3<s_int32> & target2,
                                const character::direction & finalDir = character::NONE);
        /**
         * Adds task
         * @param chr the character to be moved
         * @param target the character to where we will move
         * @param finalDir the direction the character will have after finishing moving
         * @return the id of the task, which can then be used to pause, resume, etc it
         *         -1 on error
         */
        s_int16 add_task(character * chr, character * target,
                                const character::direction & finalDir = character::NONE);

        /**
         * Adds task
         * @param chr the character to be moved
         * @param target the name of the zone to where we will move
         * @param finalDir the direction the character will have after finishing moving
         * @return the id of the task, which can then be used to pause, resume, etc it
         *         -1 on error
         */
        s_int16 add_task(character * chr, const std::string & target,
                                const character::direction & finalDir = character::NONE);

        /**
         * Adds a callback to the task that will return failure or success on completion
         * of the task. Note that the callback will not be saved, so it will have to be
         * reset after loading the entity that has an active pathfinding task.
         *
         * @param id the id of the task the callback is added to
         * @param callback the callback to run on task completion or
         *        NULL to clear a previously set callback.
         */
        void set_callback (const s_int16 & id, base::functor_1<const s_int32> * callback);

        /**
         * Set the direction to where the character will point after finishing moving
         * @param id the id of the task to be altered
         * @param finalDir the direction the character will point after finishing moving
         */
         void set_final_direction(const s_int16 & id, const character::direction finalDir);

        /**
         * Pauses a task
         * @param the id of the task to be paused
         */
        void pause_task(const s_int16 & id);

        /**
         * Resumes a paused task
         * @param id the id of the paused task to be resumed
         */
        void resume_task(const s_int16 & id);

        /**
         * Deletes a task. When deleting a task the slot it used will be freed and open to reuse,
         * whereas when pausing, the slot will remain intact and blocked.
         * @param id the id of the task to be deleted
         * @return \b true on success, \b false otherwise
         */
        bool delete_task(const s_int16 & id);

        /**
         * Returns the state of the task. Can be useful for knowing whether the target has been
         * reached or not.
         * @param id the id of the task
         * @return the state
         */
        state return_state(const s_int16 & id);

        /**
         * Return pathfinding task.
         * @param id the id of the task
         * @return the task, or NULL if id is invalid
         */
        const pathfinding_task* get_task(const s_int16 & id);

        /**
         * Updates every task as needed.
         * HAS to be called every frame.
         */
        void update();

        /**
         * Save state to stream
         * @param file stream to save to
         */
        void put_state(base::flat & file);

        /**
         * Load state from stream.
         * @param file stream to load from.
         */
        void get_state(base::flat & file);

        /**
         * Save state to file
         * @param fname file to save to
         */
        bool load(std::string & fname);

        /**
         * Load state from stream
         * @param fname stream to load from
         */
        bool save(std::string & fname);

    private:
        /**
         * Handles the low-level stuff of adding tasks
         * @param id the task number
         * @param chr the character for which we find the task
         * @param target the lower corner of the target area
         * @param target2 the upper corner of the target area
         * @param finalDir the direction the character should face when reaching the goal
         * @return \b false on error, \b true on success
         */
        bool add_task_ll(const s_int16 id, character * chr,
                         const world::vector3<s_int32> & target,
                         const world::vector3<s_int32> & target2,
                         const character::direction & finalDir);

        /**
         * Verify if we can add the task and in which slot
         * @return a free slot where we can add the task or -1 of
         *  no free slot available.
         */
        s_int16 find_free_task();

        /**
         * Handles the movement of the character
         * @param id of the task
         * @return \b true when all the movement has finished, \b false otherwise
         * @note when \b true the task has done everything and will delete itself
         */
        bool move_chr(const s_int16 id);

        bool is_blocked(const world::coordinates & pos, world::character * chr) const;

        /// A vector with the tasks
        std::vector<world::pathfinding_task*> m_task;

        /// Highest slot in use
        s_int16 m_taskHighest;

        /// A vector that quickly tells when a slot is locked(under use), or unlocked(free to be used)
        std::vector<bool> m_locked;

        /// A list containing all the characters in movement
        slist<world::character *> m_chars;
    };
}

#endif // PATHFINDING_MANAGER_H
