/*
 $Id: plane3.h,v 1.4 2007/12/16 22:30:43 ksterker Exp $
 
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

#include "triangle3.h"

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
    plane3 (const triangle3<float> & triangle);

    /**
     * Create a plane given origin and normal vector.
     * @param origin origin of the plane.
     * @param normal normal vector of the plane.
     */
    plane3 (const vector3<float> & origin, const vector3<float> & normal);
    
    /**
     * @name Member Access
     */
    //@{
    /**
     * Get the normal vector of this plane.
     * Returns the normal vector of this plane.
     */
    const vector3<float> & normal () const { return Normal; }
    //@}
    
    /**
     * @name Geometric Checks 
     */
    //@{
    /**
     * Check whether the plane is facing towards the given point.
     * @param point a point in 3d space
     * @return true if the plane is facing the point, false otherwise.
     */
    bool is_facing (const vector3<float> & point) const
    {
        return Normal.dot (point) <= 0;
    }
    
    /**
     * Return the distance of the given point to the plane.
     * @param return distance between point and plane.
     */
    float signed_distance (const vector3<float> & point) const 
    { 
        return Normal.dot (point) + Equation[D]; 
    }
    //@}
    
private:
    /** Indizes of Plane equation */
    enum
    {
        A, B, C, D
    };

    /// the plane's normal vector
    vector3<float> Normal;
    /// the plane's origin
    vector3<float> Origin;
    /// the plane equation ax + by + cz + d = 0
    float Equation[4];
};

}

#endif
