/*
   $Id: callback.cc,v 1.2 2003/07/24 12:57:59 gnurou Exp $

   Copyright (C) 2003   Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/**
 * @file   python/callback.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the Python callback class.
 * 
 * 
 */

#include "python/python.h"

namespace python
{
    functor_base::functor_base (PyObject * c) : callable(c)
    {
        Py_INCREF(callable);
    }
    
    functor_base::~functor_base()
    {
        Py_DECREF(callable);
    }

    functor_0::functor_0 (PyObject * c) : base::functor_0(),
                                          python::functor_base(c)
    {
        *((base::functor_0 *)this) = base::membertranslator_0<functor_0, void (functor_0::*)()>(*this, &python::functor_0::run);
        Py_INCREF(callable);
    }

    void functor_0::run()
    {
        PyObject * pyres;
        
        // We can directly call our function
        pyres = PyObject_CallObject(callable, NULL);
        
        show_traceback();
        
        Py_XDECREF(pyres);
    }
}
