/*
   $Id: quest_event_manager.cc,v 1.1 2005/08/14 16:52:55 ksterker Exp $

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
// FIXME: breaks loading library 
// static quest_event_manager QuestEventManager;

// function returning a new time event
NEW_EVENT (rpg, quest_event)

// register time events with event subsystem 
quest_event_manager::quest_event_manager () : manager_base (&new_quest_event)
{
	// nothing to do here
}

// See whether a matching event is registered and execute the
// according script(s) 
void quest_event_manager::raise_event (const event * e)
{
}

// Unregister a listener
void quest_event_manager::remove (listener *li)
{
}

// register a listener with the manager
void quest_event_manager::add (listener *li)
{
}
