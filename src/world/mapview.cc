/*
 $Id: mapview.cc,v 1.1 2008/05/04 13:49:21 ksterker Exp $
 
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

#include "world/mapview.h"
#include "gfx/screen.h"
#include "python/pool.h"

using world::mapview;

/// the python script containing mapview schedules
#define SCHEDULE_SCRIPT "schedules.map.mapview"

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
void mapview::center_on (const u_int16 & x, const u_int16 & y, const u_int16 & ox, const u_int16 & oy)
{
    if (!Map) return;
    
    // calculate size of map in pixels
    const u_int16 ml = Map->length() * SQUARE_SIZE;
    const u_int16 mh = Map->height() * SQUARE_SIZE;
    
    // calculate start and offset of view (x-axis)
    if (length() >= ml) 
    {
        Ox = (length() - ml)/2;
        Sx = 0;
    }
    else 
    {
        Ox = 0;
        Sx = x * SQUARE_SIZE + ox - length()/2;

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
        Sy = y * SQUARE_SIZE + oy - length()/2;
        
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
    gfx::drawing_area da (Ox + x, Oy + y, length() - Ox, height() - Oy);
    if (da_opt) 
    {
        da.assign_drawing_area (da_opt);
        da = da.setup_rects ();
    }
    
    // drawing
    static std::list <world::square_info> drawqueue; 
    world::square *sq;
    
    // the coordinates from which the drawing starts
    u_int16 start_x = Sx / SQUARE_SIZE - (Sx % SQUARE_SIZE != 0);
    u_int16 start_y = Sy / SQUARE_SIZE - (Sy % SQUARE_SIZE != 0);
    
    // the coordinates where the drawing ends
    u_int16 end_x = (Sx + length() - 2 * Ox) / SQUARE_SIZE;
    u_int16 end_y = (Sy + length() - 2 * Oy) / SQUARE_SIZE;
    
    // first row
    for (u_int16 i = start_x; i < end_x; i++)
    {
        sq = Map->get (i, start_y);
        
        for (world::square::iterator it = sq->begin(); it != sq->end(); it++)
        {
            // overflowing first row
            if (it->base_y() < start_y)
            {
                if (it->base_x () == i)
                {
                    drawqueue.push_back (*it);
                    continue;
                }
                
                // top left corner
                if (i == start_x && it->base_x () < start_x)
                {
                    drawqueue.push_back (*it);
                    continue;                
                }
                
                // top right corner
                if (i == end_x && it->base_x () > end_x)
                {
                    drawqueue.push_back (*it);
                    continue;
                }
            }
        }
    }
    
    render (drawqueue, da, target);
    
    // for every row
    for (u_int16 j = start_y; j < end_y; j++)
    {
        // for every cell in the current row
        for (u_int16 i = start_x; i <= end_x; i++) 
        {
            sq = Map->get (i, j);

            // collect all objects whose base tile is in the current row
            for (world::square::iterator it = sq->begin(); it != sq->end(); it++)
            {
                // base tile in view
                if (it->is_base()) 
                {
                    drawqueue.push_back (*it);
                    continue;
                }
                
                // base tile left of view
                if (i == start_x && it->base_x() < start_x)
                {
                    drawqueue.push_back (*it);
                    continue;
                }
                
                // base tile right of view
                if (i == end_x && it->base_x() > end_x)
                {
                    drawqueue.push_back (*it);
                    continue;
                }
            }
        }
        
        render (drawqueue, da, target);
    }
    
    // last row
    for (u_int16 i = start_x; i < end_x; i++)
    {
        sq = Map->get (i, end_y);
        
        for (world::square::iterator it = sq->begin(); it != sq->end(); it++)
        {
            // overflowing last row
            if (it->base_y() > end_y)
            {
                if (it->base_x () == i)
                {
                    drawqueue.push_back (*it);
                    continue;
                }
                
                // bottom left corner
                if (i == start_x && it->base_x () < start_x)
                {
                    drawqueue.push_back (*it);
                    continue;                
                }
                
                // bottom right corner
                if (i == end_x && it->base_x () > end_x)
                {
                    drawqueue.push_back (*it);
                    continue;
                }
            }
        }
    }
    
    render (drawqueue, da, target);
}

// save mapview state
bool mapview::put_state (base::flat & file) const
{
    base::flat record;
    
    // save current position
    record.put_uint16("vox", Ox);
    record.put_uint16("voy", Oy);
    record.put_uint16("vsx", Sx);
    record.put_uint16("vsy", Sy);
    
    // save height related variables
    record.put_sint32("vz", Z);
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
void mapview::render (std::list <world::square_info> & drawqueue, const gfx::drawing_area & da, gfx::surface * target) const
{
    // sort according to drawing order
    drawqueue.sort ();
    
    // draw everything which has its base tile in the current row
    for (std::list <world::square_info>::iterator it = drawqueue.begin (); it != drawqueue.end (); it++)
    {
        s_int16 pos_x = Ox + (*it).x () * world::SQUARE_SIZE + (*it).ox ();
        s_int16 pos_y = Oy + (*it).y () * world::SQUARE_SIZE + (*it).oy () - (*it).z(); 
        
        switch ((*it).obj->type ()) 
        {
            case world::CHARACTER:
            {
                ((world::character_with_gfx *) (*it).obj)->draw (pos_x, pos_y, &da, target);               
                break; 
            }   
            case world::OBJECT:
            {
                ((world::object_with_gfx *) (*it).obj)->draw (pos_x, pos_y, &da, target);
                break;
            }   
            default:
            {
                break; 
            }
        }
    }
    
    drawqueue.clear ();    
}
