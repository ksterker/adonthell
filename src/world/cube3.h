/*
 $Id: cube3.h,v 1.8 2009/03/21 14:29:10 ksterker Exp $
 
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
#include "collision.h"

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
		BOTTOM_BACK_LEFT = 2,
		BOTTOM_BACK_RIGHT = 3,
		TOP_FRONT_LEFT = 4,
		TOP_FRONT_RIGHT = 5,
		TOP_BACK_LEFT = 6,
		TOP_BACK_RIGHT = 7,
        NUM_CORNERS = 8
	};
	
    /**
     * Create a new standard 3D cube.
     * @param length size along x axis in pixel
     * @param width size along y axis in pixel
     * @param height size along z axis in pixel
     */
    cube3 (const u_int16 & length, const u_int16 & width, const u_int16 & height);
	
    /**
     * Create a new standard 3D cube.
     * @param min Minimum point
     * @param max Maximum point
     */
    cube3 (const vector3<s_int16> &min, const vector3<s_int16> &max);

    /**
     * Destructor.
     */
    ~cube3 ();
    
    /**
     * @name Member access.
     */
    //@{
    /**
     * Move a corner of the cube to the given position. This does not
     * automatically update the bounding box and mesh.
     * @param point the corner to update.
     * @param pos the new position.
     */
    void set_point (const u_int32 & point, const vector3<s_int16> & pos)
    {
        if (point < NUM_CORNERS)
        {
            Corners[point] = pos;
        }
    }
    
    /**
     * Get the position of one of the cube's corner points.
     * @return the position of the desired corner point.
     */
    vector3<s_int16> get_point (const u_int32 & point) const
    {
        if (point < NUM_CORNERS)
        {
            return Corners[point];
        }
        
        return vector3<s_int16>();
    }
    //@}
    
    /**
     * @name Bounding Box
     */
    //@{
    /**
     * Calculate bounding box for this cube. Call every time a point of the
	 * cube is changed.
     */
    void create_bounding_box ();
    
    /**
     * Return the point of the cube furthest to the left.
     * @return the left side of the cube.
     */
    s_int16 min_x () const { return Min.x (); }
    
    /**
     * Return the point of the cube furthest to the right.
     * @return the right side of the cube.
     */
    s_int16 max_x () const { return Max.x (); }
    
    /**
     * Return the point representing the back of the cube.
     * @return the back of the cube.
     */
    s_int16 min_y () const { return Min.y (); }
    
    /**
     * Return the point representing the front of the cube.
     * @return the front of the cube.
     */
    s_int16 max_y () const { return Max.y (); }
    
    /**
     * Return the lowest point of the cube.
     * @return the lowest point of the cube.
     */
    s_int16 min_z () const { return Min.z (); }
    
    /**
     * Return the highest point of the cube.
     * @return the highest point of the cube.
     */
    s_int16 max_z () const { return Max.z (); }
    
	/**
     * Draw the cube's outline
	 * @param x x offset in pixels
	 * @param y y offset in pixels
     * @param da_opt optional drawing area for clipping
	 * @param target surface to draw on. NULL to draw on screen surface.
	 */
    void draw (const u_int16 & x, const u_int16 & y, const gfx::drawing_area * da_opt, gfx::surface * target) const;
    //@}
    
    /**
     * @name Mesh Related Methods
     */
    //@{
	/**
	 * Convert the cube into a mesh of triangles. Call every time a point of the
	 * cube is changed.
	 */
	void create_mesh ();
	
	/**
	 * Draw the cube's mesh
	 * @param x x offset in pixels
	 * @param y y offset in pixels
	 * @param target surface to draw on. NULL to draw on screen surface.
	 */
	void draw_mesh (const u_int16 & x, const u_int16 & y, gfx::surface * target = NULL) const;
    
    /**
     * Perform collision against the mesh of this object. Result is stored in
     * given collisionData parameter.
     * @param collisionData information about the performed move.
     * @param offset position of object on the world map.
     */
    void collide (collision * collisionData, const vector3<s_int16> & offset) const;
    //@}
	
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
	vector3<s_int16> Corners[8];	

    /**
     * Cleanup.
     */
    void clear();
        
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
	std::vector<triangle3<s_int16> *> Surface;
	
    /// bounding box minimum values
    vector3<s_int16> Min;
    /// bounding box maximum values
    vector3<s_int16> Max;
    
    /**
     * Forbid copy construction.
     */
    cube3 (const cube3 & src);
};

}

#endif /* WORLD_CUBE_3_H */
