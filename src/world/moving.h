/*
 $Id: moving.h,v 1.1 2007/05/19 07:42:09 ksterker Exp $
 
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
 * @file   world/moving.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the moving class.
 * 
 * 
 */


#ifndef WORLD_MOVING_H
#define WORLD_MOVING_H

#include "world/placeable.h"

namespace world
{
    /**
     * Implements a map moving object.
     *
     * While placeable can occupy space on a map, it has not the ability to move.
     * Objects inheriting from this class can be placed on a map and be given a certain
     * velocity, that they will follow each time they are updated.
     * 
     */
    class moving : public placeable, public coordinates
    {
    public:
        s_int32 zground;

        moving (area & mymap); 

        float vx () 
        {
            return Vx; 
        }

        float vy () 
        {
            return Vy; 
        }

        float vz()
        {
            return Vz;
        }

        u_int16 climb_capability()
        {
            return Climb_capability;
        }

        u_int16 lx () 
        {
            return Lx;
        }

        u_int16 ly () 
        {
            return Ly; 
        }
    
        void set_offset (u_int16 ox, u_int16 oy); 
    
        void set_limits (u_int16 mx, u_int16 my); 

        void set_velocity (float vx, float vy); 

        void set_vertical_velocity(float vz);

        /** 
         * 
         * 
         * @bug When reaching an unreacheable area, the PREVIOUS position is restored
         *      instead of taking the position nearest to the unreacheable square.
         */
        void update_pos();

        void update_pos2();

        bool update(); 

    protected:
        /// ???
        float fox, foy, foz;
        /// velocites along the 3 axis in world space.
        float Vx, Vy, Vz;
        /// ???
        bool Has_moved;
        /// whether object has negative vertical velocity.
        bool Is_falling;
        /// Limit ???
        u_int16 Lx, Ly;
        
        /// difference in ground height an object can overcome without jumping
        static const u_int16 Climb_capability = 5;
    }; 
}

#endif

