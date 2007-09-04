/*
 $Id: vector3.h,v 1.1 2007/09/04 02:27:18 ksterker Exp $
 
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
 * @file   world/vector3.h
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Defines the 3D vector  class.
 * 
 */

#ifndef WORLD_VECTOR_3_H
#define WORLD_VECTOR_3_H

#include "base/flat.h"

namespace world
{
/**
 * Implements a point in 3D space.
 */
class vector3 
{
public:
    /**
     * Create a 3D vector located at the origin
     */
    vector3 () : X (0), Y (0), Z (0)
    {
    }

	/**
	 * Create a new 3D vector.
	 * @param x x coordinate
	 * @param y y coordinate
	 * @param z z coordinate
	 */
	vector3 (const u_int16 & x, const u_int16 & y, const u_int16 & z) : X (x), Y (y), Z (z)
	{
	}

    /**
     * Create a new 3D vector as copy of existing vector
     * @param v the vector to copy
     */
    vector3 (const vector3 & v) : X (v.X), Y (v.Y), Z (v.Z)
    {
    }
    
    /**
     * Set the location of a vector.
     * @param x new x location
     * @param y new y location
     * @param z new z location
     */
    void set (const u_int16 & x, const u_int16 & y, const u_int16 & z)
    {
    	X = x;
    	Y = y;
    	Z = z;
    }
    
    /**
     * @name Vector Comparison
     */
    //@{
	/**
	 * Compare two sets of vectors for equality.
	 * @param v vector to compare with this.
	 * @return true if all members are equal, false otherwise.
	 */
	bool operator == (const vector3 & v)
	{
		return (X == v.X && Y == v.Y && Z == v.Z);
	}

	/**
	 * Compare two sets of vectors for differenct.
	 * @param v vector to compare with this.
	 * @return true if at least one members differs, false otherwise.
	 */
	bool operator != (const vector3 & v)
	{
		return (X != v.X || Y != v.Y || Z != v.Z);
	}
	//@}
	
    /**
     * @name Loading / Saving
     */
    //@{
    /**
     * Save the vector to a stream. 
     * @param file stream to save vector to.
     * @return \b true if saving successful, \b false otherwise.
     */
    bool put_state (base::flat & file) const;
    
    /**
     * Load vector from stream. 
     * @param file stream to load vecotr from.
     * @return \b true if loading successful, \b false otherwise.
     */
    bool get_state (base::flat & file);
    //@}

private:
	/// X coordinate
	u_int16 X; 
	/// Y coordinate
	u_int16 Y;
	/// Z coordinate
	u_int16 Z;
};

}

#endif /* WORLD_VECTOR_3_H */
