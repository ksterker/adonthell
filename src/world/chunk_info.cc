/*
 Copyright (C) 2008/2009/2011 Kai Sterker <kai.sterker@gmail.com>
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
 * @file   world/chunk_info.cc
 * @author Kai Sterker <kai.sterker@gmail.com>
 * 
 * @brief  Defines the chunk_info class.
 * 
 * 
 */

#include <adonthell/base/hash_map.h>
#include "placeable_model.h"

#if __GNUC__ < 4 || (__GNUG__ == 4 && __GNUC_MINOR__ < 4)

// allow to use placeable_model pointer as key in a hashmap when we do not have
// access tr1::unordered_map
namespace std
{
    template<> struct equal_to<world::placeable_model*>
    {
        bool operator()(const world::placeable_model *a, const world::placeable_model *b) const 
        {
            return a == b;
        }
    };
}

namespace __gnu_cxx 
{
    template<> struct hash<world::placeable_model*>
    {
        size_t operator() (const world::placeable_model *m) const 
        { 
            return (size_t) m; 
        }
    };
}

#endif

#include "chunk_info.h"

using world::chunk_info;


void chunk_info::add_shadow (world::placeable_model* model, const world::shadow_info & s)
{
    Shadow[model].push_back (s);
}

void chunk_info::remove_shadow (const s_int32 & x, const s_int32 & y) 
{ 
    std::hash_map<world::placeable_model*, std::vector<world::shadow_info> >::iterator i;
    std::vector<world::shadow_info>::iterator shdw;
    
    for (i = Shadow.begin(); i != Shadow.end(); i++)
    {
        for (shdw = i->second.begin(); shdw != i->second.end(); shdw++)
        {
            if (shdw->X == x && shdw->Y == y)
            {
                i->second.erase (shdw);
                return;
            }
        }
    }
}

// get shadows cast on given part of this object
const std::vector<world::shadow_info> *chunk_info::get_shadow (world::placeable_model* model)
{
    return &Shadow[model];
}
