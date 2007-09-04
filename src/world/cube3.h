/*
 $Id: cube3.h,v 1.1 2007/09/04 02:27:18 ksterker Exp $
 
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
 * @file   world/cube3.h
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Defines the 3D cube class.
 * 
 */

#ifndef WORLD_CUBE_3_H
#define WORLD_CUBE_3_H

#include <vector>
#include "world/triangle3.h"

namespace world
{
/**
 * Implements a 3D cube.
 * 
 */
class cube3 
{
public:
	/**
	 * The corner points of the cube
	 */
	enum 
	{
		BOTTOM_FRONT_LEFT = 0,
		BOTTOM_FRONT_RIGHT = 1,
		BOTTOM_BACK_RIGHT = 2,
		BOTTOM_BACK_LEFT = 3,
		TOP_FRONT_LEFT = 4,
		TOP_FRONT_RIGHT = 5,
		TOP_BACK_RIGHT = 6,
		TOP_BACK_LEFT = 7,		
	};
	
	/**
	 * Create a new standard 3D cube.
	 * @param length size along x axis in pixel
	 * @param width size along y axis in pixel
	 * @param height size along z axis in pixel
	 */
	cube3 (const u_int16 & length, const u_int16 & width, const u_int16 & height);

	/**
	 * Convert the cube into a mesh of triangles. Call every time a point of the
	 * cube is changed.
	 */
	void create_mesh ();
	
    /**
     * @name Loading / Saving
     */
    //@{
    /**
     * Save the cube to a stream. 
     * @param file stream to save cube to.
     * @return \b true if saving successful, \b false otherwise.
     */
    bool put_state (base::flat & file) const;
    
    /**
     * Load cube from stream. 
     * @param file stream to load cube from.
     * @return \b true if loading successful, \b false otherwise.
     */
    bool get_state (base::flat & file);
    //@}

protected:
	/// the cube's corners
	vector3 Corners[8];	

private:
	/**
	 * Split a face of the cube, specified by the indices of its four corner points,
	 * into triangles.
	 * @param a index of bottom left corner
	 * @param b index of bottom right corner
	 * @param c index of top right corner
	 * @param d index of top left corner
	 */
	void convert_face (const u_int16 & a, const u_int16 & b, const u_int16 & c, const u_int16 & d);

	/// the cube's surface
	std::vector<triangle3 *> Surface;
	
    /**
     * Forbid copy construction.
     */
    cube3 (const cube3 & src);
};

}

#endif /* WORLD_CUBE_3_H */
