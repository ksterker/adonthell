/*
 $Id: render_info.h,v 1.10 2009/02/07 21:47:10 ksterker Exp $
 
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

#include "gfx/sprite.h"
#include "world/plane3.h"
#include "world/placeable_shape.h"
#include "world/shadow.h"

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
     * @param shdw the shadow cast onto this object
     * @param pos the position of the object in world-space
     */
    render_info (const placeable_shape *shape, const gfx::sprite *sprite, const vector3<s_int32> & pos, const std::vector<shadow_info> *shdw) 
    : Pos (pos), Shape (shape), Sprite (sprite), Shadow (shdw)
    {
        Projection[0] = x();
        Projection[1] = y() - z() - shape->height();
        Projection[2] = x() + shape->length();
        Projection[3] = y() - z() + shape->width();
    }
    
    render_info (const render_info & ri)
    : Pos (ri.Pos), Shape (ri.Shape), Sprite (ri.Sprite), Shadow (ri.Shadow)
    {
        Projection[0] = ri.Projection[0];
        Projection[1] = ri.Projection[1];
        Projection[2] = ri.Projection[2];
        Projection[3] = ri.Projection[3];
    }
    
    s_int32 x () const
    {
        return Pos.x() + Shape->x();
    }

    s_int32 y () const
    {
        return Pos.y() + Shape->y();
    }

    s_int32 z () const
    {
        return Pos.z() + Shape->z();
    }

    s_int32 min_x () const
    {
        return Projection[0];
    }
    
    s_int32 min_yz () const
    {
        return Projection[1];
    }

    s_int32 max_x () const
    {
        return Projection[2];
    }

    s_int32 max_yz () const
    {
        return Projection[3];
    }
    
    /// position of object in world space
    vector3<s_int32> Pos;
    /// the object's bounding box 
    const placeable_shape *Shape;
    /// the object's graphical representation
    const gfx::sprite *Sprite;
    /// shadows cast onto this object
    const std::vector<shadow_info> *Shadow;
    
private:
    /// the 2D projection of the object
    s_int32 Projection[4];
};

}

#endif // WORLD_RENDERINFO_H
