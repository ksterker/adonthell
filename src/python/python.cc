/*
   $Id: python.cc,v 1.2 2003/07/24 12:57:59 gnurou Exp $

   Copyright (C) 2003  Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
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
        if ( PyErr_Occurred() )
        {
            PyErr_Print();
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
