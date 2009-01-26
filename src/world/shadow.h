/*
 $Id: shadow.h,v 1.1 2009/01/26 21:09:15 ksterker Exp $
 
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
 * @file   world/shadow.h
 * @author Kai Sterker <kai.sterker@gmail.com>
 * 
 * @brief  Declares the shadow class.
 * 
 */

#ifndef WORLD_SHADOW_H
#define WORLD_SHADOW_H

#include <list>
#include "gfx/surface.h"
#include "world/coordinates.h"

namespace world
{
    
/**
 * Represents a character shadow that will be drawn on tiles
 * below the character. It requires an (optionally masked) image
 * with per-surface alpha (so no alpha channel) that will 
 * represent the shadow. Depending on the distance between
 * character and ground, the opacity of the shadow will be
 * changed.
 */
class shadow
{
public:
    /**
     * Create a new shadow object.
     * @param shadow filename of the shadow image.
     * @param pos position of the object that casts the shadow.
     */
    shadow (const std::string & shadow, const coordinates * pos);

    /**
     * Destroy shadow object.
     */
    ~shadow ();
    
    /**
     * Clear shadow object for rendering next frame.
     */
    void reset ();
    
    /**
     * Draw part of shadow specified by given drawing area onto given target.
     * @param pos position of placeable to draw shadow on.
     * @param da surface area of placeable.
     * @param target surface to draw on.
     */
    void draw (const vector3<s_int32> & pos, gfx::drawing_area * da, gfx::surface * target);
    
protected:
    void subtract_area (const gfx::drawing_area & a, const gfx::drawing_area & b);

private:
    /// current position of shadow casting object
    const coordinates *Pos;
    /// image representing the shadow
    gfx::surface *Shadow;
    /// region still covered by the shadow
    std::list<gfx::drawing_area> Areas;
};

}

#endif
