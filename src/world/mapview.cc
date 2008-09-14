/*
 $Id: mapview.cc,v 1.8 2008/09/14 14:25:25 ksterker Exp $
 
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

#include "gfx/screen.h"
#include "python/pool.h"
#include "world/mapview.h"
#include "world/area.h"

using world::mapview;

/// the python script containing mapview schedules
#define SCHEDULE_SCRIPT "schedules.map.mapview"

// ctor
mapview::mapview (const u_int32 & length, const u_int32 & height) : Z(0), FinalZ(0), Speed(0)
{
    set_length (length);
    set_height (height);
    
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

// update position of mapview
void mapview::center_on (const s_int32 & x, const s_int32 & y)
{
    if (!Map) return;
    
    // get size of map in pixels
    const u_int32 ml = ((chunk *)Map)->length();
    const u_int32 mh = ((chunk *)Map)->height();
    
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
        if (Sx < 0) Sx = 0;
        else if (Sx + length() > ml) Sx = ml - length();
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
        Sy = y - height()/2;
        
        // don't go past edge of map
        if (Sy < 0) Sy = 0;
        else if (Sy + height() > mh) Sy = mh - height();        
    }
}

// render map
void mapview::draw (const s_int16 & x, const s_int16 & y, const gfx::drawing_area * da_opt, gfx::surface * target) const
{
    // is there something to draw at all?
    if (!Map || !Map->length() || !Map->height()) return;
    
    // this is the area we need to draw
    gfx::drawing_area da (x + Ox, y + Oy, length() - Ox, height() - Oy);
    if (da_opt)
    {
        da.assign_drawing_area (da_opt);
        da = da.setup_rects ();
    }
    
    std::list <render_info> drawqueue;

    // get objects we need to draw
    const std::list<world::chunk_info> & objectlist = Map->objects_in_view (Sx, Sy, Z, length(), height());
    
    // populate drawqueue
    std::list<world::chunk_info>::const_iterator i;
    for (i = objectlist.begin(); i != objectlist.end(); i++)
    {
        for (placeable::iterator obj = i->Object->begin(); obj != i->Object->end(); obj++)
        {
            drawqueue.push_back (render_info ((*obj)->current_shape(), (*obj)->get_sprite(), i->Min));
        }
    }
    
    // draw everything on screen
    render (drawqueue, da, target);
}

// save mapview state
bool mapview::put_state (base::flat & file) const
{
    base::flat record;
    
    // save current position
    record.put_uint16 ("vox", Ox);
    record.put_uint16 ("voy", Oy);
    record.put_uint16 ("vsx", Sx);
    record.put_uint16 ("vsy", Sy);
    
    // save height related variables
    record.put_sint32 ("vz", Z);
    record.put_sint32 ("vfz", FinalZ);
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
    
    // get coordinates
    Ox = record.get_uint16("vox"); 
    Oy = record.get_uint16("voy");
    Sx = record.get_uint16("vsx"); 
    Sy = record.get_uint16("vsy"); 

    // get height related variables
    Z = record.get_sint32("vz");
    FinalZ = record.get_sint32("vfz");
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

// render objects in queue
void mapview::render (std::list <world::render_info> & drawqueue, const gfx::drawing_area & da, gfx::surface * target) const
{
    // sort according to drawing order
    drawqueue.sort ();
    
    for (std::list <world::render_info>::iterator it = drawqueue.begin (); it != drawqueue.end (); it++)
    {
        s_int16 pos_x = da.x() + it->Pos.x () - Sx;
        s_int16 pos_y = da.y() + Z + it->Pos.y () - it->Pos.z() - Sy - it->Shape->height() - it->Shape->z(); 
        
        it->Sprite->draw (pos_x, pos_y, &da, target);
    }
    
    drawqueue.clear ();    
}
