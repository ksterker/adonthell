/*
   $Id: manager.cc,v 1.2 2004/10/25 06:50:08 ksterker Exp $

   Copyright (C) 2000/2001/2002/2003 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file   event/manager.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Implements the %event %handler class.
 * 
 */
 
#include "event/manager.h"
// #include "event/map_event.h"
// #include "event/map_event_manager.h"
#include "event/time_event.h"
#include "event/time_event_manager.h"

using event::manager;
using event::time_event_manager;

// Array with registered event handlers
event::manager_base* manager::Manager[MAX_EVENTS];

// functions that return newly instanciated events
// of a certain type
NEW_EVENT (time_event)
// NEW_EVENT (enter_event); 
// NEW_EVENT (leave_event); 
// NEW_EVENT (action_event); 

// Initialize the game event system
void manager::init ()
{
    // register event managers
    Manager[ENTER_EVENT] = NULL; // was: new map_event_handler;
    Manager[LEAVE_EVENT] = NULL; // was: new map_event_handler;
    Manager[ACTION_EVENT] = NULL; // was: new map_event_handler;
    Manager[TIME_EVENT] = new time_event_manager;

    // register events (required for loading them from file)
    REGISTER_EVENT (TIME_EVENT, time_event)
    // REGISTER_EVENT (ENTER_EVENT, enter_event)
    // REGISTER_EVENT (LEAVE_EVENT, leave_event) 
    // REGISTER_EVENT (ACTION_EVENT, action_event) 
}

// Clear the registered handlers
void manager::cleanup ()
{
    for (int i = 0; i < MAX_EVENTS; i++)
        delete Manager[i];
}
