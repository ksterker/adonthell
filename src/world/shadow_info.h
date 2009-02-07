/*
 $Id: shadow_info.h,v 1.1 2009/02/07 21:47:10 ksterker Exp $
 
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
 * @file   world/shadow_info.h
 * @author Kai Sterker <kai.sterker@gmail.com>
 * 
 * @brief  Declares the moving class.
 * 
 * 
 */

#ifndef WORLD_SHADOW_INFO_H
#define WORLD_SHADOW_INFO_H

#include <list>
#include "gfx/surface.h"

namespace world
{
    
/**
 * Container class for date required to render shadow onto a placeable.
 */
class shadow_info
{
public:
    /**
     * Store information about shadow.
     * @param image graphical representation of shadow.
     * @param distance distance between object and its shadow.
     */
    shadow_info (const s_int32 & x, const s_int32 & y, const gfx::surface *image, const u_int32 & distance)
    {
        X = x;
        Y = y;
        Image = (gfx::surface *) image;
        Distance = distance;
    }

    /**
     * Copy constructor.
     * @param shadow info structure to create a copy of.
     */
    shadow_info (const shadow_info & si) : X (si.X), Y (si.Y), Distance (si.Distance), Image (si.Image)
    {
        Area = si.Area;
    }

    /// x-coordinate of shadow
    s_int32 X;
    /// y-coordinate of shadow
    s_int32 Y;
    /// distance between object and its shadow
    s_int32 Distance;
    /// image representing the shadow
    gfx::surface *Image;
    /// parts of the shadow to render
    std::list<gfx::drawing_area> Area;
};

}

#endif
