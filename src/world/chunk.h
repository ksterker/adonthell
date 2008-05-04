/*
 $Id: chunk.h,v 1.1 2008/05/04 13:49:21 ksterker Exp $
 
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
        
        placeable * Object;
        vector3<s_int32> Min;
        vector3<s_int32> Max;
    };
            
    class chunk
    {
    public:        
        chunk ();
        
        void add (placeable * object, const coordinates & pos);
        void add (const chunk_info * ci);
        
        bool is_leaf () const
        {
            static vector3<s_int32> EMPTY;
            return Split == EMPTY;
        }
        
        bool can_split () const;
        
        void debug () const;
            
    private:
        const u_int8 find_chunks (s_int8 chunks[9], const vector3<s_int32> & min, const vector3<s_int32> & max) const;
        void split ();
        void debug (std::ofstream & graph, const int & parent) const;
        
        chunk* Children[8];
        std::vector<const chunk_info *> Objects;
        
        vector3<s_int32> Min;
        vector3<s_int32> Max;
        vector3<s_int32> Split;
    };
}

#endif // WORLD_CHUNK_H
