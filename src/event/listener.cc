/*
   $Id: listener.cc,v 1.7 2005/06/03 17:29:13 ksterker Exp $

   Copyright (C) 2004/2005 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file   event/listener.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Declares the %listener class.
 * 
 */

#include "python/pool.h"
#include "event/manager.h"
#include "event/listener.h"

using events::factory;
using events::listener;

// ctor
listener::listener (factory *f, event *e)
{
    Registered = false;
    Method = NULL;
    Args = NULL;
    Factory = f;
    Event = e;
    Paused = 0;
    Id = "";
}

// destructor
listener::~listener ()
{
    // automatically remove myself from the event manager
    if (Registered) manager::remove (this);
    // ... and from the factory
    if (Factory) Factory->remove (this);
    
    delete Event;
    
    // we no longer use the callback
    delete Method;
    // ... and the arguments neither
    Py_XDECREF (Args);
}

// set python method to be called when the event occurs
bool listener::connect_callback (const string & file, const string & classname, const string & callback, PyObject *args)
{
    u_int16 size;
    
    // cleanup
    delete Method;
    
    // just disconnect the callback
    if (file == "") 
    {
        Method = NULL;
        return false;
    }
    
    // create the callback
    Method = python::pool::connect (EVENTS_DIR + file, classname, callback);
    if (!Method)
    {
        fprintf (stderr, "*** listener::connect_callback: connecting callback failed!\n");
        return false;
    }
    
    // make sure the given arguments are a tuple
    if (!args || !PyTuple_Check (args))
    {
        if (args) fprintf (stderr, "*** warning: listener::connect_callback: argument must be a tuple!\n");
        size = 2;
    }
    else size = PyTuple_GET_SIZE (args) + 2;
    
    // keep old argument tuple, if possible
    if (!Args || PyTuple_GET_SIZE (Args) != size)
    {
        // free old args
        Py_XDECREF (Args);

        // prepare callback arguments
        Args = PyTuple_New (size);
    
        // first argument is the listener itself
        PyTuple_SET_ITEM (Args, 0, python::pass_instance (this));
    }
    
    // second argument will be the event that triggered the callback
    for (u_int16 i = 2; i < size; i++)
    {
        // copy remaining arguments, if any
        PyObject *arg =  PyTuple_GET_ITEM (args, i-2);
        Py_INCREF (arg);
        PyTuple_SET_ITEM (Args, i, arg);
    }
    return true;
}

// execute callback for given event
s_int32 listener::raise_event (const event* evnt) 
{
    if (Method && Event->repeat ())
    {
        // make sure that arguments remain valid while the script executes
        PyObject *args = Args;
        Py_INCREF (args);
        
        // event that triggered the script is 2nd argument of callback
        PyTuple_SET_ITEM (args, 1, python::pass_instance ((event*) evnt));
        
        // adjust repeat count
        Event->do_repeat ();
        
        // execute callback
        Method->execute (args);
        
        // clean up
        Py_DECREF (PyTuple_GET_ITEM (args, 1));
        Py_DECREF (args);
    }
    else
    {
        if (!Method) fprintf (stderr, "*** warning: listener::raise_event: no callback connected\n");
        return 0;
    }
    
    // return whether event needs be repeated or not
    return Event->repeat ();
}

// disable the event temporarily
void listener::pause (const u_int16 & level)
{
    if (Paused == 0) manager::remove (this);
    Paused += level;
}

// resume a disabled event
void listener::resume ()
{
    if (Paused == 0)
    {
        fprintf (stderr, "*** warning: listener::resume: listener was not paused!\n");
        if (!Registered) manager::add (this);
    }
    else 
    {
        Paused--;
        if (Paused == 0) manager::add (this);
    }
}

// save the state of the script associated with the event
void listener::put_state (base::flat & out) const
{
    base::flat listener;
    
    listener.put_string ("lid", Id);
    listener.put_uint16 ("lps", Paused);
    listener.put_bool ("lmt", Method != NULL);

    if (Method != NULL)
    {
        Method->put_state (listener);
        python::put_tuple (Args, listener, 2);
    }
    
    listener.put_bool ("lev", Event != NULL);
    if (Event != NULL) Event->put_state (listener);
    
    out.put_flat ("", listener);
}

// load the state of the script associated with the event 
bool listener::get_state (base::flat & in) 
{
    Id = in.get_string ("lid");
    Paused = in.get_uint16 ("lps");
    
    // load callback
    if (in.get_bool ("lmt") == true)
    {
        Method = new python::method ();
        if (Method->get_state (in) == false)
        {
            fprintf (stderr, "*** listener::get_state: restoring callback failed!\n");
            return false;
        }
        
        Args = python::get_tuple (in, 2);
        PyTuple_SET_ITEM (Args, 0, python::pass_instance (this));
    }

    // load event structure
    if (in.get_bool ("lev") == true)
    {
        std::string type = in.get_string ("etp");
        
        // Instanciate an event of the given type
        Event = event_type::instanciate_event (type);
 
        // try to load it
        if (Event == NULL || Event->get_state (in) == false)
        {
            fprintf (stderr, "*** listener::get_state: could not load event of type '%s'!\n", type.c_str ());
            return false;
        }    
    }

    return in.success ();
}


