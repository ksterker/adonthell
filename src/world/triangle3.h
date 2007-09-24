/*
 $Id: triangle3.h,v 1.2 2007/09/24 03:14:11 ksterker Exp $
 
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
};

} // namespace world
#endif /* WORLD_TRIANGLE3_H */
