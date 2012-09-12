/*
 Copyright (C) 2009/2011 Kai Sterker <kai.sterker@gmail.com>
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
 * @file   world/shadow.cc
 * @author Kai Sterker <kai.sterker@gmail.com>
 * 
 * @brief  Implements the shadow class.
 * 
 */

#include <adonthell/gfx/surface_cacher.h>
#include "shadow.h"
#include "chunk_info.h"
#include <adonthell/base/logging.h>

using gfx::drawing_area;
using world::shadow;
using world::chunk_info;

// ctor
shadow::shadow (const std::string & shadow, const coordinates *pos, const vector3<s_int32> & offset)
{
    Shadow = gfx::surfaces->get_surface_only (shadow, true, false);
    Offset = offset;
    Pos = pos;
    init ();
}

// dtor
shadow::~shadow ()
{
    gfx::surfaces->free_surface (Shadow);
    Shadow = NULL;
}

// init at the beginning of each frame
void shadow::init ()
{
    // prepare shadow for next frame
    Remaining.clear();
    drawing_area area (Pos->x() + Offset.x(), Pos->y() + Offset.y(), Shadow->length(), Shadow->height());
    Remaining.push_back (area);
}

// cleanup at the end of each frame
void shadow::reset ()
{
    // prepare shadow for next frame
    s_int32 x = Pos->x() + Offset.x();
    s_int32 y = Pos->y() + Offset.y();

    // clean tiles with shadow on them
    for (std::vector<chunk_info*>::const_iterator i = TilesWithShadow.begin(); i != TilesWithShadow.end(); i++)
    {
        (*i)->remove_shadow(x, y);
    }

    TilesWithShadow.clear();
}

// cast shadow on a "floor" object
void shadow::cast_on (chunk_info* ci)
{
    VLOG(4) << Remaining.size() << " " << ci << " " << ci->Min << "-" << ci->Max;

    // are there parts of the shadow remaining at all?
    if (Remaining.size() > 0)
    {
        const placeable *object = ci->get_object();
        
        // cast shadow on scenery only
        if (object->type() != world::OBJECT) return;
        
        // distance between object and its shadow
        s_int32 distance = Pos->z() - ci->solid_max().z();
        VLOG(4) << "Total distance " << distance;

        for (placeable::iterator i = object->begin(); i != object->end(); i++)
        {
            const world::placeable_shape *shape = (*i)->current_shape();
            if (!shape || !shape->is_solid()) continue;

            s_int32 shape_distance = Pos->z() - ci->center_min().z() - shape->z() - shape->height();
            VLOG(4) << "Shape distance " << shape_distance;

            // this seems a sensible thing to do, but it will break
            // the shadow on sloped surfaces, where the character will
            // be located inside the bounding box, below the top.
            // if (shape_distance < 0) continue;

            // data for rendering shadow later on
            shadow_info si (Pos->x() + Offset.x(), Pos->y() + Offset.y(), Shadow, shape_distance);

            // floor surface area
            drawing_area obj_surface (ci->Min.x() + shape->x(),
                                      ci->Min.y() + shape->y(),
                                      shape->length(), shape->width());
        
            // check remaining shadow areas for overlap with floor ...
            for (shadow::parts::iterator area = Remaining.begin(); area != Remaining.end(); /* nothing */)
            {
                bool no_overlap =
                        area->x() >= obj_surface.x() + obj_surface.length() ||
                        area->y() >= obj_surface.y() + obj_surface.height() ||
                        area->x() + area->length() <= obj_surface.x() ||
                        area->y() + area->height() <= obj_surface.y();

                if (no_overlap)
                {
                    area++;
                }
                else
                {
                    // store area for later rendering
                    area->assign_drawing_area (&obj_surface);
                    si.Area.push_back (area->setup_rects());

                    // remove overlapping area from shadow
                    drawing_area::subtract_area (Remaining, *area, obj_surface);
                    area = Remaining.erase (area);
                }
            }
        
            // is shadow cast on floor at all?
            if (si.Area.size() > 0)
            {
                // assign shadow to floor ...
                ci->add_shadow (*i, si);
                // ... and remember for later cleanup
                TilesWithShadow.push_back (ci);
            }
        }
    }
}
