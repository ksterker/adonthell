/*
 $Id: render_info.h,v 1.3 2008/10/10 20:37:35 ksterker Exp $
 
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
        Pos.set (Pos.x() + Shape->x(), Pos.y() + Shape->y(), Pos.z() + Shape->z());
    }
    
    /**
     * Compare two render_info objects based on their render sequence. 
     * This is used to sort the render queue prior to rendering.
     * @param ri render_info object to compare to this one.
     * @return true if this needs to be rendered first, false otherwise.
     */
    bool operator < (const render_info & ri) const
    {
        if (z_pos() <= ri.Pos.z()) return true;
        if (Pos.z() >= ri.z_pos()) return false;
        
        return Pos.y() + Shape->width() < ri.Pos.y() + ri.Shape->width();
    }

    /**
     * Return the Z coordinate of the object.
     * @param the Z position of the object.
     */
    s_int32 z_pos () const
    {
        return Pos.z() + Shape->z() + Shape->height();
    }

    /// position of object in world space
    vector3<s_int32> Pos;
    /// the object's bounding box 
    const placeable_shape *Shape;
    /// the object's graphical representation
    const gfx::sprite *Sprite;
    
private:
    /// forbid copy construction
    // render_info (const render_info & ri);
};

}

#endif // WORLD_RENDERINFO_H
