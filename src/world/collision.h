/*
 $Id: collision.h,v 1.8 2008/10/03 17:16:24 ksterker Exp $
 
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
 * @file   world/collision.h
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Defines the 3D collision class.
 * 
 */

#ifndef WORLD_COLLISION_H
#define WORLD_COLLISION_H

#include "triangle3.h"

namespace world
{
/**
 * Collision detection. Based on swept sphere algorithm described by 
 * Kasper Fauerby at http://www.peroxide.dk . Version from 25th July 2003.
 */
class collision
{
public:
    /**
     * Create new collision object.
     * @param position initial position.
     * @param velocity initial velocity.
     * @param radius the radius of the swept sphere. 
     */
    collision (const vector3<float> & position, const vector3<float> & velocity, const vector3<float> & radius);
    
    /**
     * Destructor.
     */
    ~collision ();
    
    /**
     * Test whether collision occurs with given triangle. If so, update members
     * #CollisionFound, #NearestDistance and #IntersectionPoint accordingly.
     *
     * @param triangle triangle to check collision against.
     * @param offset position of triangle on world map.
     */
    void check_triangle (const triangle3<s_int16> & triangle, const vector3<s_int16> & offset);

    /**
     * @name Member access
     */
    //@{
    /**
     * Check whether a collision was detected.
     * @return \b true if a collision was detected, \b false otherwise.
     */
    bool collision_found () const { return CollisionFound; }
    
    /**
     * Get the distance to the collision point. Valid if collision was found.
     * @return the distance to the closest collision point.
     */
    double distance () const { return NearestDistance; }
    
    /**
     * Get the point where the collision occured.
     * @return the point where the collision occured.
     */
    vector3<float> intersection () const { return IntersectionPoint; }

    /**
     * Change intersection point by given difference.
     * @param difference value to substract from intersection point.
     */
    void update_intersection (const vector3<float> & difference)
    {
        IntersectionPoint = IntersectionPoint - difference;
    }

    /**
     * Set whether object is falling.
     * @param is_falling true or false.
     */
    void set_falling (const bool & is_falling)
    {
        IsFalling = is_falling;
    }
    
    /**
     * Check whether the object is falling
     * @param true if it is, false otherwise.
     */
    bool is_falling () const
    {
        return IsFalling;
    }
    
    /**
     * Get the velocity of planned move.
     * @return the velocity of planned move.
     */
    vector3<float> velocity () const { return Velocity; }

    /**
     * Get the starting position of planned move.
     * @return the starting position of planned move.
     */
    vector3<float> position () const { return BasePoint; }
    
    /**
     * Change movement information for subsequent collision iterations.
     * @param position new position.
     * @param velocity new velocity.
     */
    void update_movement (const vector3<float> & position, const vector3<float> & velocity);
        
    /**
     * Get the triangle we collided against.
     * @return the triangle we collided against.
     */
    const triangle3<float> * triangle () const;
    //@}
    
private:
    /**
     * Solves the equation ax^2 + by + c = 0. Returns true if a solution greater 
     * zero and less than threshold exists. In that case, the solution is returned
     * in the result parameter.
     *
     * @param a 1st parameter of quadric equation
     * @param b 2nd parameter of quadric equation
     * @param c 3rd parameter of quadric equation
     * @param threshold maximum size of result considered successful
     * @param result contains the result, if successful
     *
     * @return \c true if a solution greater zero and less than threshold exists.
     */
    bool solve_quadric_equation (const float & a, const float & b, const float & c, const float & threshold, float* result) const;
    
    /// Radius of sphere in ellipse space
    vector3<float> Radius;
    
    /**
     * @name Planned Move
     */
    //@{
    /// velocity in ellipse space
    vector3<float> Velocity; 
    /// normalized velocity in ellipse space
    vector3<float> NormalizedVelocity; 
    /// starting point of the move in ellipse space
    vector3<float> BasePoint; 
    //@}
    
    /**
     * @name Results of Move
     */
    //@{
    /// whether character has Z-velocity != 0
    bool IsFalling;
    /// whether collision was detected 
    bool CollisionFound; 
    /// distance between start and intersection point
    double NearestDistance; 
    /// point where the collision occurs
    vector3<float> IntersectionPoint;
    //@}
    
    /// Triangle that caused collision
    const triangle3<float> *Triangle;
};

}

#endif // WORLD_COLLISION_H
