// $Id: callback_support.h,v 1.1 2003/07/18 15:16:09 gnurou Exp $
/*
   Copyright (C) 2003 Alexandre Courbot.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.
   See the COPYING file for more details.

 */


/** @file callback_support.h
 *  @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 *  @brief Declares the default version of get_type_name, which is needed
 *         for classes which should be passed as callbacks..
 *
 *  This file should be included by the declaration file of every class
 *  which objects could be passed as an argument to Python callbacks. Inclusion
 *  of this file doesn't require linking to the Adonthell Python module.
 */ 


#ifndef PYTHON_CALLBACK_SUPPORT_H_
#define PYTHON_CALLBACK_SUPPORT_H_

#include <iostream>

namespace python
{

    /** 
     * Specialize this template function every time you declare a type that should
     * be passable as an argument to Python callback functions.
     * 
     * This function should actually \e never been called. A warning message will be issued
     * to ask the forgetful programmer to declare his class with PYTHON_AS_CALLBACK_ARGUMENT.
     * 
     * Don't use this directly - use PYTHON_AS_CALLBACK_ARGUMENT instead.
     *
     * @return an empty string. When this happens, it will surely not be good!
     */
    template<class A> inline
    const char* get_type_name() 
    { 
        std::cerr << "Warning! Trying to pass an object to Python which class has not been declared with PYTHON_AS_CALLBACK_ARGUMENT! Expect crash!\n";
        return "";
    }   
}

/**
 * This macro makes the class which name is given as argument available
 * for Python argument passing, which means objects of this class can
 * be passed as arguments to Python callbacks.
 * 
 * It makes successive specializations of get_type_name.
 *
 */
#define PYTHON_AS_CALLBACK_ARGUMENT(CLASS) \
template<> inline \
const char* python::get_type_name<CLASS *>() { return #CLASS " *"; } \
template<> inline \
const char* python::get_type_name<const CLASS *>() { return #CLASS " *"; } \
template<> inline \
const char* python::get_type_name<CLASS &>() { return #CLASS " *"; } \
template<> inline \
const char* python::get_type_name<const CLASS &>() { return #CLASS " *"; }

#endif
