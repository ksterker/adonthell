/*
   $Id: audio_event_manager.cc,v 1.1 2005/10/08 07:38:17 Mithander Exp $

   Copyright (C) 2002/2003/2004/2005 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file    event/audio_event_manager.cc
 * @author  Tyler Nielsen <tyler.nielsen@gmail.com>
 *
 * @brief   Implements the audio_event_manager class.
 */

#include <algorithm>
#include "audio/audio_event.h"
#include "audio/audio_event_manager.h"

using std::vector;
using audio::audio_event_manager;
using events::event_type;

// time_event manager instance that is initialized when the audio package is loaded
static audio_event_manager AudioEventManager;

// function returning a new time event
NEW_EVENT (audio, audio_event)

// register time events with event subsystem
audio_event_manager::audio_event_manager () : manager_base (&new_audio_event)
{
    // nothing to do here
}

// See whether a matching event is registered and execute the
// according script(s)
void audio_event_manager::raise_event (const event * e)
{
    vector<listener*>::iterator idx;

    // As long as matching events are in the list

    for(idx = Listeners.begin(); idx != Listeners.end(); idx++)
    {
        if((*idx)->equals (e))
            (*idx)->raise_event(e);
    }
}

// Unregister a listener
void audio_event_manager::remove (listener *li)
{
    vector<listener*>::iterator i;

    // Search for the event we want to remove
    i = find (Listeners.begin (), Listeners.end (), li);

    // found? -> get rid of it :)
    if (i != Listeners.end ()) Listeners.erase (i);
}

// register a listener with the manager
void audio_event_manager::add (listener *li)
{
    Listeners.push_back (li);
}

