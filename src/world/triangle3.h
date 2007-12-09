/*
 $Id: triangle3.h,v 1.4 2007/12/09 21:39:43 ksterker Exp $
 
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
class triangle3 
{
public:
	/**
	 * Create a new triangle from its 3 corner points.
	 * @param a first point
	 * @param b second point
	 * @param c third point
	 */
	triangle3 (const vector3<s_int16> & a, const vector3<s_int16> & b, const vector3<s_int16> & c) : A (a), B (b), C (c)
	{
	}
	
	/**
	 * Calculate normal vector for the face of the triangle.
	 * @return normal vector for this triangle.
	 */
	vector3<s_int16> normal () const
	{
		return (B - A) * (C - A);
	}
	
    /**
     * Check whether the given point lies inside the triangle or not.
     * Based on http://www.blackpawn.com/texts/pointinpoly/default.html
     * @param p point to check whether it is located inside triangle.
     * @return \e true if it does, \e false otherwise.
     */
    bool triangle3::contains (const vector3<float> & p) const;
    
    /**
     * Return points of the triangle.
     * @param point number between 0 and 2 for the point to return.
     * @return points of the triangle.
     */
    const vector3<s_int16> & get_point (const u_int16 & point) const
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
    vector3<s_int16> get_edge (const u_int16 & edge) const
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
	vector3<s_int16> A;
	/// second point
	vector3<s_int16> B;
	/// third point
	vector3<s_int16> C;
    
    /**
     * forbid copy construction
     */
    triangle3 (const triangle3 & src);
};

} // namespace world
#endif /* WORLD_TRIANGLE3_H */
