/*
 Copyright (C) 2010 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file world/action.cc
 *
 * @author Kai Sterker
 * @brief Implements the object interaction class.
 */


#include "world/action.h"
#include "python/pool.h"
#include "world/character.h"
#include "world/object.h"


using world::action;

// ctor
action::action ()
{
    Action = NULL;
    Args = NULL;
}

// dtor
action::~action()
{
    clear ();
}

// reset
void action::clear ()
{
    delete Action;
    Py_XDECREF(Args);
    
    Action = NULL;
    Args = NULL;
}

// init
bool action::init (const std::string & script, const std::string & method, PyObject *args)
{
    clear ();
    
    Action = python::pool::connect (ACTION_DIR + script, script, method);
    if (Action == NULL) return false;

    Args = python::pad_tuple (args, 2);
    if (Args == NULL) 
    {
        delete Action;
        Action = NULL;
        
        return false;
    }
    
    return true;
}

// execute action
void action::execute (world::character *actor, world::object *target)
{
    execute (python::pass_instance (actor), python::pass_instance (target));
}

// execute action
void action::execute (PyObject *actor, PyObject *target)
{
    if (Action != NULL)
    {
        // prepare arguments
        PyTuple_SET_ITEM (Args, 0, actor);
        PyTuple_SET_ITEM (Args, 1, target);
        
        Action->execute (Args);
        
        // reset
        PyTuple_SET_ITEM (Args, 0, Py_None);
        PyTuple_SET_ITEM (Args, 1, Py_None);
    }
}

// save
void action::put_state (base::flat& file) const
{
    if (Action != NULL)
    {
        base::flat record;
        Action->put_state (record);
        python::put_tuple (Args, record, 2);
        file.put_flat ("action", record);
    }
}

// load
bool action::get_state (base::flat& file)
{
    Action = new python::method();
    if (Action->get_state (file))
    {
        Args = python::get_tuple (file, 2);
        return true;
    }

    return false;
}
