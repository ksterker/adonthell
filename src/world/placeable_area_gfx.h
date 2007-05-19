/*
 $Id: placeable_area_gfx.h,v 1.1 2007/05/19 07:42:10 ksterker Exp $
 
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
 * @file   world/placeable_area_gfx.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the placeable_area_gfx class.
 * 
 * 
 */

#ifndef WORLD_PLACEABLE_AREA_GFX
#define WORLD_PLACEABLE_AREA_GFX

#include "base/file.h"
#include "gfx/animation.h"
#include "world/placeable_area.h"

namespace world
{
    /**
     * Graphics representing a state of a placeable_model.
     * 
     */
    class placeable_area_gfx : public gfx::drawable
    {
    private:
        gfx::animation * anim;

    public:
        placeable_area_gfx();

        ~placeable_area_gfx();

        gfx::animation * get_animation()
        {
            return anim;
        }

        void play()
        {
            anim->play();
        }

        void stop()
        {
            anim->stop();
        }

        void rewind()
        {
            anim->rewind();
        }

        bool update() 
        {
            anim->update ();
            return true; 
        }

        void draw(s_int16 x, s_int16 y, const gfx::drawing_area * da_opt = NULL,
                  gfx::surface * target = NULL) const;
    
        void set_area_size(u_int16 nx, u_int16 ny);

        void put(base::ogzstream & file) const
        {
            // anim->put(file);
        }

        void get(base::igzstream & file)
        {
            // anim->get(file);
        }
        
#ifndef SWIG
        GET_TYPE_NAME_VIRTUAL(world::placeable_area_gfx)
#endif // SWIG            
    }; 
}

#endif
