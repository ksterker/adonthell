/*
 $Id: triangle3.cc,v 1.6 2008/02/16 21:13:26 ksterker Exp $
 
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
 * @file   world/triangle3.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Implements the 3D triangle class.
 * 
 */

#include "world/triangle3.h"
#include "gfx/screen.h"

namespace world
{
// check if point is inside triangle
template<>
bool triangle3<float>::contains (const vector3<float> & p) const
{
    // compute vectors        
    vector3<float> v0 = C - A;
    vector3<float> v1 = B - A;
    vector3<float> v2 = p - A;
    
    // compute dot products
    float dot00 = v0.dot (v0);
    float dot01 = v1.dot (v0);
    float dot11 = v1.dot (v1);
    float dot02 = v2.dot (v0);
    float dot12 = v2.dot (v1);
    
    // compute barycentric coordinates
    float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
    float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    float v = (dot00 * dot12 - dot01 * dot02) * invDenom;
    
    // check if point is in triangle
    return (u > 0.0f) && (v > 0.0f) && (u + v < 1.0f);
}

// draw triangle 
template <>
void triangle3<s_int16>::draw (const u_int16 & x, const u_int16 & y, gfx::surface * target) const
{
	if (!target) target = gfx::screen::get_surface();
	u_int32 color = target->map_color (0, 255, 255);
    
	target->draw_line (x + A.x(), y + A.y() - A.z(), x + B.x(), y + B.y() - B.z(), color);
	target->draw_line (x + B.x(), y + B.y() - B.z(), x + C.x(), y + C.y() - C.z(), color);
	target->draw_line (x + C.x(), y + C.y() - C.z(), x + A.x(), y + A.y() - A.z(), color);
	
	/*
 	// draw normals
	vector3<float> n = normal ().normalize ();
	printf ("[%.2f %.2f %.2f]\n", n.x(), n.y(), n.z());
	vector3<s_int16> c = A + B + C;
	int ox = x + c.x() / 3;
	int oy = y + (c.y() + c.z()) / 3;
	target->draw_line (ox + n.x(), oy + n.y() + n.z(), ox + n.x() * 20, oy + (n.y() + n.z()) * 20, 0x00FF00);
	*/
}

// draw triangle 
template <>
void triangle3<float>::draw (const u_int16 & x, const u_int16 & y, gfx::surface * target) const
{
	if (!target) target = gfx::screen::get_surface();
	u_int32 color = target->map_color (0, 255, 255);
	
	target->draw_line (x + A.x(), y + A.y() - A.z(), x + B.x(), y + B.y() - B.z(), color);
	target->draw_line (x + B.x(), y + B.y() - B.z(), x + C.x(), y + C.y() - C.z(), color);
	target->draw_line (x + C.x(), y + C.y() - C.z(), x + A.x(), y + A.y() - A.z(), color);
}

} // namespace world
