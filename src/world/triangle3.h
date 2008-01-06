/*
 $Id: triangle3.h,v 1.6 2008/01/06 15:49:41 ksterker Exp $
 
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
 * @file   world/triangle3.h
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Defines the 3D triangle class.
 * 
 */


#ifndef WORLD_TRIANGLE3_H
#define WORLD_TRIANGLE3_H

#include "gfx/surface.h"
#include "world/vector3.h"

namespace world 
{
/**
 * A triangle in 3D space. 
 */
template<class T> class triangle3 
{
public:
	/**
	 * Create a new triangle from its 3 corner points.
	 * @param a first point
	 * @param b second point
	 * @param c third point
	 */
	triangle3 (const vector3<T> & a, const vector3<T> & b, const vector3<T> & c) : A (a), B (b), C (c)
	{
	}
	
	/**
	 * Calculate normal vector for the face of the triangle.
	 * @return normal vector for this triangle.
	 */
	vector3<T> normal () const
	{
		return (B - A) * (C - A);
	}
	
    /**
     * Check whether the given point lies inside the triangle or not.
     * Based on http://www.blackpawn.com/texts/pointinpoly/default.html
     * @param p point to check whether it is located inside triangle.
     * @return \e true if it does, \e false otherwise.
     */
    bool contains (const vector3<float> & p) const;

    /**
     * Convert given triangle from world to ellipse space.
     * @param v the units to scale each axis.
     * @param offset the units to translate each axis.
     * @return converted triangle.
     */
    triangle3<float> translate (const vector3<float> & v, const vector3<s_int16> & offset) const
    {
        float x = v.x(), y = v.y(), z = v.z();
        return triangle3<float> (
            vector3<float> ((A.x() + offset.x()) / x, (A.y() + offset.y()) / y, (A.z() + offset.z()) / z),
            vector3<float> ((B.x() + offset.x()) / x, (B.y() + offset.y()) / y, (B.z() + offset.z()) / z),
            vector3<float> ((C.x() + offset.x()) / x, (C.y() + offset.y()) / y, (C.z() + offset.z()) / z));
    }

    /**
     * Return points of the triangle.
     * @param point number between 0 and 2 for the point to return.
     * @return points of the triangle.
     */
    const vector3<T> & get_point (const u_int16 & point) const
    {
        switch (point)
        {
            case 0: return A;
            case 1: return B;
            case 2: return C;
            default: break;
        }

        fprintf (stderr, "*** triangle3::get_point: index %i out of range [0, 2]!\n", point);
        return get_point (point % 3);
    }

    /**
     * Return edge of the triangle.
     * @param edge number between 0 and 2 for the edge to return.
     * @return edge of the triangle.
     */
    vector3<T> get_edge (const u_int16 & edge) const
    {
        switch (edge)
        {
            case 0: return A - C;
            case 1: return B - A;
            case 2: return C - B;
            default: break;
        }
        
        fprintf (stderr, "*** triangle3::get_edge: index %i out of range [0, 2]!\n", edge);
        return get_edge (edge % 3);
    }    
    
	/**
	 * Draw the triangle.
	 * @param x x offset in pixels
	 * @param y y offset in pixels
	 * @param target surface to draw on. NULL to draw on screen surface.
	 */
	void draw (const u_int16 & x, const u_int16 & y, gfx::surface * target = NULL) const;
	
private:
	/// first point
	vector3<T> A;
	/// second point
	vector3<T> B;
	/// third point
	vector3<T> C;
    
    /**
     * forbid copy construction
     */
    template<class PT>
    triangle3<T> (const triangle3<PT> & src);
};

} // namespace world
#endif /* WORLD_TRIANGLE3_H */
