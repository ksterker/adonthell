/*
   $Id: python.cc,v 1.1 2003/07/18 15:16:09 gnurou Exp $

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

void python::show_traceback()
{
    if ( PyErr_Occurred() )
    {
        PyErr_Print();
        fflush (stderr);
    }
}

void python::init()
{
    Py_Initialize();
}

void python::cleanup()
{
    Py_Finalize();
}

bool python::add_search_path(const std::string & path)
{
    std::string buf = "import sys; sys.path.insert(0, \"";
    buf += path;
    buf += "\")";

    return run_string(buf);
}

bool python::run_string(const std::string & statements)
{
    bool ret = PyRun_SimpleString((char *)statements.c_str());

    show_traceback();
    return ret;
}

PyObject * python::import_module(const std::string & name)
{
    PyObject * ret = PyImport_ImportModule((char *)name.c_str());

    show_traceback();
    return ret;
}
