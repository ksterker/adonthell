/*
   $Id: paths.h,v 1.1 2003/07/18 15:16:09 gnurou Exp $

   Copyright (C) 2003  Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

/**
 * @file   base/paths.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares some primitives to get essential game paths.
 * 
 * 
 */

#ifndef BASE_PATHS_H_
#define BASE_PATHS_H_

#include "ltdl.h"

#include <string>

namespace base
{
    lt_dlhandle get_module(const std::string & modname);
}

#endif
