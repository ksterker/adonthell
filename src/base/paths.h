/*
   $Id: paths.h,v 1.2 2003/11/22 09:35:21 ksterker Exp $

   Copyright (C) 2003  Alexandre Courbot <alexandrecourbot@linuxgames.com>
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
