/*
   $Id: python.cc,v 1.4 2003/11/22 09:38:09 ksterker Exp $

   Copyright (C) 2003  Alexandre Courbot <alexandrecourbot@linuxgames.com>
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
 * @file   python.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines some common Python functions.
 * 
 * 
 */

#include "python/python.h"

namespace python
{
    void show_traceback()
    {
        if (PyErr_Occurred ())
        {
            PyErr_Print ();
            fflush (stderr);
        }
    }

    void init()
    {
        Py_Initialize();
    }

    void cleanup()
    {
        Py_Finalize();
    }
    
    bool add_search_path(const std::string & path)
    {
        std::string buf = "import sys; sys.path.insert(0, \"";
        buf += path;
        buf += "\")";
        
        return run_string(buf);
    }

    bool run_string(const std::string & statements)
    {
        bool ret = PyRun_SimpleString((char *)statements.c_str());
        
        show_traceback();
        return ret;
    }

    PyObject * import_module(const std::string & name)
    {
        PyObject * ret = PyImport_ImportModule((char *)name.c_str());
        
        show_traceback();
        return ret;
    }
}
