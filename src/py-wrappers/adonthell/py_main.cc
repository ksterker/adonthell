/*
   $Id: py_main.cc,v 1.1 2004/08/23 06:35:21 ksterker Exp $

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
 * @file   py_wrappers/adonthell/py_main.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  The main application class for python scripts using the Adonthell framework.
 */

#include "py_main.h"

// Global entry point
AdonthellApp theApp;

// ctor
AdonthellApp::AdonthellApp ()
{
    MainFunc = NULL;
}

// dtor
AdonthellApp::~AdonthellApp ()
{
    delete MainFunc;
}

// called from initialization code    
int AdonthellApp::main () 
{ 
    if (!MainFunc)
    {
        printf ("*** py_main: no callback to python script given! Exiting ...");
        return 1;
    }
        
    return (*MainFunc)();
}

// init adonthell framework from python
void AdonthellApp::init (PyObject *main_func, const bool & parse_args) 
{
    int argc = 0;
    char **argv = NULL;
    MainFunc = new python::functor_0ret<int> (main_func);

    // retrieve args passed to script?
    if (parse_args == true)
    {
        PyObject *py_args = PySys_GetObject ("argv");
        if (py_args && PyList_Check (py_args))
        {
            argc = PyList_GET_SIZE (py_args);
            argv = new char*[argc];

            // copy over args from python side
            for (int i = 0; i < argc; i++)
            {
                // return borrowed reference to python arguments
                PyObject *item = PyList_GET_ITEM (py_args, i);
                if (item && PyString_Check (item))
                    argv[i] = PyString_AsString (item);
            }
            
            // 'fix' argv[0]
            if (argc > 0)
            {
                char *prog = new char[strlen (argv[0]) + 8];
                sprintf (prog, "python %s", argv[0]);
                argv[0] = prog;
            }
        }
    }
    
    // pass arguments to main class
    adonthell::app::parse_args (argc, argv);

    // initialize Adonthell framework
    adonthell::app::init ();
}
