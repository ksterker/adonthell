/*
 $Id: triangle3.cc,v 1.1 2007/09/24 03:15:34 ksterker Exp $
 
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

using world::triangle3;
using world::vector3;

// draw triangle 
void triangle3::draw (const u_int16 & x, const u_int16 & y, gfx::surface * target) const
{
	if (!target) target = gfx::screen::get_surface();
	
	target->draw_line (x + A.x(), y + A.y() + A.z(), x + B.x(), y + B.y() + B.z(), 0xFFFFFF);
	target->draw_line (x + B.x(), y + B.y() + B.z(), x + C.x(), y + C.y() + C.z(), 0xFFFFFF);
	target->draw_line (x + C.x(), y + C.y() + C.z(), x + A.x(), y + A.y() + A.z(), 0xFFFFFF);
	
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
