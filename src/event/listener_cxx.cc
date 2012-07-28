/*
 Copyright (C) 2006/2011 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file   event/listener_cxx.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Implements a %listener with C++ callback attached.
 * 
 */

#include <adonthell/base/logging.h>
#include "listener_cxx.h"

using events::listener;
using events::listener_cxx;

// ctor
listener_cxx::listener_cxx (factory *f, event *e) : listener (f, e)
{
    Callback = NULL;
}

// destructor
listener_cxx::~listener_cxx ()
{
    delete Callback;
    Callback = NULL;
}

// set python method to be called when the event occurs
bool listener_cxx::connect_callback (const string & file, const string & classname, const string & callback, PyObject *args)
{
    LOG(ERROR) << "listener_cxx::connect_callback (python): unsupported operation!";
    return false;
}

// set a C/C++ callback as event's action
void listener_cxx::connect_callback (base::functor_1<const event*> * callback)
{
    Callback = callback;
}

// execute callback for given event
s_int32 listener_cxx::raise_event (const event* evnt) 
{
    if (Callback && Event->repeat ())
    {
        // adjust repeat count
        Event->do_repeat ();
        
        // execute callback
        (*Callback) (evnt);
    }
    else
    {
        if (!Callback)
        {
            LOG(WARNING) << "listener::raise_event: '" << Id << "' no callback connected";
            destroy();
        }
    }

    // return whether event needs be repeated or not
    return Event->repeat ();
}

// save the state of the script associated with the event
void listener_cxx::put_state (base::flat & out) const
{
    base::flat record;
    
    // save listener type
    record.put_uint8 ("ltp", LISTENER_CXX);
    
    // save base data
    listener::put_state (record);
        
    out.put_flat ("", record);
}

// load the state of the script associated with the event 
bool listener_cxx::get_state (base::flat & in) 
{
    listener::get_state (in);

    // reconnect callback

    return in.success ();
}

