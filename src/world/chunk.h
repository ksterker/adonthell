/*
 $Id: chunk.h,v 1.11 2009/03/21 11:59:47 ksterker Exp $
 
 Copyright (C) 2008 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file   world/chunk.h
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Declares the chunk class.
 * 
 * 
 */


#ifndef WORLD_CHUNK_H
#define WORLD_CHUNK_H

#include "world/chunk_info.h"
#include "world/coordinates.h"

namespace world
{
    /**
     * An octree-like structure that keeps track of the location of map objects in
     * 3D space. Unlike most octrees, it allows dynamic adding and removal of 
     * objects to reflect changes in the game world.
     *
     * All objects in the tree of chunks is stored by axis aligned bounding boxes (AABB),
     * allowing for efficient retrieval methods. Those include finding objects that
     * intersect with a given AABB (for collision detection) and finding objects that
     * are visible in a given map view.
     */
    class chunk
    {
    public:        
        chunk ();
        
        /**
         * @name Chunk population
         */
        //@{
        /**
         * Add object at given coordinates.
         * @param object entity to add to the world.
         * @param coordinates location of the entity.
         */
        void add (entity * object, const coordinates & pos);
        
        /**
         * Add object at given coordinates.
         * @param ci entity to add to the world.
         */        
        void add (const chunk_info & ci);
        
        /**
         * Remove object at given coordinates.
         * @param object entity to remove from the world.
         * @param coordinates location of the entity.
         */
        void remove (entity * object, const coordinates & pos);
        
        /**
         * Remove object from world.
         * @param ci entity to remove from world.
         */                
        void remove (const chunk_info & ci);
        //@}
        
        /**
         * @name Object retrieval
         */
        //@{
        /**
         * Collects a list of objects that are contained in the given mapview.
         *
         * @param x      x-coordinate of the views origin
         * @param y      y-coordinate of the views origin
         * @param z      z-coordinate of the views origin
         * @param length length of the view
         * @param width  width of the view
         *
         * @return list of objects contained in view.
         */
        std::list<chunk_info*> objects_in_view (const s_int32 & x, const s_int32 & y, const s_int32 & z, const s_int32 & length, const s_int32 & width) const;
        
        /**
         * Collects a list of objects that are contained by the given bounding box.
         *
         * @param min the lower coordinate triplet of the bbox.
         * @param max the upper coordinate triplet of the bbox.
         *
         * @return list of objects contained in bbox.
         */
        std::list<chunk_info*> objects_in_bbox (const vector3<s_int32> & min, const vector3<s_int32> & max) const;
        
        /**
         * Collects a list of objects that are contained by the given bounding box and adds them to given list.
         *
         * @param min the lower coordinate triplet of the bbox.
         * @param max the upper coordinate triplet of the bbox.
         * @param result list that will receive the objects contained in the bbox
         */
        void objects_in_bbox (const vector3<s_int32> & min, const vector3<s_int32> & max, std::list<chunk_info*> & result) const;
        //@}
        
        /**
         * @name Chunk attributes
         */
        //@{
        /**
         * Check if the %chunk has any children.
         * @return true, if it doesn't, false otherwise.
         */
        bool is_leaf () const
        {
            static vector3<s_int32> EMPTY;
            return Split == EMPTY;
        }
        
        /**
         * Check whether the %chunk contains any objects.
         * @return true if it doesn't, false otherwise.
         */
        bool is_empty () const
        {
            return Objects.empty ();
        }
        
        /**
         * Check whether the %chunk can be split or if it already has
         * minimum dimensions.
         * @return true if it can be split, false otherwise.
         */        
        bool can_split () const;
        
        /**
         * Return the extend of the %chunk in x direction.
         * @return extend of %chunk in x direction.
         */
        u_int32 length () const { return Max.x() - Min.x(); }
        
        /**
         * Return the extend of the %chunk in y direction.
         * @return extend of %chunk in y direction.
         */
        u_int32 height () const { return Max.y() - Min.y(); }
        //@}
        
        void debug () const;
            
private:
        /**
         * Find those children of the %chunk that overlap with the bbox
         * specified by its minumum and maximum coordinate triplets.
         *
         * @param chunks this array will be filled with the indices of 
         *   the child %chunks that overlap with the given bbox.
         * @param min minimum coordinate triplet
         * @param max maximum coordinate triplet
         *
         * @return number of children that overlap.
         */
        const u_int8 find_chunks (s_int8 chunks[8], const vector3<s_int32> & min, const vector3<s_int32> & max) const;
        
        /**
         * Collects a list of objects that are contained in the given mapview.
         *
         * @param min_x  x-coordinate of the views origin
         * @param max_x  x-coordinate of the views origin plus length of the view
         * @param min_yz difference of y and z-coordinates of the views origin
         * @param max_yz min_yz plus width of the view
         * @param result vector to populate with contained objects.
         */
        void objects_in_view (const s_int32 & min_x, const s_int32 & max_x, const s_int32 & min_yz, const s_int32 & max_yz, std::list<chunk_info*> & result) const;

        /**
         * Checks whether a given mapview overlaps with an axis aligned bounding box.
         *
         * @param min_x x-coordinate of the views origin
         * @param max_x x-coordinate of the views origin plus length of the view
         * @param min_yz difference of y and z-coordinates of the views origin
         * @param max_yz min_yz plus width of the view
         * @param min   minimum corner of the AABB
         * @param max   maximum corner of the AABB
         * @return true if view and AABB overlap, false otherwise.
         */
        bool in_view (const s_int32 & min_x, const s_int32 & max_x, const s_int32 & min_yz, const s_int32 & max_yz, const vector3<s_int32> & min, const vector3<s_int32> & max) const;
        
        bool in_bbox (const vector3<s_int32> & a_min, const vector3<s_int32> & a_max, const vector3<s_int32> & b_min, const vector3<s_int32> & b_max) const;

        /**
         * Calculate the split planes of the chunk. Can be called only once in
         * the chunks lifetime, unless its children are merged together.
         */
        void split ();
        
        /**
         * Generate a picture of the chunk (and its children) in .dot format, as
         * parsed by AT&Ts graphviz package.
         * @param graph file stream to write to
         * @param parent id of the parent %chunk
         */
        void debug (std::ofstream & graph, const int & parent) const;
        
        /// indicates that the chunk size has changed and needs to be recalculated 
        bool Resize;
        /// the children of the chunk
        chunk* Children[8];
        /// the objects contained in the chunk
        std::list<chunk_info> Objects;
        
        /// the minimum of the chunks AABB
        vector3<s_int32> Min;
        /// the maximum of the chunks AABB
        vector3<s_int32> Max;
        /// the split planes of the chunk
        vector3<s_int32> Split;
    };
}

#endif // WORLD_CHUNK_H
