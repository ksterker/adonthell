/*
 $Id: cube3.cc,v 1.1 2007/09/04 02:27:18 ksterker Exp $
 
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
 * @file   world/cube3.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Implements the 3D cube class.
 * 
 */

#include "world/cube3.h"

using world::cube3;

// ctor
cube3::cube3 (const u_int16 & length, const u_int16 & width, const u_int16 & height)
{
	Corners[BOTTOM_FRONT_LEFT].set (0, 0, 0);
	Corners[BOTTOM_FRONT_RIGHT].set (length, 0, 0);
	Corners[BOTTOM_BACK_RIGHT].set (length, width, 0);
	Corners[BOTTOM_BACK_LEFT].set (0, width, 0);
	Corners[TOP_FRONT_LEFT].set (0, 0, height);
	Corners[TOP_FRONT_RIGHT].set (length, 0, height);
	Corners[TOP_BACK_RIGHT].set (length, width, height);
	Corners[TOP_BACK_LEFT].set (0, width, height);
}

// convert to triangles
void cube3::create_mesh ()
{
	Surface.clear ();
	
	convert_face (BOTTOM_FRONT_LEFT, BOTTOM_FRONT_RIGHT, BOTTOM_BACK_RIGHT, BOTTOM_BACK_LEFT);
	convert_face (BOTTOM_FRONT_LEFT, BOTTOM_FRONT_RIGHT, TOP_FRONT_RIGHT, TOP_FRONT_LEFT);
	convert_face (BOTTOM_FRONT_RIGHT, BOTTOM_BACK_RIGHT, TOP_BACK_RIGHT, TOP_FRONT_RIGHT);
	convert_face (BOTTOM_BACK_RIGHT, TOP_BACK_RIGHT, TOP_BACK_LEFT, BOTTOM_BACK_LEFT);
	convert_face (BOTTOM_FRONT_LEFT, BOTTOM_BACK_RIGHT, TOP_BACK_RIGHT, TOP_FRONT_LEFT);
	convert_face (TOP_FRONT_LEFT, TOP_FRONT_RIGHT, TOP_BACK_RIGHT, TOP_BACK_LEFT);
}

// save cube
bool cube3::put_state (base::flat & file) const
{
	base::flat record;
	for (u_int32 i = 0; i < 8; i++)
	{
		Corners[i].put_state (record);
	}
	
	file.put_flat ("cube", record);
	return true;
}

// load cube
bool cube3::get_state (base::flat & file)
{
	bool result = true;
	base::flat record = file.get_flat ("cube");
	for (u_int32 i = 0; result && i < 8; i++)
	{
		result = result & Corners[i].get_state (record);
	}
	
	create_mesh ();
	return result;
}

// split one cube face into its triangles
void cube3::convert_face (const u_int16 & a, const u_int16 & b, const u_int16 & c, const u_int16 & d)
{
	// in this case, no triangles exist
	if (Corners[a] == Corners[c] || Corners[b] == Corners[d]) return;
	
	// make sure that triangle is not actually a line
	if (Corners[a] != Corners[b] &&  Corners[b] != Corners[c])
	{
		Surface.push_back (new triangle3 (Corners[a], Corners[b], Corners[c]));
	}
	
	// make sure that triangle is not actually a line
	if (Corners[a] != Corners[d] &&  Corners[c] != Corners[d])
	{
		Surface.push_back (new triangle3 (Corners[a], Corners[c], Corners[d]));
	}
}
