/*
 $Id: plane3.cc,v 1.1 2007/10/22 06:05:09 ksterker Exp $
 
 Copyright (C) Kai Sterker <kaisterker@linuxgames.com>
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
 * @file   world/plane3.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Implements the 3D plane class.
 * 
 */

#include "world/plane3.h"

using world::plane3;

// ctor
plane3::plane3 (const triangle3 & triangle)
{
    Origin = triangle.a ();
    Normal = triangle.normal ().normalize ();
    
    Equation[A] = Normal.x ();
    Equation[B] = Normal.y ();
    Equation[C] = Normal.z ();
    Equation[D] = -(Normal.dot (Origin)); 
}
