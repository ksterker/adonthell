/*
   $Id: quest_event_manager.h,v 1.3 2006/09/28 19:13:27 gnurou Exp $

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

#ifndef QUEST_EVENT_MANAGER_H
#define QUEST_EVENT_MANAGER_H

#include <adonthell/event/manager_base.h>

#include <list>
#include <map>

using events::manager_base;
using events::listener;
using events::event;

namespace rpg
{
	/**
	 * Manager keeping track of quest_events.
	 */
	class quest_event_manager : public manager_base
	{
	public:
		/**
		 * Create new quest event manager and register it with event
		 * handling system.
		 */
		quest_event_manager ();
		
		/**
		 * Cleanup.
		 */
		~quest_event_manager ();

        /**
         * Registers a %listener.
         * 
         * @param li pointer to the %event to register.
         */
        void add (listener* li);
    
        /** 
         * Unregister a %listener.
         * 
         * @param li pointer to the %event to unregister.
         */
        void remove (listener* li);
    
        /** 
         * Execute scripts of listeners waiting for given %events. 
         * 
         * @param ev %event to raise.
         */
        void raise_event (const event* ev);

	protected:
		/**
		 * Execute all scripts of given listeners matching the given %event.
		 * @param e the triggering event.
		 * @param listeners listeners to test whether they fit to the %event.
		 */
		void raise_event (const event * e, std::list<listener*> *listeners);
	
		/// registered quest events
		std::map<std::string, std::list<listener*> > Events;
	};
	
}

#endif // QUEST_EVENT_MANAGER_H
