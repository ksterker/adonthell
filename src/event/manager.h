/*
   Copyright (C) 2000/2001/2002/2003/2004 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file   event/manager.h
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Declares the %event %manager class
 * 
 */

#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include "event/factory.h"
#include "event/manager_base.h"

namespace events
{
    /**
     * It ensures global access to the individual %event managers.
     */
    class manager
    {
    public:
        /** 
         * Unregister an %event %listener.
         * 
         * @param li pointer to the %listener to unregister.
         */
        static void remove (listener* li)
        {
            manager_base *manager = event_type::get_manager (li->type ());
            if (manager != NULL)
            {
                li->set_registered (false);
                manager->remove (li);
            }
        }
    
        /** 
         * Check if an %event corresponding to ev exists, and execute it. 
         * 
         * @param ev %event to raise.
         */
        static void raise_event (const event* ev)
        {
            manager_base *manager = event_type::get_manager (((event *) ev)->type ());
            if (manager != NULL)
            {
                manager->raise_event (ev);
            }
        }
    
    protected:
        /** 
         * Registers an %event %listener.
         * 
         * @param li pointer to the %listener to register.
         */
        static void add (listener* li)
        {
            manager_base *manager = event_type::get_manager (li->type ());
            if (manager != NULL)
            {
                li->set_registered (true);
                manager->add (li);
            }
        }
    
        /**
         * Only %event %factory is allowed to register events with the
         * %event %manager.
         */
        friend class factory;
        
        /**
         * As is %listener::resume
         */
        friend void listener::resume ();
    };
}
#endif // EVENT_MANAGER_H
