/*
   $Id: factory.cc,v 1.1 2004/04/09 11:59:19 ksterker Exp $

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
 * @file   event/factory.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Implements the %event %factory class.
 * 
 */

#include <algorithm>
#include "base/diskio.h"
#include "event/factory.h"
#include "event/manager.h"

using std::vector;
using event::factory;
using event::listener;
using event::manager;

// constructor
factory::factory ()
{
}

// destructor
factory::~factory ()
{
    clear (); 
}

// Unregisters and deletes all events.
void factory::clear () 
{
    listener *li;
    
    while (!Listeners.empty ())
    {
        // deleting the listener will remove it from our list too
        li = Listeners.front ();
        delete li;
    }    
}

// Adds an event to the list and register it with the event manager.
listener *factory::add (event* ev)
{
    listener *li = new listener (this, ev);
    Listeners.push_back (li);

    // if the factory is paused, also pause new events
    if (Paused) li->pause (Paused);

    // only register event if not paused
    else if (!li->is_paused ()) manager::add (li);
    
    // finally return the listener, so a callback can be attached
    return li;
}

// Remove an event from the list
void factory::remove (listener *li)
{
    vector<listener*>::iterator i;

    // Search for the event we want to remove
    i = find (Listeners.begin (), Listeners.end (), li);

    // found? -> get rid of it :)
    if (i != Listeners.end ()) Listeners.erase (i);
}

// retrieve event by its id
listener *factory::get_listener (const string & id)
{
    vector<listener*>::iterator i;

    for (i = Listeners.begin (); i != Listeners.end (); i++)
         if ((*i)->id () == id) return *i;
    
    return NULL;   
}

// disable all events in the list
void factory::pause ()
{
    Paused++;
    for (vector<listener*>::iterator i = Listeners.begin (); i != Listeners.end (); i++)
        (*i)->pause ();
}

// enable all events in the list
void factory::resume ()
{
    Paused--;
    for (vector<listener*>::iterator i = Listeners.begin (); i != Listeners.end (); i++)
        (*i)->resume ();
}

// Save a factory to file
void factory::put_state (base::ogzstream& file) const
{
    base::diskio out;
    base::flat element;
    std::vector <listener *>::iterator i;

    out.put_uint16 ("fps", Paused);
    
    for (i = Listeners.begin (); i != Listeners.end (); i++)
    {
        if ((*i)->is_destroyed ()) continue;
        
        (*i)->put_state (element);
        out.put_flat ("", element);
        element.clear ();
    }
    
    out.put_record (file);
}

// Loads an factory from file
bool factory::get_state (base::igzstream& file)
{
    void *value;
    int type, size;
    base::diskio in;
    listener *li;
    
    // try to read the factory state record from file
    if (!in.get_record (file)) return false;

    Paused = in.get_uint16 ("fps");

    // get registered listeners
    while ((type = in.next (&value, &size)) != -1) 
    {
        if (type != base::flat::T_FLAT)
        {
            fprintf (stderr, "*** error: factory::get_state: expected type T_FLAT but got %i!\n", type);
            return false;
        }
        base::flat state ((const char*) value, size);
        
        li = new listener (this, NULL);
        if (li->get_state (state))
        {
            Listeners.push_back (li);
            if (!li->is_paused ()) manager::add (li);
        }
        else 
        {
            delete li;
            return false;
        }
    }
    return true;
}
