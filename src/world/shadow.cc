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

#include "gfx/surface_cacher.h"
#include "world/shadow.h"
#include "world/chunk_info.h"
#include "base/logging.h"

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
    VLOG(1) << Remaining.size() << " " << ci << " " << ci->Min << "-" << ci->Max;

    // are there parts of the shadow remaining at all?
    if (Remaining.size() > 0)
    {
        const placeable *object = ci->get_object();
        
        // cast shadow on scenery only
        if (object->type() != world::OBJECT) return;
        
        // distance between object and its shadow
        s_int32 distance = Pos->z() - ci->solid_max().z();
        VLOG(3) << "Total distance " << distance;

        for (placeable::iterator i = object->begin(); i != object->end(); i++)
        {
            const world::placeable_shape *shape = (*i)->current_shape();
            if (!shape || !shape->is_solid()) continue;

            s_int32 shape_distance = Pos->z() - ci->center_min().z() - shape->z() - shape->height();
            VLOG(3) << "Shape distance " << shape_distance;

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
                    subtract_area (*area, obj_surface);
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

#define XL(da)   (da.x())
#define XR(da)   (da.x() + da.length())
#define YT(da)   (da.y())
#define YB(da)   (da.y() + da.height())

// remove area b from a
void shadow::subtract_area (const drawing_area & a, const drawing_area & b)
{
    int flags = a.compare (b);
    
    // gcc generates a nice jump table for the following switch...
    //
    // the dotted lines in the drawings indicated how `A' is splitted
    // into smaller rectangles.
    switch (flags) 
    {
        case XMR|YMU:
        {
            // +---+
            // | A |
            // |.+-+-+
            // | | | |
            // +-+-+ |
            //   | B |
            //   +---+
            Remaining.push_front (drawing_area (XL (a), YT (a), a.length(), YT (b) - YT (a)));
            Remaining.push_front (drawing_area (XL (a), YT (b), XL (b) - XL (a), YB (a) - YT (b)));
            break;
        }
        case XMR|YOM:
        {
            //   +---+
            //   | B |
            // +-+-+ |
            // | | | |
            // |.+-+-+
            // | A |
            // +---+
            Remaining.push_front (drawing_area (XL (a), YT (a), XL (b) - XL (a), YB (b) - YT(a)));
            Remaining.push_front (drawing_area (XL (a), YB (b), a.length(), YB (a) - YB (b)));
            break;
        }
        case XLM|YOM:
        {
            // +---+
            // | B |
            // | +-+-+
            // | | | |
            // +-+-+.|
            //   | A |
            //   +---+
            Remaining.push_front (drawing_area (XR (b), YT (a), XR (a) - XR (b), YB (b) - YT (a)));
            Remaining.push_front (drawing_area (XL (a), YB (b), a.length(), YB (a) - YB (b)));
            break;
        }
        case XLM|YMU:
        {
            //   +---+
            //   | A |
            // +-+-+.|
            // | | | |
            // | +-+-+
            // | B |
            // +---+
            Remaining.push_front (drawing_area (XL (a), YT (a), a.length(), YT (b) - YT (a)));
            Remaining.push_front (drawing_area (XR (b), YT (b), XR (a) - XR (b), YB (a) - YT (b)));
            break;
        }
        case XMR|YMM:
        {
            // +---+
            // |.+-+---+
            // |A| | B |
            // |.+-+---+
            // +---+
            Remaining.push_front (drawing_area (XL (a), YT (a), a.length(), YT (b) - YT (a)));
            Remaining.push_front (drawing_area (XL (a), YT (b), XL (b) - XL (a), b.height()));
            Remaining.push_front (drawing_area (XL (a), YB (b), a.length(), YB (a) - YB (b)));
            break;
        }
        case XMM|YOM:
        {
            //   +---+
            //   | B |
            // +-+---+-+
            // | +---+ |
            // | : A : |
            // +-------+
            Remaining.push_front (drawing_area (XL (a), YT (a), XL (b) - XL (a), a.height()));
            Remaining.push_front (drawing_area (XL (b), YB (b), b.length(), YB (a) - YB (b)));
            Remaining.push_front (drawing_area (XR (b), YT (a), XR (a) - XR (b), a.height()));
            break;
        }
        case XLM|YMM:
        {
            //     +---+
            // +---+-+.|
            // | B | |A|
            // +---+-+.|
            //     +---+
            Remaining.push_front (drawing_area (XL (a), YT (a), a.length(), YT (b) - YT (a)));
            Remaining.push_front (drawing_area (XR (b), YT (b), XR (a) - XR (b), b.height()));
            Remaining.push_front (drawing_area (XL (a), YB (b), a.length(), YB (a) - YB (b)));
            break;
        }
        case XMM|YMU:
        {
            // +-------+
            // | : A : |
            // | +---+ |
            // +-+---+-+
            //   | B |
            //   +---+
            Remaining.push_front (drawing_area (XL (a), YT (a), XL (b) - XL (a), a.height()));
            Remaining.push_front (drawing_area (XL (b), YT (a), b.length(), YT (b) - YT (a)));
            Remaining.push_front (drawing_area (XR (b), YT (a), XR (a) - XR (b), a.height()));
            break;
        }
        case XMR|YOU:
        {
            //     +---+
            // +---+-+ |
            // | A | |B|
            // +---+-+ |
            //     +---+
            Remaining.push_front (drawing_area (XL (a), YT (a), XL (b) - XL (a), a.height()));        
            break;
        }   
        case XLR|YOM:
        {
            // +-------+
            // |   B   |
            // | +---+ |
            // +-+---+-+
            //   | A |
            //   +---+
            Remaining.push_front (drawing_area (XL (a), YB (b), a.length(), YB (a) - YB (b)));
            break;
        }
        case XLM|YOU:
        {
            // +---+
            // | +-+---+
            // |B| | A |
            // | +-+---+
            // +---+
            Remaining.push_front (drawing_area (XR (b), YT (a), XR (a) - XR (b), a.height()));
            break;
        }
        case XLR|YMU:
        {
            //   +---+
            //   | A |
            // +-+---+-+
            // | +---+ |
            // |   B   |
            // +-------+
            Remaining.push_front (drawing_area (XL (a), YT (a), a.length(), YT (b) - YT (a)));
            break;
        }
        case XLR|YMM:
        {
            //   +---+
            // +-+---+---+
            // | | A | B |
            // +-+---+---+
            //   +---+ 
            Remaining.push_front (drawing_area (XL (a), YT (a), a.length(), YT (b) - YT (a)));
            Remaining.push_front (drawing_area (XL (a), YB (b), a.length(), YB (a) - YB (b)));
            break;
        }   
        case XMM|YOU:
        {
            //   +---+
            // +-+---+---+
            // | | B | A |
            // +-+---+---+
            //   +---+
            Remaining.push_front (drawing_area (XL (a), YT (a), XL (b) - XL (a), a.height()));
            Remaining.push_front (drawing_area (XR (b), YT (a), XR (a) - XR (b), a.height()));
            break;
        }   
        case XLR|YOU:
        {
            // +---------+
            // | +---+   |
            // | | A | B |
            // | +---+   |
            // +---------+
            break;
        }   
        case XMM|YMM:
        {
            // +---------+
            // |.+---+...|
            // | | B | A |
            // |.+---+...|
            // +---------+
            Remaining.push_front (drawing_area (XL (a), YT (a), a.length(), YT (b) - YT (a)));
            Remaining.push_front (drawing_area (XL (a), YT (b), XL (b) - XL (a), b.height()));
            Remaining.push_front (drawing_area (XR (b), YT (b), XR (a) - XR (b), b.height()));
            Remaining.push_front (drawing_area (XL (a), YB (b), a.length(), YB (a) - YB (b)));
            break;
        }
        default:
        {
            // no overlap
            Remaining.push_front (a);
            break;
        }
    }
}
