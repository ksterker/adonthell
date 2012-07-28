/*
 $Id: coordinates.h,v 1.7 2009/02/08 13:25:53 ksterker Exp $
 
 Copyright (C) 2002/2007 Alexandre Courbot <alexandrecourbot@linuxgames.com>
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
 * @file   world/coordinates.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the coordinates class.
 * 
 * 
 */

#ifndef WORLD_COORDINATES_H
#define WORLD_COORDINATES_H

#include "vector3.h"

namespace world
{
    /// world space coordinates
    typedef vector3<s_int32> coordinates;    
}

#endif
