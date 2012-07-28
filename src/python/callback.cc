/*
   $Id: callback.cc,v 1.4 2003/11/22 09:38:09 ksterker Exp $

   Copyright (C) 2003   Alexandre Courbot <alexandrecourbot@linuxgames.com>
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
 * @file   python/callback.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the Python callback class.
 * 
 * 
 */

#include "python.h"

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
