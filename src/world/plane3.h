/*
 $Id: plane3.h,v 1.1 2007/10/22 06:05:09 ksterker Exp $
 
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
 * @file   world/plane3.h
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Defines the 3D plane class.
 * 
 */

#ifndef WORLD_PLANE3_H
#define WORLD_PLANE3_H

#include "world/triangle3.h"

namespace world
{
    
/**
 * A plane in R3.
 */
class plane3
{
public:
    /**
     * Create a plane given a triangle.
     * @param triangle the triangle from which to create the plane.
     */
    plane3 (const triangle3 & triangle);
    
private:
    /** Indizes of Plane equation */
    enum
    {
        A, B, C, D
    };

    /// the plane's normal vector
    vector3<float> Normal;
    /// the plane's origin
    vector3<s_int16> Origin;
    /// the plane equation ax + by + cz + d = 0
    float Equation[4];
};

}

#endif
