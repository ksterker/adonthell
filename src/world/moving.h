/*
 $Id: moving.h,v 1.13 2009/01/26 21:09:14 ksterker Exp $
 
 Copyright (C) 2002 Alexandre Courbot <alexandrecourbot@linuxgames.com>
 Copyright (C) 2009 Kai Sterker <kai.sterker@gmail.com>
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
 * @author Kai Sterker <kai.sterker@gmail.com>
 * 
 * @brief  Declares the moving class.
 * 
 * 
 */


#ifndef WORLD_MOVING_H
#define WORLD_MOVING_H

#include "world/chunk_info.h"

#include "gfx/surface.h"

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
         * Destructor.
         */
        virtual ~moving ();
        
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
         * Set location on x/y plane.
         * @param x new x position
         * @param y new y position
         */
        void set_position (const s_int32 & x, const s_int32 & y);         
        
        /**
         * Set altitude.
         * @param z new z position
         */
        void set_altitude (const s_int32 & z); 

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
        virtual bool update (); 

        /**
         * When compiled with -DDEBUG_COLLISION, calling this method
         * before blitting a frame to the screen will create an overlay with
         * some helpful information for debugging collision detection.
         * Otherwise it is a noop.
         */
        void debug_collision (const u_int16 & x, const u_int16 & y) const;

#ifndef SWIG
        /**
         * Allow %moving to be passed as python argument
         */
        GET_TYPE_NAME_VIRTUAL (world::moving)
#endif
            
    protected:
        /**
         * Update position on the map.
         */
        void update_position ();
        
        /**
         * Find the z-position of the ground under the movable.
         */
        void calculate_ground_pos ();
        
        /**
         * Try to move from given point with given velocity, colliding with
         * objects on the map and bouncing back in the process. Called recursively
         * up to a certain depth.
         *
         * @param collisionData data of performed move.
         * @param depth current recursion depth
         *
         * @return final position.
         */
        vector3<float> execute_move (collision *collisionData, u_int16 depth = 0); 
        
        /**
         * Check whether the performed move collides with any object on the map.
         * @param collisionData data of performed move.
         * @return true if collision occured, false otherwise.
         *
         * @todo would be more efficient when filtering objects according to vertical 
         * position and when comparing objects that span multiple squares only once.
         */
        bool collide_with_objects (collision *collisionData);
        
        /// Altitude of ground under the object (for drawing shadow)
        s_int32 GroundPos;
        
        /// precise position of object
        vector3<float> Position;
        /// velocites along the 3 axis in world space.
        vector3<float> Velocity;
        
    private:
        /// 
        std::list<chunk_info> GroundTiles;
        /// for debugging
        gfx::surface *Image;
    }; 
}

#endif
