/*
   $Id: python.cc,v 1.6 2004/04/09 11:57:51 ksterker Exp $

   Copyright (C) 2003/2004 Alexandre Courbot <alexandrecourbot@linuxgames.com>
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
#include "python/pool.h"

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
        Py_Initialize ();
        pool::init ();
    }

    void cleanup ()
    {
        pool::cleanup ();
        Py_Finalize ();
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
    
    // unflatten the contents of a tuple
    PyObject *get_tuple (base::flat & in, u_int16 start)
    {
        u_int16 len = in.get_uint16 ("pln");
        PyObject *tuple = PyTuple_New (len);
        void *value;
        
        for (u_int16 i = start; i < len; i++) 
        {
            switch (int type = in.next (&value)) 
            {
                case base::flat::T_STRING:
                {
                    // Stolen reference
                    PyTuple_SetItem (tuple, i, pass_instance ((*(string*) value).c_str ()));
                    break;
                }   
                case base::flat::T_SINT32:
                {
                    // Stolen reference
                    PyTuple_SetItem (tuple, i, pass_instance (*((int*) value)));
                    break; 
                }
                default:
                {
                    fprintf (stderr, "*** python::get_tuple: unsupported type: %i!\n", type);
                    break;
                }
            }
        }
        
        return tuple; 
    }
    
    // flatten the contents of a tuple
    void put_tuple (PyObject * tuple, base::flat & out, u_int16 start)
    {
        u_int16 len = PyTuple_Size (tuple);
        out.put_uint16 ("pln", len);
        
        for (u_int16 i = start; i < len; i++) 
        {
            // Borrowed reference
            PyObject *item = PyTuple_GetItem (tuple, i);
            
            // Check for the type of this object
            // String?
            if (PyString_Check (item)) 
                out.put_string ("s", PyString_AsString (item));
            
            // Integer?
            else if (PyInt_Check (item)) 
                out.put_sint32 ("i", PyInt_AsLong (item));
            
            // Not saveable
            else
            {
                PyObject *repr = PyObject_Repr (item);
                fprintf (stderr, "*** python::put_tuple: cannot save '%s'!\n", PyString_AsString (repr));
                Py_DECREF (repr);
            }
        }
    }
}
