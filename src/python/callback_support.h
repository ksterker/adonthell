/*
   $Id: callback_support.h,v 1.4 2004/04/29 08:07:49 ksterker Exp $
   
   Copyright (C) 2003/2004 Alexandre Courbot.
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


/** @file callback_support.h
 *  @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 *  @brief Declares the default version of get_type_name, which is needed
 *         for classes which should be passed as callbacks..
 *
 *  This file should be included by the declaration file of every class
 *  which objects could be passed as an argument to Python callbacks. Inclusion
 *  of this file doesn't require linking to the Adonthell Python module.
 */ 


#ifndef PYTHON_CALLBACK_SUPPORT_H
#define PYTHON_CALLBACK_SUPPORT_H

/**
 * This set of macros makes the class which name is given as argument 
 * available for Python argument passing, which means objects of this 
 * class can be passed as arguments to Python callbacks.
 */
#define GET_TYPE_NAME_VIRTUAL(CLASS) \
virtual const char* get_type_name () { return #CLASS " *"; }

#define GET_TYPE_NAME_ABSTRACT(CLASS) \
virtual const char* get_type_name () = 0;

#define GET_TYPE_NAME(CLASS) \
const char* get_type_name () { return #CLASS " *"; }

#endif
