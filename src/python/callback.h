/*
   $Id: callback.h,v 1.1 2003/07/18 15:16:09 gnurou Exp $

   Copyright (C) 2003   Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
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

#include "base/types.h"
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
        functor_base (PyObject * c) : callable(c)
        {
            Py_INCREF(callable);
        }

        ~functor_base()
        {
            Py_DECREF(callable);
        }

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
        functor_0 (PyObject * c) : base::functor_0(),
                                   python::functor_base(c)
        {
            *((base::functor_0 *)this) = base::membertranslator_0<functor_0, void (functor_0::*)()>(*this, &python::functor_0::run);
            Py_INCREF(callable);
        }

    private:
        void run()
        {
            PyObject * pyres;
            
            // We can directly call our function
            pyres = PyObject_CallObject(callable, NULL);
            
            show_traceback();
            
            Py_XDECREF(pyres);
        }
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
            *((base::functor_0ret<RT> *)this) = base::membertranslator_0ret<RT, functor_0ret<RT>, void (functor_0ret<RT>::*)()>(*this, &python::functor_0ret<RT>::run);
            Py_INCREF(callable);
        }

    private:
        RT run()
        {
            RT retvalue;
            PyObject * pyres;
            
            // We can directly call our function
            pyres = PyObject_CallObject(callable, NULL);            
            show_traceback();
            
            if (pyres)
            {
                retvalue = retrieve_instance<RT>(pyres);
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
            Py_INCREF(callable);
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
            
            Py_XDECREF(pyargs);
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
            Py_INCREF(callable);
        }
        
    private:
        RT run(P1 arg1)
        {
            RT retvalue;
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
                retvalue = retrieve_instance<RT>(pyres);
                Py_XDECREF(pyres);
            }

            return retvalue;
            
            return 0;
        }
    };
}

#endif
