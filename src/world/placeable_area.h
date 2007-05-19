/*
 $Id: placeable_area.h,v 1.1 2007/05/19 07:42:10 ksterker Exp $
 
 Copyright (C) 2002 Alexandre Courbot <alexandrecourbot@linuxgames.com>
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
 * @file   world/placeable_area.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the placeable_area class.
 * 
 * 
 */


#ifndef WORLD_PLACEABLE_AREA_H_
#define WORLD_PLACEABLE_AREA_H_

#include <vector>

#include "world/coordinates.h"
#include "base/file.h"

namespace world
{
    const int square_size = 40;

    /**
     * Walkability info class.
     * 
     */
    class square_walkable_info
    {
    private:
        bool Walkable;

    public:
        square_walkable_info () 
        {
            Walkable = true;  
        }

        bool is_walkable () const
        {
            return Walkable; 
        }

        void set_walkable (bool b) 
        {
            Walkable = b; 
        }

        void put(base::ogzstream & file) const
        {
            Walkable >> file;
        }

        void get(base::igzstream & file)
        {
            Walkable << file;
        }

        bool does_intersect(const square_walkable_info & mwi, const u_int16 x, const u_int16 oy) const
        {
            return (is_walkable() && mwi.is_walkable());
        }
    }; 

    /**
     * Area of squares that represents the occupation of a placeable
     * on the map.
     * 
     */
    class placeable_area
    {
    private:
        std::vector <std::vector <square_walkable_info> > area;
    
    public:
        u_int16 zsize;
    
        coordinates base; 

        placeable_area()
        {
            zsize = 0;
        }

        u_int16 area_length() const
        {
            return area.size ();
        }
    
        u_int16 area_height() const;

        void set_area_size(u_int16 nx, u_int16 ny);

        square_walkable_info & get(u_int16 x, u_int16 y) 
        {
            return area[x][y]; 
        }

        bool update() 
        {
            return true; 
        }

        void put(base::ogzstream & file);

        void get(base::igzstream & file);
    }; 
}

#endif

