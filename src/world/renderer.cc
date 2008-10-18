/*
 $Id: renderer.cc,v 1.1 2008/10/18 12:41:18 ksterker Exp $
 
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
 * @file   world/renderer.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Defines a part of the map visible on screen.
 * 
 */

#include "base/timer.h"
#include "gfx/screen.h"
#include "world/renderer.h"

namespace world
{
    
// default rendering
void default_renderer::render (const s_int16 & x, const s_int16 & y, const std::list <world::render_info> & queue, const gfx::drawing_area & da, gfx::surface * target) const
{
    for (std::list <world::render_info>::const_iterator it = queue.begin (); it != queue.end (); it++)
    {
        it->Sprite->draw (x + it->x (), y + it->y () - it->z() - it->Shape->height(), &da, target);
    }
}

// debug rendering
void debug_renderer::render (const s_int16 & x, const s_int16 & y, const std::list <world::render_info> & queue, const gfx::drawing_area & da, gfx::surface * target) const
{
    for (std::list <world::render_info>::const_iterator it = queue.begin (); it != queue.end (); it++)
    {
        it->Sprite->draw (x + it->x (), y + it->Pos.y () - it->z() - it->Shape->height(), &da, target);
        
        if (DrawBBox)
        {
            cube3 bbox (it->Shape->length(), it->Shape->width(), it->Shape->height());
            bbox.draw (x + it->x(), y + it->y () - it->z(), &da, target);
        }
        
        if (Print)
        {
            printf ("Pos [%i, %i, %i] ", it->Pos.x(), it->Pos.y(), it->Pos.z());
            printf ("Shape [%i, %i, %i] - [%i, %i, %i] ", it->Shape->x(), it->Shape->y(), it->Shape->z(),
                    it->Shape->x() + it->Shape->length(), it->Shape->y() + it->Shape->width(), it->Shape->z() + it->Shape->height());
            printf ("Sprite [%i x %i] %c\n", it->Sprite->length(), it->Sprite->height(), (it->IsFlat ? '_' : '|'));
        }
        
        if (Delay > 0)
        {
            // debug rendering by drawing one tile at a time
            base::timer::sleep (Delay);
            gfx::screen::update ();
        }        
    }
}

}