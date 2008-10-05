/*
   $Id: callback.h,v 1.8 2008/10/05 09:22:03 ksterker Exp $

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
 * @file   python/callback.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the Python callback template classes.
 * 
 * 
 */


#ifndef PYTHON_CALLBACK_H
#define PYTHON_CALLBACK_H

#include "base/callback.h"
#include "python/python.h"

#include <iostream>

namespace python
{
    /**
     * Base class for Python callbacks.
     * 
     */
    class functor_base
    {
    public:
        functor_base (PyObject * c);
        ~functor_base();

    protected:
        PyObject * callable;
    };

    /**
     * Python callback, no argument, no return value.
     * 
     */
    class functor_0 : public base::functor_0, public python::functor_base
    {
    public:
        functor_0 (PyObject * c);

    private:
        void run();
    };

    /**
     * Python callback, no argument, return value.
     * 
     */
    template <class RT>
    class functor_0ret : public base::functor_0ret<RT>, public python::functor_base
    {
    public:
        functor_0ret (PyObject * c) : base::functor_0ret<RT>(),
                                      python::functor_base(c)
        {
            *((base::functor_0ret<RT> *)this) = base::membertranslator_0ret<RT, functor_0ret<RT>, RT (functor_0ret<RT>::*)()>(*this, &python::functor_0ret<RT>::run);
        }

    private:
        RT run()
        {
            RT retvalue = RT();
            PyObject * pyres;
            
            // We can directly call our function
            pyres = PyObject_CallObject(callable, NULL);            
            show_traceback();
            
            if (pyres)
            {
                retvalue = retrieve_instance<RT, RT>(pyres);
                Py_XDECREF(pyres);
            }

            return retvalue;
        }
    };
    
  /**
     * Python callback, one argument, no return value.
     * 
     */
    template <class P1>
    class functor_1 : public base::functor_1<P1>, public python::functor_base
    {
    public:
        functor_1 (PyObject * c) : base::functor_1<P1>(),
                                   python::functor_base(c)
        {
            *((base::functor_1<P1>*)this) = base::membertranslator_1<P1,functor_1<P1>,void (functor_1<P1>::*)(P1)>(*this, &python::functor_1<P1>::run);
        }
        
    private:
        void run(P1 arg1)
        {
            PyObject * pyarg1;
            PyObject * pyres;
            
            PyObject * pyargs = PyTuple_New(1);
            pyarg1 = pass_instance(arg1);
            if (!pyarg1) std::cerr << "Warning! Argument not valid!\n" << std::endl;
            
            // The SetItem steals our reference to pyarg1
            PyTuple_SetItem(pyargs, 0, pyarg1);
            
            // We can finally call our function
            pyres = PyObject_CallObject(callable, pyargs);
            
            show_traceback();
            
            Py_DECREF(pyargs);
            Py_XDECREF(pyres);
        }
    };



  /**
     * Python callback, two arguments, no return value.
     * 
     */
  template <class P1, class P2>
    class functor_2 : public base::functor_2<P1, P2>, public python::functor_base
    {
    public:
      functor_2 (PyObject * c) : base::functor_2<P1, P2>(),
				 python::functor_base(c)
      {
	*((base::functor_2<P1, P2>*)this) = base::membertranslator_2<P1, P2,functor_2<P1, P2>,void (functor_2<P1, P2>::*)(P1, P2)>(*this, &python::functor_2<P1, P2>::run);
      }
        
    private:
        void run(P1 arg1, P2 arg2)
        {
            PyObject * pyarg1;
	    PyObject * pyarg2;
            PyObject * pyres;
            
            PyObject * pyargs = PyTuple_New(2);
            pyarg1 = pass_instance(arg1);
	    pyarg2 = pass_instance(arg2);
            if (!pyarg1) std::cerr << "Warning! Argument not valid!\n" << std::endl;
	    if (!pyarg2) std::cerr << "Warning! Argument not valid!\n" << std::endl;
            
            // The SetItem steals our reference to pyarg1
            PyTuple_SetItem(pyargs, 0, pyarg1);
	    PyTuple_SetItem(pyargs, 1, pyarg2);
            
            // We can finally call our function
            pyres = PyObject_CallObject(callable, pyargs);
            
            show_traceback();
            
            Py_DECREF(pyargs);
            Py_XDECREF(pyres);
        }
    };



    /**
     * Python callback, one argument, return value.
     * 
     */
    template <class P1, class RT>
    class functor_1ret : public base::functor_1ret<P1, RT>, public python::functor_base
    {
    public:
        functor_1ret (PyObject * c) : base::functor_1ret<P1, RT>(),
                                      python::functor_base(c)
        {
            *((base::functor_1ret<P1, RT>*)this) = base::membertranslator_1ret<P1,RT,functor_1ret<P1, RT>,RT (functor_1ret<P1, RT>::*)(P1)>(*this, &python::functor_1ret<P1, RT>::run);
        }
        
    private:
        RT run(P1 arg1)
        {
            RT retvalue = RT();
            PyObject * pyarg1;
            PyObject * pyres;
            
            PyObject * pyargs = PyTuple_New(1);
            pyarg1 = pass_instance(arg1);
            if (!pyarg1) std::cerr << "Warning! Argument not valid!\n" << std::endl;
            
            // The SetItem steals our reference to pyarg1
            PyTuple_SetItem(pyargs, 0, pyarg1);

            // We can finally call our function
            pyres = PyObject_CallObject(callable, pyargs);

            show_traceback();
            
            Py_XDECREF(pyargs);

            if (pyres)
            {
                retvalue = retrieve_instance<RT, RT>(pyres);
                show_traceback();
                Py_XDECREF(pyres);
            }

            return retvalue;
            
            return 0;
        }
    };
}

#endif
