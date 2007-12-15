/*
 $Id: moving.h,v 1.5 2007/12/15 23:15:10 ksterker Exp $
 
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
#include "world/coordinates.h"

namespace world
{
    /**
     * Implements a map moving object.
     *
     * While placeable can occupy space on a map, it does not have the ability to move.
     * Objects inheriting from this class can be placed on a map and have a certain
     * velocity, resulting in a path that they will follow each time they are updated.
     */
    class moving : public placeable, public coordinates
    {
    public:

        /**
         * Create a new movable object on the given map.
         * @param area the map this object belongs to.
         */
        moving (area & mymap); 

        /**
         * @name Member Access
         */
        //@{
        /**
         * Return x velocity (in pixels).
         * @return x velocity
         */
        float vx () const
        {
            return Velocity.x(); 
        }

        /**
         * Return y velocity (in pixels)
         * @return y velocity
         */
        float vy () const
        {
            return Velocity.y(); 
        }

        /**
         * Return z velocity (in pixels)
         * @return z velocity
         */        
        float vz() const
        {
            return Velocity.z();
        }

        /**
         * Return movement limit in x direction (in squares)
         * @return movement limit in x direction
         */
        u_int16 lx () const
        {
            return Lx;
        }

        /**
         * Return movement limit in x direction (in squares)
         * @return movement limit in x direction
         */
        u_int16 ly () const
        {
            return Ly; 
        }
        
        /**
         * Get Position of ground under object.
         * @return position of ground under object.
         */
        s_int32 ground_pos () const
        {
            return GroundPos;
        }
        //@}
        
        /**
         * @name Member Manipulation
         */
        //@{
        /**
         * Set offset.
         * @param ox new x offset
         * @param oy new y offset
         */
        void set_offset (const u_int16 & ox, const u_int16 & oy); 

        /**
         * Set altitude.
         * @param z new z position
         */
        void set_altitude (const s_int32 & z); 
        
        /**
         * Set limit of movement. Should not be bigger than the extend
         * of the map this object is on (but could be smaller).
         * @param mx new maximum x limit
         * @param my new maximum y limit
         */
        void set_limits (const u_int16 & mx, const u_int16 & my); 

        /**
         * Set valocity on x/y plane.
         * @param vx new x velocity
         * @param vy new y velocity
         */
        void set_velocity (const float & vx, const float & vy); 

        /**
         * Set valocity along z axis.
         * @param vz new z velocity
         */
        void set_vertical_velocity (const float & vz);
        //@}
        
        /**
         * Called by the engine every cycle to update the position of this
         * object, depending on its velocity and obstacles on the map.
         * @return always true.
         */
        bool update (); 

    protected:
        /**
         * Update position on the map.
         */
        void update_position ();
        
        /**
         * Try to move from given point with given velocity, colliding with
         * objects on the map and bouncing back in the process. Called recursively
         * up to a certain depth.
         *
         * @param pos initial position
         * @param vel initial velocity
         * @param depth current recursion depth
         *
         * @return final position.
         */
        vector3<float> execute_move (const vector3<float> & pos, const vector3<float> & vel, u_int16 depth = 0); 
        
        /**
         * Check whether the performed move collides with any object on the map.
         * @param collisionData data of performed move.
         * @return true if collision occured, false otherwise.
         *
         * @todo would be more efficient when filtering objects according to vertical 
         * position and when comparing objects that span multiple squares only once.
         */
        bool collide_with_objects (collision *collisionData);
        
        /// do not allow movement outside the rectangle (0, 0) - (Lx, Ly) 
        u_int16 Lx, Ly;
        
        /// Altitude of ground under the object (for drawing shadow)
        s_int32 GroundPos;
        
        /// precise position of object, relative to current tile (X, Y)
        vector3<float> Position;
        /// velocites along the 3 axis in world space.
        vector3<float> Velocity;
    }; 
}

#endif
