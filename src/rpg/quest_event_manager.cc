/*
   $Id: quest_event_manager.cc,v 1.2 2005/10/09 07:38:40 ksterker Exp $

   Copyright (C) 2005 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file 	rpg/quest_event_manager.h
 *
 * @author 	Kai Sterker
 * @brief 	Handle execution of quest events class.
 */

#include "rpg/quest_event_manager.h"
#include "rpg/quest_event.h"

using rpg::quest_event_manager;
using rpg::quest_event;

// quest_event manager instance that is initialized when the rpg package is loaded 
static quest_event_manager QuestEventManager;

// function returning a new time event
NEW_EVENT (rpg, quest_event)

// register time events with event subsystem 
quest_event_manager::quest_event_manager () : manager_base (&new_quest_event)
{
	// nothing to do here
}

// dtor
quest_event_manager::~quest_event_manager ()
{
    std::map<std::string, std::vector<listener*> >::iterator i;
	for (i = Events.begin(); i != Events.end(); i++)
	{
		std::vector<listener*> *li = &(*i).second;
		li->clear();
		// delete li; // <-- causes double free
	}
}

// See whether a matching event is registered and execute the
// according script(s) 
void quest_event_manager::raise_event (const event *e)
{
    std::map<std::string, std::vector<listener*> >::iterator li;
	static std::string paths[] = { "", "*", ">" };
	quest_event *ev = (quest_event *) e;
	paths[0] = *(ev->begin());
	
	for (int i = 0; i < 3; i++)
	{
		if ((li = Events.find (paths[i])) != Events.end ())
		{
			raise_event (e, &(*li).second);
		}
	}
}

// raise all matching events in given list of listeners
void quest_event_manager::raise_event (const event *e, std::vector<listener*> *listeners)
{
	s_int32 repeat;

	for (std::vector<listener*>::iterator li = listeners->end (); li != listeners->begin(); /* nothing */ )
	{
		li--;
		
		// if events equal ...
		if ((*li)->equals (e))
		{
			// .. raise event and ...
			repeat = (*li)->raise_event (e);
			// ... remove listener if repeat count reaches zero
			if (repeat == 0) listeners->erase (li);
		}
	}
}

// Unregister a listener
void quest_event_manager::remove (listener *li)
{
	quest_event *ev = (quest_event *) li->get_event();
    std::map<std::string, std::vector<listener*> >::iterator e;
	std::string path = *(ev->begin());
	
	// try to find vector where listener would be stored
	if ((e = Events.find (path)) != Events.end ())
    {
		std::vector<listener*>::iterator i;

		// Search for the listener we want to remove
		i = find ((*e).second.begin (), (*e).second.end (), li);

		// found? -> get rid of it :)
		if (i != (*e).second.end ())
		{
			(*e).second.erase (i);
			return;
		}
	}

	// if we end up here, listener wasn't registered or already removed
	fprintf (stderr, "*** quest_event_manager::remove: listener not registered!\n");
}

// register a listener with the manager
void quest_event_manager::add (listener *li)
{
	quest_event *ev = (quest_event *) li->get_event();
    std::map<std::string, std::vector<listener*> >::iterator e;
	std::string path = *(ev->begin());
	
	// add listener to those with the same first path element
	if ((e = Events.find (path)) != Events.end ())
    {
        (*e).second.push_back (li);
    }
    else 
	{
		std::vector<listener*> *listeners = new std::vector<listener*>();
		listeners->push_back (li);
		Events[path] = *listeners;
	}
}
