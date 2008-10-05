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
    render_info (const placeable_shape *shape, const gfx::sprite * sprite, const vector3<s_int32> & pos) 
    : Pos (pos), Shape (shape), Sprite (sprite)
    {
        Pos.set (Pos.x() + Shape->x(), Pos.y() + Shape->y(), Pos.z() + Shape->z());
    }
    
    bool operator < (const render_info & ri) const
    {
        if (Pos.z() + Shape->height() <= ri.Pos.z()) return true;
        if (Pos.z() >= ri.Pos.z() + Shape->height()) return false;
        
        return Pos.y() < ri.Pos.y();
    }
    
    ///
    vector3<s_int32> Pos;
    ///
    const placeable_shape *Shape;
    ///
    const gfx::sprite *Sprite;
    
private:
    /// forbid copy construction
    // render_info (const render_info & ri);
};

}

#endif // WORLD_RENDERINFO_H
