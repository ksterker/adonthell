/*
   $Id: pool.h,v 1.1 2003/12/01 22:42:21 ksterker Exp $

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
     *
     */
    class pool
    {
        public:
            /**
             *
             */
            static void init ();

            /**
             *
             */
            static void cleanup ();
            
            /**
             *
             */
            static python::method * connect (const std::string & file, 
                const std::string & classname, const std::string & callback);
            
            /**
             *
             */
            static python::script * reconnect (const std::string & file, 
                const std::string & classname);
                
        private:
            /**
             *
             */
            static std::map<const char*, python::script*> Pool;
    };
}

#endif // PYTHON_POOL_H
