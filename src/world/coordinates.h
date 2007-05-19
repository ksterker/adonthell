/*
 $Id: coordinates.h,v 1.1 2007/05/19 07:42:09 ksterker Exp $
 
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
 * @file   world/coordinates.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the coordinates class.
 * 
 * 
 */

#ifndef WORLD_COORDINATES_H
#define WORLD_COORDINATES_H

#include "base/types.h"

namespace world
{
    /**
     * World space representation of any object.
     */
    class coordinates
    {
    protected :
        u_int16 X, Y;
        s_int32 Z;
        u_int16 Ox, Oy;

    public:
        coordinates ();
        coordinates (u_int16 x, u_int16 y, u_int16 z, u_int16 ox = 0, u_int16 oy = 0); 

        u_int16 x () const
        {
            return X; 
        }

        u_int16 y () const
        {
            return Y; 
        }

        s_int32 z() const
        {
            return Z;
        }

        u_int16 ox () const
        {
            return Ox; 
        }

        u_int16 oy () const
        {
            return Oy; 
        }
    
        void set_position (u_int16 x, u_int16 y) 
        {
            X = x;
            Y = y; 
        }

        void set_altitude(s_int32 z)
        {
            Z = z;
        }

        void set_offset (u_int16 ox, u_int16 oy) 
        {
            Ox = ox;
            Oy = oy; 
        }
    
        bool operator == (const coordinates & mc); 
        bool operator < (const coordinates & mc); 
        bool operator <= (const coordinates & mc); 
        bool operator > (const coordinates & mc); 
        bool operator >= (const coordinates & mc); 

        coordinates & operator + (const coordinates & mc); 
        coordinates & operator - (const coordinates & mc); 
    }; 
}

#endif
