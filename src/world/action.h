/*
 Copyright (C) 2010 Kai Sterker <kaisterker@linuxgames.com>
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
 along with Adonthell; if not, write to the Free Software 
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/**
 * @file world/action.h
 *
 * @author Kai Sterker
 * @brief Declares the object interaction class.
 */

#ifndef WORLD_ACTION_H
#define WORLD_ACTION_H

#include "python/method.h"

/**
 * Path to the object interaction scripts.
 */
#define ACTION_DIR "schedules.obj."

namespace world
{
    class character;
    class object;
    
    /**
     * This class is used to associate an action with a scenery
     * object. The action itself is implemented as the method of
     * a python script. Since the method is loaded from the python::pool,
     * it should be stateless as it is probably shared by more than
     * one object.
     */
    class action
    {
    public:
        /**
         * Create a new, empty action.
         */
        action ();
        
        /**
         * Destructor.
         */
        ~action ();
        
        /**
         * Initialize the action.
         * @param script name of file inside the action directory.
         * @param method name of the method implementing the action.
         * @param args optional arguments when calling the method.
         * @return true on success, false otherwise.
         */
        bool init (const std::string & script, const std::string & method, PyObject *args = NULL);
        
#ifndef SWIG
        /**
         * Execute the action with the given character and object.
         * @param actor the character triggering the action.
         * @param target the object acted on.
         */
        void execute (world::character *actor, world::object *target);
#endif
        
        /**
         * Execute the action with the given character and object.
         *
         * This method is optimized for being called from a Python
         * script, as it avoids boxing and unboxing of its arguments.
         *
         * @param actor the character triggering the action.
         * @param target the object acted on.
         */
        void execute (PyObject *actor, PyObject *target);
        
        /**
         * @name Loading / Saving
         */
        //@{
        /** 
         * Save the %action to a stream.
         * @param file stream where to save the %action.
         */ 
        void put_state (base::flat& file) const;
        
        /** 
         * Loads the %action from a stream.
         * @param file stream to load the %action from.
         * @return \e true if the %schedule was loaded successfully, \e false otherwise.
         */
        bool get_state (base::flat& file);
        //@}
        
#ifndef SWIG
        /// allow %action to be passed through SWIG
        GET_TYPE_NAME(world::action);
#endif // SWIG
        
    private:
        /**
         * Reset the action.
         */
        void clear ();
        
        /// Forbid copy constructor
        action (const action & a);
        /// the action implementation
        python::method *Action;
        /// additional arguments for the action
        PyObject *Args;
    };
}

#endif
