/*
   $Id: base.h,v 1.3 2004/08/02 07:35:28 ksterker Exp $

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
 * @file   base/base.h 
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  The base module main include file.
 * 
 * 
 */

#include "base/paths.h"
#include "base/callback.h"
#include "base/timer.h"

/**
 * This module provides the basic stuff needed by many other modules:
 * interface for callback functions, ...
 * 
 */
namespace base
{
    /**
     * Init the base module.
     */
    void init (const std::string & userdatadir, const std::string & game);

    /// class to open files from given search paths
    static base::paths Paths;
    
    /// timer instance used by the engine
    static base::timer Timer;
}
