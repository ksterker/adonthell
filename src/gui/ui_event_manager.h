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
 * @file 	gui/ui_event_manager.h
 *
 * @author 	Kai Sterker
 * @brief 	Declares the ui_event_manager class.
 */

#ifndef GUI_UI_EVENT_MANAGER_H
#define GUI_UI_EVENT_MANAGER_H

#include "event/manager_base.h"
#include <list>

namespace gui
{
    /**
     * This class keeps track of ui events, i.e. events that are raised
     * when certain actions are performed in the user interface.
     */
    class ui_event_manager : public events::manager_base
    {
    public:
		/**
		 * Create a new ui_event_manager and register with the event handling
		 * system.
		 */
		ui_event_manager ();
	
        /**
         * Register a ui %listener with the %event manager. It is added
         * at the end of registered listeners. The %listener needs to be
         * removed before it can be safely deleted.
         *
         * @param li Pointer to the %listener to be registered.
         */
        void add (events::listener *li);
        
        /**
         * Removes the given %listener from the %event manager. Once it is
         * no longer needed, it can be freed.
         * 
         * @param li Pointer to the %listener to be removed.
         */
        void remove (events::listener *li);
        
        /**
         * Check if a handler is registered for the given source
         * and action.
         *
         * @param evnt An %event structure with the source widget
         *      and action.
         */
        void raise_event (const events::event *evnt);

        /**
         * Actually trigger all the pending events.
         */
        void update ();

    private:
        /// storage for registered listeners.
        std::list<events::listener*> Listeners;
        /// storage for pending events.
        std::list<events::listener*> Pending;
    };
}

#endif // GUI_UI_EVENT_MANAGER_H
