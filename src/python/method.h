/*
   $Id: method.h,v 1.3 2004/04/09 11:57:51 ksterker Exp $

   Copyright (C) 2003/2004 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file   python/method.h
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Encapsulate Python method calls.
 */

#ifndef PYTHON_METHOD_H
#define PYTHON_METHOD_H

#include "python/script.h"
#include "base/file.h"

namespace python
{
    /**
     *
     */
    class method
    {
    public:
#ifndef SWIG
        /**
         * Standard constructor.
         */
        method () : Script (NULL), Method (NULL) { }
#endif // SWIG

        /**
         * Constructor.
         * @param scrpt Python %script this 5method belongs to.
         * @param mtd name of the %method to connect to.
         */
        method (python::script *scrpt, const std::string & mtd);
        
        /**
         * Destructor.
         */
        ~method ();
        
        /**
         * Execute the connected %method with the given arguments.
         * @param args a python tuple to be passed to the %method.
         */
        bool execute (PyObject *args);

        /**
         * @name Loading / Saving
         */
        //@{

        /** 
         * Saves all information required to re-connect this method to its class.
         * That is, filename of the Python script, name of the class and finally
         * the name of the method itself.
         * 
         * @param out stream where to save the %method.
         */ 
        void put_state (base::flat& out) const;
    
        /** 
         * Re-connect this method to its script. This is done via the %script
         * pools reconnect method.
         *
         * @param in stream to load the %method from.
         * @return \b true if the %method could be reconnected, \b false otherwise
         */
        bool get_state (base::flat& in);
        //@}
        
    private:
        /**
         *
         */
        void init (const std::string & mtd);
        
        /**
         *
         */
        python::script *Script;

        /**
         *
         */
        PyObject *Method;
    };
}
#endif // PYTHON_METHOD_H
