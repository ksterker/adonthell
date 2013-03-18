/*
   $Id: types.cc,v 1.5 2006/09/28 19:13:26 gnurou Exp $

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
 * @file   event/types.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Stores information about different types of game events
 * 
 */

#include "manager_base.h"
#include "types.h"

using events::event;
using events::event_type;
using events::manager_base;

// register a new type of event
void event_type::register_type (const std::string & name, manager_base *manager, new_event creator)
{
    std::hash_map<std::string, event_type*>::iterator i = NamedTypes().find (name);   
    if (i == NamedTypes().end())
    {
		NamedTypes()[name] = new event_type ((u_int8) Types().size(), manager, creator);
        Types().push_back (NamedTypes()[name]);
    }
    
    else if (i->second == NULL)
    {
        i->second = new event_type ((u_int8) Types().size(), manager, creator);
        Types().push_back (i->second);
    }
}

// remove event type from list of registered event types
void event_type::remove_type (const std::string & name)
{
    std::hash_map<std::string, event_type*>::iterator i = NamedTypes().find (name);   
    if (i != NamedTypes().end())
    {
        LOG(INFO) << "removing event type '" << name << "' (id = " << (int) i->second->id() << ")";

        Types()[i->second->id()] = NULL;
        delete i->second;
        NamedTypes().erase (i);
    }
}

// get id of given event type
u_int8 event_type::get_id (const std::string & name)
{
    std::hash_map<std::string, event_type*>::iterator i = NamedTypes().find (name);   
    if (i != NamedTypes().end() && i->second != NULL) return i->second->id ();
    
    LOG(ERROR) << "event type '" << name << "' not registered!";
    return 255;
}

// instantiate new event of given type
event *event_type::instantiate_event (const std::string & name)
{
    std::hash_map<std::string, event_type*>::iterator i = NamedTypes().find (name);   
    if (i != NamedTypes().end() && i->second != NULL) return i->second->instantiate ();

    LOG(ERROR) << "event type '" << name << "' not registered!";
    return NULL;
}

// get manager for given event id
manager_base *event_type::get_manager (const u_int8 & id)
{
    if (id < Types().size() && Types()[id] != NULL) return Types()[id]->manager ();

    LOG(WARNING) << "event id '" << (int) id << "' not registered!";
    return NULL;
}

// Types of events registered with the event subsystem by name
std::hash_map<std::string, event_type*>& event_type::NamedTypes ()
{
    static std::hash_map<std::string, event_type*> *NamedTypes = new std::hash_map<std::string, event_type*>();
    return *NamedTypes;
}

// Types of events by id
std::vector<event_type*>& event_type::Types ()
{
    static std::vector<event_type*> *Types = new std::vector<event_type*>();
    return *Types;
}

event_type::event_type (const u_int8 & id, manager_base *manager, new_event creator)
{
    Id = id;
    Manager = manager;
    InstanciateEvent = creator;
}
