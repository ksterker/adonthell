/*
 $Id: mapview.cc,v 1.15 2009/04/03 21:56:53 ksterker Exp $
 
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
 * @file   world/mapview.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Implements a part of the map visible on screen.
 * 
 */

#include <limits.h>

#include "gfx/screen.h"
#include "python/pool.h"
#include "world/mapview.h"
#include "world/area_manager.h"

using world::mapview;

/// the python script containing mapview schedules
#define SCHEDULE_SCRIPT "schedules.map.mapview"

/// the fallback if no renderer is set explicitly
world::default_renderer mapview::DefaultRenderer;

// standard ctor
mapview::mapview () : CurZ(0), Speed(0)
{
    set_length (gfx::screen::length());
    set_height (gfx::screen::height());
    
    set_renderer (NULL);
    
    RenderZone = NULL;
    Schedule = NULL;
    Args = NULL;
}

// ctor
mapview::mapview (const u_int32 & length, const u_int32 & height, const renderer_base * renderer) 
 : CurZ(0), Speed(0)
{
    set_length (length);
    set_height (height);
    
    set_renderer (renderer);
    
    RenderZone = NULL;
    Schedule = NULL;
    Args = NULL;
}

// dtor
mapview::~mapview ()
{
    clear ();
}

// reset to initial state
void mapview::clear ()
{
    delete RenderZone;
    delete Schedule;
    Py_XDECREF (Args);   
    
    RenderZone = NULL;
    Schedule = NULL;
    Args = NULL;    
}

// set script called to position view on map
bool mapview::set_schedule (const std::string & method, PyObject *extraArgs)
{
    // get mapview schedule method
    Schedule = python::pool::connect (SCHEDULE_SCRIPT, "mapview", method);
    if (!Schedule) return false;
    
    // make sure the given arguments are a tuple
    if (extraArgs && !PyTuple_Check (extraArgs))
    {
        fprintf (stderr, "*** warning: mapview::set_schedule: extra args must be a tuple!\n");
        return false;
    }
    
    // calculate size of argument tuple required
    u_int16 size = extraArgs ? PyTuple_GET_SIZE (extraArgs) + 1 : 1;
    
    // free old tuple content
    if (Args)
    {
        u_int16 s = PyTuple_GET_SIZE (Args);
        for (u_int16 i = 0; i < s; i++)
        {
            PyObject *arg =  PyTuple_GET_ITEM (extraArgs, i);
            Py_DECREF (arg);
        }
    }
    
    // keep old argument tuple, if possible
    if (!Args || PyTuple_GET_SIZE (Args) != size)
    {
        // free old args
        Py_XDECREF (Args);
        
        // prepare callback arguments
        Args = PyTuple_New (size);
        
        // first argument is the mapview itself
        PyTuple_SET_ITEM (Args, 0, python::pass_instance (this));
    }
    
    // prepare arguments
    for (u_int16 i = 1; i < size; i++)
    {
        // copy remaining arguments, if any
        PyObject *arg =  PyTuple_GET_ITEM (extraArgs, i - 1);
        Py_INCREF (arg);
        PyTuple_SET_ITEM (Args, i, arg);
    }
    
    return true;
}

// change renderer
void mapview::set_renderer (const world::renderer_base * renderer)
{
    if (renderer == NULL)
    {
        Renderer = &DefaultRenderer;
    }
    else
    {
        Renderer = renderer;
    }
}

// update position of mapview
void mapview::center_on (const s_int32 & x, const s_int32 & y)
{
    area *map = world::area_manager::get_map();
    if (!map) return;
    
    // update position
    Pos.set_x (x);
    Pos.set_y (y);
    
    // get size of map in pixels
    const u_int32 ml = map->length();
    const u_int32 mh = map->height();

    // get the start point of the map
    const s_int32 start_x = map->min().x();
    const s_int32 start_y = map->min().y();
    
    // calculate start and offset of view (x-axis)
    if (length() >= ml) 
    {
        Ox = (length() - ml)/2;
        Sx = 0;
    }
    else 
    {
        Ox = 0;
        Sx = x - length()/2;

        // don't go past edge of map
        if (Sx < start_x) Sx = start_x;
        else if (Sx + (s_int32)length() > (s_int32) start_x + (s_int32)ml) Sx = start_x + ml - length();
    }
    
    // calculate start and offset of view (y-axis)
    if (height() >= mh)
    {
        Oy = (height() - mh)/2;
        Sy = 0;
    }
    else 
    {
        Oy = 0;
        Sy = y - CurZ - height()/2;
        
        // don't go past edge of map
        if (Sy < start_y - CurZ) Sy = start_y - CurZ;
        else if (Sy + (s_int32)height() > (s_int32) start_y + (s_int32)mh) Sy = start_y + mh - height();
    }
}

// render map
void mapview::draw (const s_int16 & x, const s_int16 & y, const gfx::drawing_area * da_opt, gfx::surface * target) const
{
    area *map = world::area_manager::get_map();
    
    // is there something to draw at all?
    if (!map || !map->length() || !map->height()) return;
    
    // this is the area we need to draw
    gfx::drawing_area da (x + Ox, y + Oy, length() - Ox, height() - Oy);
    if (da_opt)
    {
        da.assign_drawing_area (da_opt);
        da = da.setup_rects ();
    }
 
    // get objects we need to draw
    std::list<world::chunk_info*> objectlist;
    map->objects_in_view (Sx, Sx + length(), Sy, Sy + height(), objectlist);
    
    // are there any zones limiting what we have to render?
    std::vector<world::zone*> zones;
    if (RenderZone == NULL)
    {
        zones = map->find_zones (Pos, world::zone::TYPE_RENDER);
    }
    else
    {
        zones.push_back (RenderZone);
    }
    
    // filter list of objects to render by zones
    switch (zones.size())
    {
        case 0: break; // nothing to do
        case 1:
        {
            // check against a single zone
            world::zone *zn = zones.front();
            for (std::list<world::chunk_info*>::iterator i = objectlist.begin(); i != objectlist.end(); /* nothing */)
            {
                // above zone? --> candidate for removal
                if ((*i)->Min.z() > zn->max().z())
                {
                    // object inside zone boundaries? --> discard
                    // if (!((*i)->Max.x() < zn->min().x() || (*i)->Min.x() > zn->max().x() ||
                    //       (*i)->Max.y() < zn->min().y() || (*i)->Min.y() > zn->max().y()))
                    // {
                        i = objectlist.erase (i);
                        continue;
                    // }
                }
                i++;
            }
            break;
        }
        default:
        {
            // check against multiple zones
            for (std::list<world::chunk_info*>::iterator i = objectlist.begin(); i != objectlist.end(); /* nothing */)
            {
                bool discard = true;
                for (std::vector<world::zone*>::iterator zn = zones.begin(); zn != zones.end(); zn++)
                {
                    // above zone? --> candidate for removal
                    if ((*i)->Min.z() > (*zn)->max().z())
                    {
                        // object inside zone boundaries? --> discard
                        // if (!((*i)->Max.x() < (*zn)->min().x() || (*i)->Min.x() > (*zn)->max().x() ||
                        //       (*i)->Max.y() < (*zn)->min().y() || (*i)->Min.y() > (*zn)->max().y()))
                        // {
                            continue;
                        // }
                    }
                    
                    discard = false;
                    break;
                }
                
                discard ? i = objectlist.erase (i) : i++;
            }
            
            break;
        }
    }
    
    // draw everything on screen
    Renderer->render (da.x() - Sx, da.y() - Sy, objectlist, da, target);
}

// update render limit
void mapview::limit_z (const s_int32 & limit)
{
    if (RenderZone == NULL)
    {
        world::vector3<s_int32> min (INT_MIN, INT_MIN, INT_MIN);
        world::vector3<s_int32> max (INT_MAX, INT_MAX, limit);
        RenderZone = new world::zone (world::zone::TYPE_RENDER, "", min, max);
    }
    else
    {
        RenderZone->max().set_z(limit);
    }
}

// save mapview state
bool mapview::put_state (base::flat & file) const
{
    base::flat record;
    
    // save size
    record.put_uint16 ("vln", length());
    record.put_uint16 ("vht", height());
    
    // save current position
    record.put_uint16 ("vox", Ox);
    record.put_uint16 ("voy", Oy);
    record.put_uint16 ("vsx", Sx);
    record.put_uint16 ("vsy", Sy);
    
    // save position related variables
    Pos.put_state (record, "pos");
    record.put_sint32 ("vfz", CurZ);
    record.put_sint16 ("vsp", Speed);
    
    // save schedule name
    if (Schedule) record.put_string ("vsn", Schedule->name ());
    
    // save schedule args, if any
    record.put_bool("vsa", Args != NULL);
    python::put_tuple (Args, record, 1);
    
    // finally save mapview record
    file.put_flat ("mapview", record);
    
    return true;
}

// restore from file
bool mapview::get_state (base::flat & file)
{
    // get mapview record
    base::flat record (file.get_flat ("mapview"));
    if (!file.success()) return false;
    
    PyObject *extraArgs = NULL;

    // get size
    set_length (record.get_uint16 ("vln"));
    set_height (record.get_uint16 ("vht"));
    
    // get coordinates
    Ox = record.get_uint16("vox"); 
    Oy = record.get_uint16("voy");
    Sx = record.get_uint16("vsx"); 
    Sy = record.get_uint16("vsy"); 

    // get position related variables
    Pos.set_str (record.get_string ("pos"));
    CurZ = record.get_sint32("vfz");
    Speed = record.get_sint16("vsp");
    
    // get schedule
    std::string schedule_name = record.get_string ("vsn", true);
    
    // get (optional) schedule arguments
    if (record.get_bool ("vsa"))
    {
        extraArgs = python::get_tuple (record);
    }

    // set schedule
    set_schedule(schedule_name, extraArgs);
    
    // loading successful?
    return record.success();
}
