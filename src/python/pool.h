/*
   $Id: pool.h,v 1.3 2004/11/15 08:54:33 ksterker Exp $

   Copyright (C) 2003 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file   python/pool.h
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Allow sharing of python class instances between multiple objects.
 */

#ifndef PYTHON_POOL_H
#define PYTHON_POOL_H

#include <map>
#include "python/method.h"

namespace python
{
    /**
     * A cache for python scripts that allows multiple objects to access the
     * same python instance. This avoids multiple instanciation of python
     * objects that could easily be shared.
     */
    class pool
    {
        public:
            /**
             * Initialize the %pool. This is currently a no-op.
             */
            static void init ();

            /**
             * Remove all scripts contained in the %pool.
             */
            static void cleanup ();
            
            /**
             * Return a python method from the given script and class. If the
             * class is not yet cached in the pool, it is instanciated.
             * @param file name of the python script.
             * @param classname name of the python class to access.
             * @param callback name of the method to retrieve from that class.
             * @return python method from cached object or \c NULL on error.
             */
            static python::method * connect (const std::string & file, 
                const std::string & classname, const std::string & callback);
            
            /**
             * Return a python object from the given script. If it is not
             * yet cached, the script will be loaded and the given class
             * will be instanciated.
             * @param file name of the python script.
             * @param classname name of the python class to access.
             * @return python object from cache or \c NULL on error.
             */
            static python::script * reconnect (const std::string & file, 
                const std::string & classname);
                
        private:
            /// All python scripts currently kept in cache
            static std::map<const char*, python::script*> Pool;
    };
}

#endif // PYTHON_POOL_H
