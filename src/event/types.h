/*
   $Id: types.h,v 1.3 2005/10/09 07:38:40 ksterker Exp $

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
 * @file   event/types.h
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Stores information about different types of game events
 * 
 */

#ifndef EVENT_TYPES_H
#define EVENT_TYPES_H

#include "event/event.h"	
#include <base/hash_map.h>

namespace events {

class manager_base;

#ifndef SWIG
/**
 * Pointer to a function returning a newly allocated %event
 */
typedef events::event* (*new_event)();
#endif // SWIG

/**
 * This class keeps track of all types of game events. They can be 
 * dynamically registered at runtime and will be given an unique id
 * while being registered.
 *
 * Each type of %event needs a unique name, a event::manager to handle
 * these types of events and a creator function to instanciate this
 * type of event when restoring a saved game.
 */
class event_type
{
public:

    /**
     * Register a certain type of %event with the %event subsystem.
     * @param name unique name for %event type.
     * @param manager %event manager for that type of event.
     * @param creator method to create new instance of this event type.
     */
    static void register_type (const std::string & name, manager_base *manager, new_event creator);

    /**
     * Remove a certain type of %event from the %event subsystem.
     * @param name unique name for %event type to remove.
     */
    static void remove_type (const std::string & name);

    /**
     * Get id of %event type with given name.
     * @param name name of %event whos id to retrieve.
     * @return id of %event_type
     */
    static u_int8 get_id (const std::string & name);
    
    /**
     * Return a new instance of the given %event type.
     * @param name name of %event whos instance to retrieve.
     * @return a new instance of given %event type or \c NULL on error.
     */
    static event *instanciate_event (const std::string & name);
    
    /**
     * Return %event %manager for given %event id
     * @param id numeric %event type
     * @return %event %manager or \c NULL on error.
     */
    static manager_base *get_manager (const u_int8 & id);
    
    /**
     * Return Id of this %event type.
     * @return id of this %event type.
     */
    u_int8 id () const
    {
        return Id;
    }
    
    /**
     * Instanciate a new event of this type.
     * @return a newly instanciated event.
     */
    event *instanciate ()
    {
        return InstanciateEvent ();
    }
    
    /**
     * Return Manager of this %event type.
     * @return Manager of this %event type.
     */
    manager_base *manager ()
    {
        return Manager;
    }
    
protected:
    /**
     * Create a new type of game %event.
     * @param id unique id for %event type.
     * @param manager %event manager for that type of event.
     * @param creator method to create new instance of this event type.
     */
    event_type (const u_int8 & id, manager_base *manager, new_event creator);
    
private:
    /// Id of event
    u_int8 Id;

    /// Manager for this type of event 
    manager_base *Manager;

    /// Function to instanciate an event of this type
    new_event InstanciateEvent;
    
    /// Types of events registered with the event subsystem by name
    static std::hash_map<std::string, event_type*>& NamedTypes ();
    
    /// Types of events by id
    static std::vector<event_type*>& Types ();
};

#ifndef SWIG
/**
 * Macro for functions that return a new instance of an %event.
 * @param nsp namespace the event class lives in
 * @param evt the event class to instanciate
 */
#define NEW_EVENT(nsp, evt)\
    events::event* new_ ## evt () { return (events::event*) new nsp::evt; }
#endif // SWIG

}

#endif // EVENT_TYPES_H
