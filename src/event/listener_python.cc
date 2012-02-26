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
 * @file   event/listener_python.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Implements a %listener with python callback attached.
 * 
 */

#include "base/logging.h"
#include "listener_python.h"
#include "python/pool.h"

using events::listener;
using events::listener_python;

// ctor
listener_python::listener_python (factory *f, event *e) : listener (f, e)
{
    Method = NULL;
    Args = NULL;
}

// destructor
listener_python::~listener_python ()
{
    // we no longer use the callback
    delete Method;
    // ... and the arguments neither
    Py_XDECREF (Args);
}

// set python method to be called when the event occurs
bool listener_python::connect_callback (const string & file, const string & classname, const string & callback, PyObject *args)
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
        LOG(ERROR) << "listener::connect_callback: connecting callback failed!";
        return false;
    }
    
    // make sure the given arguments are a tuple
    if (!args || !PyTuple_Check (args))
    {
        if (args) LOG(WARNING) << "listener::connect_callback: argument must be a tuple!";
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

// set a C/C++ callback as event's action
void listener_python::connect_callback (base::functor_1<const event*> * callback)
{
    LOG(ERROR) << "listener_python::connect_callback (cxx): unsupported operation!";
}

// execute callback for given event
s_int32 listener_python::raise_event (const event* evnt) 
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
        if (!Method)
        {
            LOG(WARNING) << "listener::raise_event: '" << Id << "' no callback connected";
            destroy();
        }
    }
    
    // return whether event needs be repeated or not
    return Event->repeat ();
}

// save the state of the script associated with the event
void listener_python::put_state (base::flat & file) const
{
    base::flat record;
    
    // save listener type
    record.put_uint8 ("ltp", LISTENER_PYTHON);
    
    // save base data
    listener::put_state (record);
    
    // save callback
    record.put_bool ("lmt", Method != NULL);
    if (Method != NULL)
    {
        Method->put_state (record);
        python::put_tuple (Args, record, 2);
    }
    
    file.put_flat ("", record);
}

// load the state of the script associated with the event 
bool listener_python::get_state (base::flat & file) 
{
    listener::get_state (file);

    // load callback
    if (file.get_bool ("lmt") == true)
    {
        Method = new python::method ();
        if (Method->get_state (file) == false)
        {
            LOG(ERROR) << "listener::get_state: restoring callback failed for '" << Id << "'!";
            return false;
        }
        
        Args = python::get_tuple (file, 2);
        PyTuple_SET_ITEM (Args, 0, python::pass_instance (this));
    }

    return file.success ();
}

