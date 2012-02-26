/*
   Copyright (C) 2011 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file 	gui/ui_event_manager.cc
 *
 * @author 	Kai Sterker
 * @brief 	Implements the ui_event_manager class.
 */

#include <algorithm>

#include "gui/ui_event_manager.h"
#include "gui/ui_event.h"

using std::list;
using gui::ui_event_manager;
using events::event_type;

// function returning a new ui event
NEW_EVENT (gui, ui_event)

// register ui events with event subsystem
ui_event_manager::ui_event_manager () : events::manager_base (&new_ui_event)
{
	// nothing to do here
}

// trigger the event listeners
void ui_event_manager::update()
{
    // event handlers might fire new events, so use a copy
    std::list<events::listener*> copy (Pending.begin(), Pending.end());

    // clear list of pending events
    Pending.clear();

    // call event handlers
    for (std::list<events::listener*>::iterator li = copy.begin(); li != copy.end(); li++)
    {
        (*li)->raise_event ((*li)->get_event());
    }
}

// See whether a matching event is registered
void ui_event_manager::raise_event (const events::event * e)
{
    // As long as matching events are in the list
    for (std::list<events::listener*>::iterator li = Listeners.begin(); li != Listeners.end(); /* nothing */)
    {
        if ((*li)->is_destroyed())
        {
            events::listener *temp = *li;
            li = Listeners.erase(li);
            delete temp;

            continue;
        }

        const events::event *evt = (*li)->get_event();
        if (evt->equals (e))
        {
            Pending.push_back(*li);
        }

        li++;
    }
}

// Unregister a listener
void ui_event_manager::remove (events::listener *li)
{
    list<events::listener*>::iterator i;

    // Search for the event we want to remove
    i = std::find (Listeners.begin (), Listeners.end (), li);

    // found? -> get rid of it :)
    if (i != Listeners.end ()) Listeners.erase (i);
}

// register a listener with the manager
void ui_event_manager::add (events::listener *li)
{
    Listeners.push_back (li);
}
