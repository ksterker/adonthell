/*
   Copyright (C) 2012 Kai Sterker <kai.sterker@gmail.com>
   Part of the Adonthell Project http://adonthell.nongnu.org

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
 * @file 	world/move_event_manager.cc
 *
 * @author 	Kai Sterker
 * @brief 	Handle execution of move events.
 */

#include <algorithm>

#include "move_event_manager.h"
#include "move_event.h"

using world::move_event_manager;
using world::move_event;

// function returning a new move event
NEW_EVENT (world, move_event)

// register move events with event subsystem
move_event_manager::move_event_manager () : manager_base (&new_move_event)
{
	// nothing to do here
}

// dtor
move_event_manager::~move_event_manager ()
{
    Events.clear();
}

// See whether a matching event is registered and execute the
// according script(s) 
void move_event_manager::raise_event (const event *e)
{
    for (std::list<listener*>::iterator i = Events.begin(); i != Events.end(); /* nothing */)
    {
        if ((*i)->is_destroyed())
        {
            events::listener *temp = *i;
            i = Events.erase(i);
            delete temp;

            continue;
        }

        if ((*i)->equals (e))
        {
            (*i)->raise_event (e);
        }

        i++;
    }
}

// Unregister a listener
void move_event_manager::remove (listener *li)
{
    std::list<events::listener*>::iterator i;

    // Search for the event we want to remove
    i = std::find (Events.begin (), Events.end (), li);

    // found? -> get rid of it :)
    if (i != Events.end ()) Events.erase (i);
}

// register a listener with the manager
void move_event_manager::add (listener *li)
{
    Events.push_back (li);
}
