/*
   $Id: python.h,v 1.1 2003/07/18 15:16:09 gnurou Exp $

   Copyright (C) 2003   Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/**
 * @file   python.h 
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  The python module main include file.
 * 
 * 
 */

#ifndef PYTHON_PYTHON_H_
#define PYTHON_PYTHON_H_

#include <Python.h>
#include <string>

#include "python/callback_support.h"


/**
 * Provides a bridge between C++ and Python,
 * as well as some convenience classes to ease
 * the use of the Adonthell library from Python.
 * 
 */
namespace python
{
    /**
     * Dump any error information to stderr.
     *
     * This function should be called after every operation
     * involving Python from C++, so the user can get informed if
     * something went wrong. By default, all the methods of this
     * module call it when appropriate.
     *
     */
    void show_traceback();

    /**
     * @name Initialization and cleanup.
     * 
     */
    //@{

    void init();
    void cleanup();

    //@}

    /**
     * @name High-level functions.
     * 
     */
    //@{

    bool add_search_path(const std::string & path);
    bool run_string(const std::string & statements);

    PyObject * import_module(const std::string & name);

    //@}

    /**
     * @name C++ to Python instance passing functions.
     *
     */
    //@{

    /**
     * Ownership of a python object made with pass_instance. Value
     * python::c_owns means that once the Python object is deleted, the
     * C++ instance will remain valid. On the contrary, python::python_owns
     * will cause the C++ instance to be destroyed when the Python object is
     * deleted.
     * 
     */
    typedef enum { c_owns = 0, python_owns = 1 } ownership;

    /** 
     * Default version of pass_instance - it will fetch the name of the class
     * that is passed using a specialized version of get_type_name to create
     * a Python object from a pointer.
     * 
     * @param arg a pointer to the object to pass to Python.
     * @param own ownership of the C++ object.
     * 
     * @return a Python object representing \e arg.
     */
    template <class A> inline
    PyObject * pass_instance(A arg, const ownership own = c_owns)
    { 
        swig_type_info * tt = SWIG_TypeQuery(get_type_name<A>());
        if (tt) return SWIG_NewPointerObj(arg, tt, own);
        return NULL;
    }
    
    /** 
     * Specialized version of pass_instance which makes a Python integer
     * from a C++ one.
     * 
     * @param arg the integer to be passed to Python.
     * @param own useless here.
     * 
     * @return a Python object representing \e arg.
     */
    template <> inline
    PyObject * pass_instance<int>(int arg, const ownership own)
    { 
        return PyInt_FromLong(arg);
    }
    
    /** 
     * Specialized version of pass_instance which makes a Python string
     * from a C++ std::string.
     * 
     * @param arg the std::string to be passed to Python.
     * @param own useless here.
     * 
     * @return a Python object representing \e arg.
     */
    template <> inline
    PyObject * pass_instance<std::string &>(std::string & arg, const ownership own) 
    { 
        return PyString_FromString(arg.c_str());
    }

    /** 
     * Specialized version of pass_instance which makes a Python string
     * from a C++ char *.
     * 
     * @param arg the char * to be passed to Python.
     * @param own useless here.
     * 
     * @return a Python object representing \e arg.
     */
    template <> inline
    PyObject * pass_instance<const char *>(const char * arg, const ownership own) 
    { 
        return PyString_FromString((char *) arg);
    }

    /** 
     * Sets the ownership of an object created with pass_instance.
     * 
     * @param obj the object to change ownership of.
     * @param o the new ownership.
     */
    inline void set_ownership(PyObject * obj, const ownership o)
    {
	PyObject * n = PyInt_FromLong(o);
	PyObject_SetAttrString (obj, (char*)"thisown", n);
	Py_DECREF(n);
    }
    
    //@}

    /**
     * @name Python to C++ instance passing functions.
     *
     */
    //@{

    /** 
     * Retrieves the C++ intance of a Python object created with pass_instance.
     * 
     * This is the default version of retrieve_instance. The template element A must
     * be a pointer type.
     *
     * @param pyinstance The Python instance of the object to retrieve.
     * 
     * @return A borrowed pointer to the C++ instance of the object.
     */
    template <class A> inline
    A retrieve_instance(PyObject * pyinstance)
    {
        A retvalue;

        swig_type_info * tt = SWIG_TypeQuery(get_type_name<A>());
        if (!tt) return NULL;
        if (SWIG_ConvertPtr(pyinstance, (void **) &retvalue, tt, 0) == -1) return NULL;
        return retvalue;
    }

    /** 
     * Retrieves the C++ value of a Python integer.
     * 
     * @param pyinstance The Python integer to retrieve.
     * 
     * @return C++ value of pyinstance.
     */
    template <> inline
    int retrieve_instance<int>(PyObject * pyinstance)
    { 
        return PyInt_AsLong(pyinstance);
    }

    /** 
     * Retrieves the C++ value of a Python string as a char *.
     * 
     * @param pyinstance The Python string to retrieve.
     * 
     * @return char * string value of pyinstance.
     */
    template <> inline
    const char * retrieve_instance<const char *>(PyObject * pyinstance)
    {
        return PyString_AsString(pyinstance);
    }

    /** 
     * Retrieves the C++ value of a Python string as a std::string.
     * 
     * @param pyinstance The Python string to retrieve.
     * 
     * @return std::string value of pyinstance.
     */
    template <> inline
    std::string retrieve_instance<std::string>(PyObject * pyinstance)
    { 
        return std::string(PyString_AsString(pyinstance));
    }

    //@}

}

#include "python/callback.h"

#endif
