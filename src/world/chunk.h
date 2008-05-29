/*
 $Id: chunk.h,v 1.4 2008/05/29 20:18:27 ksterker Exp $
 
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

#include <list>

#include "world/coordinates.h"
#include "world/placeable.h"

namespace world
{
    class chunk_info
    {
    public:
        chunk_info (placeable *o, const vector3<s_int32> & min, const vector3<s_int32> & max) 
            : Object (o), Min (min), Max (max)
        {
        }

        chunk_info (const chunk_info & ci) 
            : Object (ci.Object), Min (ci.Min), Max (ci.Max)
        {
        }
        
        bool operator == (const chunk_info & ci) const;
        bool operator < (const chunk_info & ci) const;
        
        placeable * Object;
        vector3<s_int32> Min;
        vector3<s_int32> Max;
    };
            
    class chunk
    {
    public:        
        chunk ();
        
        void add (placeable * object, const coordinates & pos);
        void add (const chunk_info & ci);
        
        void remove (placeable * object, const coordinates & pos);
        void remove (const chunk_info & ci);
        
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
        std::list<chunk_info> objects_in_view (const s_int32 & x, const s_int32 & y, const s_int32 & z, const s_int32 & length, const s_int32 & width) const;
        
        bool is_leaf () const
        {
            static vector3<s_int32> EMPTY;
            return Split == EMPTY;
        }
        
        bool is_empty () const
        {
            return Objects.empty ();
        }
        
        bool can_split () const;
        
        u_int32 length () const { return Max.x() - Min.x(); }
        u_int32 height () const { return Max.y() - Min.y(); }
        
        void debug () const;
            
private:
        const u_int8 find_chunks (s_int8 chunks[9], const vector3<s_int32> & min, const vector3<s_int32> & max) const;
        
        /**
         * Collects a list of objects that are contained in the given mapview.
         *
         * @param min_x  x-coordinate of the views origin
         * @param max_x  x-coordinate of the views origin plus length of the view
         * @param min_yz difference of y and z-coordinates of the views origin
         * @param max_yz min_yz plus width of the view
         * @param result vector to populate with contained objects.
         */
        void objects_in_view (const s_int32 & min_x, const s_int32 & max_x, const s_int32 & min_yz, const s_int32 & max_yz, std::list<chunk_info> & result) const;

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
        
        /**
         * Calculate the split planes of the chunk. Can be called only once in
         * the chunks lifetime, unless its children are merged together.
         */
        void split ();
        
        /**
         * Generate a picture of the chunk (and its children) in .dot format, as
         * parsed by AT&Ts graphviz package.
         * @param
         * @param
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
