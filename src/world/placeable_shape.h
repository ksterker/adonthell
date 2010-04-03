/*
 Copyright (C) 2007 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file   world/placeable_shape.h
 * @author  Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Declares the placeable_shape class.
 * 
 * 
 */


#ifndef WORLD_PLACEABLE_AREA_H_
#define WORLD_PLACEABLE_AREA_H_

#include "world/cube3.h"

namespace world
{
    /**
     * A rough 3D representation of a placeable on the map. It consists
     * of a number of hexahedrons and a bounding box. It is used for 
     * collision detection and rendering order.
     * 
     * All values returned are pixel based.
     */
    class placeable_shape
    {
    public:
        /**
         * Create new collision information for a placeable.
         */
        placeable_shape()
        {
            Solid = true;
        }

        /**
         * Destructor.
         */
        ~placeable_shape();
        
        /**
         * @name Parts of Shape
         */
        //@{
        /**
         * Add a part to this shape.
         * @param part the part to add to the shape.
         */
        void add_part (cube3 * part);
        
        /**
         * Remove a part from this shape.
         * @param part the part to remove from the shape.
         */
        void remove_part (cube3 *part);

#ifndef SWIG
        /**
         * Get iterator pointing to the first part of the shape.
         * @return iterator into the shape list.
         */
        std::vector<cube3*>::const_iterator begin() const { return Parts.begin(); } 

        /**
         * Get iterator pointing past the end of the shape list.
         * @return iterator to the shape list end.
         */
        std::vector<cube3*>::const_iterator end() const { return Parts.end(); }   
#endif // SWIG
        //@}
        
        /**
         * Perform collision against this object. Result is stored in
         * given collisionData parameter.
         * @param collisionData information about the performed move.
         * @param offset position of shape on the world map
         */
        void collide (collision * collisionData, const vector3<s_int16> & offset) const;
        
        /**
         * @name Extension of Shape
         * Methods to query base point and size of the shape.
         */
        //@{
        /**
         * Get x-coordinate of shape's base point.
         * @return x-coordinate of shape's base point.
         */
        s_int16 x () const
        {
            return Min.x();
        }

        /**
         * Get y-coordinate of shape's base point.
         * @return y-coordinate of shape's base point.
         */
        s_int16 y () const
        {
            return Min.y();
        }
        
        /**
         * Get z-coordinate of shape's base point.
         * @return z-coordinate of shape's base point.
         */
        s_int16 z () const
        {
            return Min.z();
        }
        
        /**
         * Get extension of shape in x direction
         * @return extension in x direction.
         */
        u_int16 length () const
        {
            return Max.x() - Min.x();
        }

        /**
         * Get extension of shape in y direction
         * @return extension in y direction.
         */
        u_int16 width () const
        {
            return Max.y() - Min.y();
        }

        /**
         * Get extension of shape in z direction
         * @return extension in z direction.
         */
        u_int16 height () const
        {
            return Max.z() - Min.z();
        }

        /**
         * Get min base point
         * @return base point for the shape
         */
        vector3<s_int16> get_min() const
        {
            return Min;
        }

        /**
         * Get extension of shape in all directions
         * @return extension in all direction.
         */
        vector3<s_int16> get_max() const
        {
            return Max;
        }
        
        /**
         * @name Sprite position relative to shape.
         */
        //@{
        /**
         * Get sprites x-offset from shape's base point.
         * @return sprites x-offset from of shape's base point.
         */
        s_int16 ox () const
        {
            return Offset.x();
        }
        
        /**
         * Get sprites y-offset from of shape's base point.
         * @return sprites y-offset from shape's base point.
         */
        s_int16 oy () const
        {
            return Offset.y();
        }        
        //@}
        
        /**
         * @name "Physical" shape properties.
         */
        //@{
        /**
         * Return whether the shape if flat or vertical.
         * @return true if width >= height, false otherwise.
         */
        bool is_flat () const
        {
            return width() >= height();
        }
        
        /**
         * Return whether the shape is solid or not.
         * @return true if solid, false otherwise.
         */
        bool is_solid () const
        {
            return Solid;
        }
        //@}
        
        /**
         * @name Shape editing.
         */
        //@{
        /**
         * Set offset of sprite in respect to the shape.
         * @param ox offset in x direction.
         * @param oy offset in y direction.
         */
        void set_offset (const s_int16 & ox, const s_int16 & oy)
        {
            Offset.set (ox, oy, 0);
        }
        
        /**
         * Set whether shape is solid or not. Non-solid shapes
         * are ignored by collision detection, but still used for
         * rendering.
         * @param solid true to make shape solid, false otherwise.
         */
        void set_solid (const bool & solid)
        {
            Solid = solid;
        }
        //@}
        
        /**
         * @name Loading / Saving
         */
        //@{
        /**
         * Save shape state to stream. 
         * @param file stream to save shape to.
         * @return \b true if saving successful, \b false otherwise.
         */
        bool put_state (base::flat & file) const;
        
        /**
         * Load shape state from stream. 
         * @param file stream to load shape from.
         * @return \b true if loading successful, \b false otherwise.
         */        
        bool get_state (base::flat & file);
        //@}

    private:
        /// collision information  
        std::vector <cube3*> Parts;
        /// minimum of object bounding box
        vector3<s_int16> Min;
        /// maximum of object bounding box
        vector3<s_int16> Max;
        /// sprite offset
        vector3<s_int16> Offset;
        /// used to toggle collision on or off
        bool Solid;
    }; 
}

#endif
