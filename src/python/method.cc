/*
   $Id: method.cc,v 1.1 2003/12/01 22:42:21 ksterker Exp $

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
 * @file   python/method.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Encapsulate Python method calls.
 */

#include "python/method.h"
#include "python/pool.h"

using python::method;

method::method (python::script *scrpt, const std::string & mtd)
{
    Script = scrpt;
    init (mtd);
}

void method::init (const std::string & mtd)
{
    // since the method will most likely get called more often
    Method = Script->get_attribute (mtd);
    if (!PyCallable_Check (Method))
    {
        fprintf (stderr, "*** python::method init: method %s.%s not found!\n", 
            Script->class_name ().c_str (), mtd.c_str ());
        Method = NULL;
    }
}

method::~method ()
{
    Py_XDECREF (Method);
}

bool method::execute (PyObject *args)
{
    if (Method) 
    {
        PyObject *result = PyObject_CallObject (Method, args);
        if (result) 
        {
            Py_DECREF (result);
            return true;
        }

        // if we end up here, something has gone utterly wrong
        show_traceback ();
    }
    return false;
}

/*
void method::put_state (ogzstream & file)
{
    if (!Script) 
    {
        false >> file;
        return;
    }
    else true >> file;
    
    Script->file_name () >> file;
    Script->class_name () >> file;
    PyModule_GetName (Method) >> file;
}
*/

/*
bool method::get_state (igzstream & file)
{
    bool connected;
    
    connected << file;
    if (connected)
    {
        std::string filename, classname, methodname;

        filename << file;
        classname << file;
        methodname << file;
        
        Script = pool::reconnect (filename, classname);
        if (!Script) return false;
        
        init (methodname);
        if (!Method) return false;
    }
    
    return true;
}
*/
