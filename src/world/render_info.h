/*
 $Id: render_info.h,v 1.7 2008/11/09 14:07:40 ksterker Exp $
 
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
 * @file   world/render_info.h
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Declares the render info class.
 * 
 */

#ifndef WORLD_RENDERINFO_H
#define WORLD_RENDERINFO_H

#include "gfx/sprite.h"
#include "world/plane3.h"
#include "world/placeable_shape.h"

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
     */
    render_info (const placeable_shape *shape, const gfx::sprite * sprite, const vector3<s_int32> & pos) 
    : Pos (pos), Shape (shape), Sprite (sprite)
    {
        if (shape->is_flat())
        {
            // flat tiles are sorted Z, Y, X
            OrderValue[0] = z();
            OrderValue[1] = y();
            OrderValue[2] = x();
        }
        else
        {
            // flat tiles are sorted Y, X, Z
            OrderValue[0] = y(); 
            OrderValue[1] = x();
            OrderValue[2] = z();
        }
    }
    
    /**
     * Compare two render_info objects based on their render sequence. 
     * This is used to sort the render queue prior to rendering.
     * @param ri render_info object to compare to this one.
     * @return true if this needs to be rendered first, false otherwise.
     */
    bool operator < (const render_info & ri) const
    {
        if (OrderValue[0] == ri.OrderValue[0])
        {
            if (OrderValue[1] == ri.OrderValue[1])
            {
                return OrderValue[2] < ri.OrderValue[2];
            }
            return OrderValue[1] < ri.OrderValue[1];
        }
        return OrderValue[0] < ri.OrderValue[0];
    }
    
    s_int32 x () const
    {
        return Pos.x() + Shape->x();
    }

    s_int32 y () const
    {
        return Pos.y() - Shape->y();
    }

    s_int32 z () const
    {
        return Pos.z() + Shape->z();
    }

    /// position of object in world space
    vector3<s_int32> Pos;
    /// the object's bounding box 
    const placeable_shape *Shape;
    /// the object's graphical representation
    const gfx::sprite *Sprite;
    
private:
    s_int32 OrderValue[3];
        
    /// forbid copy construction
    // render_info (const render_info & ri);
};

}

#endif // WORLD_RENDERINFO_H
