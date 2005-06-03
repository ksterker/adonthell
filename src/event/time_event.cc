/*
   $Id: time_event.cc,v 1.7 2005/06/03 17:29:13 ksterker Exp $

   Copyright (C) 2002/2003/2004 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file event/time_event.cc
 *
 * @author Kai Sterker
 * @brief Implements the time_event class.
 */

#include "event/time_event.h"
#include "event/date.h"

using events::date;
using events::time_event;

// create a new time event
time_event::time_event (const string & time, bool absolute) : event ()
{
    Repeat = 1;
    Absolute = absolute;
    Time = date::parse_time (time);
    if (!absolute) Time += date::time ();
}

// specify the interval between two occurances of the event
void time_event::set_repeat (const string & interval, s_int32 count)
{
    Interval = date::parse_time (interval);
    Repeat = count;
}

// Save time event to file
void time_event::put_state (base::flat & out) const
{
    // save basic event data first
    event::put_state (out);
    
    // save time event data
    out.put_uint32 ("ttm", Time);
    out.put_uint32 ("tin", Interval);
    out.put_bool ("tab", Absolute);
}

// load time event from file
bool time_event::get_state (base::flat & in)
{
    // get basic event data
    if (event::get_state (in))
    {   
        // get time event data
        Time = in.get_uint32 ("ttm");
        Interval = in.get_uint32 ("tin");
        Absolute = in.get_bool ("tab");
    }
    return in.success ();
}

void time_event::do_repeat ()
{
    // don't repeat multiple times after being resumed
    while (Time <= date::time ()) Time += Interval;
    event::do_repeat ();
}

