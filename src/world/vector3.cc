/*
 $Id: vector3.cc,v 1.1 2007/09/04 02:27:18 ksterker Exp $
 
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
 * @file   world/vector3.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Implements the 3D vector class.
 * 
 */

#include <sstream>
#include "world/vector3.h"

using world::vector3;

// save vector
bool vector3::put_state (base::flat & file) const
{
	std::stringstream out (std::ios::out);
	out << "[" << X << ", " << Y << ", " << Z << "]";
	
	file.put_string ("p3d", out.str());
	
	return true;
}

// load vector
bool vector3::get_state (base::flat & file)
{
	std::string vec = file.get_string ("p3d");
	u_int32 res = sscanf (vec.c_str(), "[%hd, %hd, %hd]", &X, &Y, &Z);
	if (res != 3)
	{
		fprintf (stderr, "*** vector3::get_state: error parsing vector '%s'", vec.c_str());
		return false;
	}
	
	return true;	
}
