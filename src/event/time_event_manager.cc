/*
   $Id: time_event_manager.cc,v 1.10 2007/07/22 05:23:11 ksterker Exp $

   Copyright (C) 2002/2003/2004/2005 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file 	event/time_event_manager.cc
 *
 * @author 	Kai Sterker
 * @brief 	Implements the time_event_manager class.
 */

#include "event/time_event_manager.h"
#include "event/time_event.h"
#include "event/date.h"
#include <algorithm>

using std::vector;
using events::time_event_manager;
using events::event_type;

// time_event manager instance that is initialized when the event package is loaded 
static time_event_manager TimeEventManager;

// function returning a new time event
NEW_EVENT (events, time_event)

// register time events with event subsystem 
time_event_manager::time_event_manager () : manager_base (&new_time_event)
{
	// nothing to do here
}

// See whether a matching event is registered and execute the
// according script(s) 
void time_event_manager::raise_event (const event * e)
{
    s_int32 repeat;
    listener *li;
    
    // As long as matching events are in the list
    while (!Listeners.empty () && (li = Listeners.back ())->equals (e))
    {
        // no matter whether the listener will be destroyed or not,
        // it needs to be reregistered, so remove it in any case
        Listeners.pop_back ();

        // execute event callback
        repeat = li->raise_event (e);

        // only re-register listener if time event is repeating
        if (repeat) add (li);
        else delete li;
    }
    
    return;
}

// Unregister a listener
void time_event_manager::remove (listener *li)
{
    vector<listener*>::iterator i;

    // Search for the event we want to remove
    i = find (Listeners.begin (), Listeners.end (), li);

    // found? -> get rid of it :)
    if (i != Listeners.end ()) Listeners.erase (i);
}

// register a listener with the manager
void time_event_manager::add (listener *li)
{
    vector<listener*>::iterator i = Listeners.begin ();

    // search for the proper place to insert new listener
    while (i != Listeners.end ())
    {
        // skip events that are raised earlier than e
        if (((time_event *) li->get_event ())->time () > ((time_event *) (*i)->get_event ())->time ()) break;
        i++;
    }

    Listeners.insert (i, li);
}
