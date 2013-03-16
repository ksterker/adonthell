/*
 Copyright (C) 2009 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file   base/serializer.h
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Gamedata serialization suppert.
 * 
 * 
 */

#ifndef BASE_SERIALIZER_H
#define BASE_SERIALIZER_H

#include "callback.h"
#include <adonthell/python/callback_support.h>

namespace base
{
    /**
     * Wrapper for anything that can be serialized to a file on disk.
     * It supports static classes, instantiated objects and Python
     * classes, as long as those provide the two methods
     *
     * \li bool load()
     * \li bool save(const std::string & path)
     *
     * Using this wrapper, objects can be registered at the gamedata
     * class, which will load or save their contents as appropriate.
     */
    class serializer_base
    {
    public:
        /**
         * Cleanup.
         */
        ~serializer_base ()
        {
            delete Save;
            delete Load;
            delete Cleanup;
        }
        
        /**
         * Serialize object into the given directory. Which filename
         * to use is up to the object that is being serialized.
         * @param path directory to write into.
         * @return true on success, false otherwise.
         */
        bool save (const std::string & path)
        {
            if (!Save) return false;
            return (*Save)(path);
        }
        
        /**
         * Load object from a file. Which file to load is up to the
         * object itself. It must search for that file in Adonthell's
         * search path.
         * @return true on success, false otherwise.
         */
        bool load ()
        {
            if (!Load) return false;
            return (*Load)();
        }

        void cleanup ()
        {
            if (Cleanup)
                (*Cleanup)();
        }

#ifndef SWIG
        /// python support
        GET_TYPE_NAME(base::serializer_base);
#endif
        
    protected:
        /**
         * Prevent instantiation of this abstract base class through
         * the user.
         */
        serializer_base()
        {
            Save = NULL;
            Load = NULL;
            Cleanup = NULL;
        }
        
        /// callback for saving the wrapped object
        base::functor_1ret<const std::string &, bool> *Save;
        /// callback for loading the wrapped object
        base::functor_0ret<bool> *Load;
        /// callback for cleanup of the wrapped object
        base::functor_0ret<void> *Cleanup;
    };
    
    /**
     * Wrapper around static or instantiated class. Called
     * py_serializer on Python side (see base.i for specialization).
     * This class can be instantiated to register at the savegame
     * class.
     */
    template <class T>
    class serializer : public serializer_base
    {
    public:
        /**
         * Create a serializer for a static class.
         */
        serializer()
        {
            Save = base::make_functor_ret(&T::save);
            Load = base::make_functor_ret(&T::load);
            Cleanup = base::make_functor_ret(&T::cleanup);
        }
    
        /**
         * Create a serializer for the given instance. Note 
         * that the serializer knows nothing about the instance's
         * livetime. So make sure to deregister the serializer
         * once the instance gets deleted.
         * @param instance object to serialize.
         */
        serializer(T *instance)
        {
            Save = base::make_functor_ret(*instance, &T::save);
            Load = base::make_functor_ret(*instance, &T::load);            
            Cleanup = base::make_functor_ret(*instance, &T::cleanup);
        }
    };

}

#endif
