/*
 $Id: render_info.h,v 1.11 2009/02/08 13:25:54 ksterker Exp $
 
 Copyright (C) 2008/2009 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file   world/render_info.h
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Declares the render info class.
 * 
 */

#ifndef WORLD_RENDERINFO_H
#define WORLD_RENDERINFO_H

#include <adonthell/gfx/sprite.h>
#include "placeable_shape.h"
#include "shadow.h"

namespace world
{

/**
 * Structure holding meta-data required to render a surface.
 * Its main use is sorting a list of surfaces for correct
 * drawing order.
 */
class render_info
{
public:
    /**
     * Create render data for given object.
     * @param shape the physical representation of the object
     * @param sprite the graphical representation of the object
     * @param pos the position of the object in world-space
     * @param shdw the shadow cast onto this object
     */
    render_info (const placeable_shape *shape, const gfx::sprite *sprite, const vector3<s_int32> & pos, const std::vector<shadow_info> *shdw) 
    : Pos (pos), Shape (shape), Sprite (sprite), Shadow (shdw)
    {
        Projection[0] = x() /*+ shape->ox()*/;
        Projection[1] = y() /*+ shape->oy()*/ - z() - shape->height();
        Projection[2] = x() /*+ shape->ox()*/ + shape->length();
        Projection[3] = y() /*+ shape->oy()*/ - z() + shape->width();
    }
    
    /**
     * Create copy of render data.
     * @param ri the render data to duplicate.
     */
    render_info (const render_info & ri)
    : Pos (ri.Pos), Shape (ri.Shape), Sprite (ri.Sprite), Shadow (ri.Shadow)
    {
        Projection[0] = ri.Projection[0];
        Projection[1] = ri.Projection[1];
        Projection[2] = ri.Projection[2];
        Projection[3] = ri.Projection[3];
    }
    
    /**
     * @name Sprite position
     *
     * Access to the location where sprite needs to be
     * rendered on the drawing %surface.
     */
    ///@{
    /**
     * Return real world space position of object.
     * @return object x-coordinate. 
     */
    s_int32 x () const
    {
        return Pos.x() + Shape->x();
    }

    /**
     * Return real world space position of object.
     * @return object y-coordinate. 
     */
    s_int32 y () const
    {
        return Pos.y() + Shape->y();
    }

    /**
     * Return real world space position of object.
     * @return object z-coordinate. 
     */
    s_int32 z () const
    {
        return Pos.z() + Shape->z();
    }
    
    /**
     * Return screen position of sprite for drawing.
     * @return x-coordinate projected to screen.
     */
    s_int32 screen_x () const
    {
        return Projection[0] + Shape->ox();
    }
    
    /**
     * Return screen position of sprite for drawing.
     * @return y-coordinate projected to screen.
     */
    s_int32 screen_y () const
    {
        return Projection[1] + Shape->oy();
    }
    ///@}
    
    /**
     * @name Sprite projection.
     *
     * Gain access to the rectangle covered when projecting the
     * sprite onto the drawing %surface.
     */
    ///@{
    /**
     * Return lower x coordinate of projected sprite.
     * @return sprite lower x-coordinate. 
     */
    s_int32 min_x () const
    {
        return Projection[0];
    }
    
    /**
     * Return lower y coordinate of projected sprite.
     * @return sprite lower y-coordinate. 
     */
    s_int32 min_yz () const
    {
        return Projection[1];
    }

    /**
     * Return upper x coordinate of projected sprite.
     * @return sprite upper x-coordinate. 
     */
    s_int32 max_x () const
    {
        return Projection[2];
    }

    /**
     * Return upper y coordinate of projected sprite.
     * @return sprite upper y-coordinate. 
     */
    s_int32 max_yz () const
    {
        return Projection[3];
    }
    ///@}
    
    /// position of object in world space
    vector3<s_int32> Pos;
    /// the object's bounding box 
    const placeable_shape *Shape;
    /// the object's graphical representation
    const gfx::sprite *Sprite;
    /// shadows cast onto this object
    const std::vector<shadow_info> *Shadow;
    
private:
    /// the 2D projection of the object onto the drawing surface
    s_int32 Projection[4];
};

}

#endif // WORLD_RENDERINFO_H
