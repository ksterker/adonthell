/*
 $Id: placeable_gfx.h,v 1.1 2007/05/19 07:42:10 ksterker Exp $
 
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
 * @file   world/placeable_gfx.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the placeable_gfx class.
 * 
 * 
 */


#ifndef WORLD_PLACEABLE_GFX_H
#define WORLD_PLACEABLE_GFX_H

#include <map>

#include "world/placeable_model_gfx.h"
#include "world/placeable.h"

namespace world
{

    /**
     * The graphical representation of a placeable.
     *
     * Each object of this class has to be associated at constructor
     * time with a placeable_model that it will stick to all life long.
     * 
     */
    class placeable_gfx : public placeable_model_gfx
    {
    private:
        placeable & Target;

    public:
        placeable_gfx(placeable & target);
    }; 
}

#endif
