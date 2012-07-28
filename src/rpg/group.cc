/*
   Copyright (C) 2009 Frederico Cerveira <frederico.cerveira@gmail.com>
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

#include "group.h"
#include <adonthell/python/python.h>

using rpg::group;

group::group(const std::string & name)
{
    set_name(name);
}

bool group::create_instance (const std::string & templ, PyObject * args)
{
    if (python::script::create_instance (Group_Package + templ, templ, args))
    {
        // add reference to group
        set_attribute ("this", python::pass_instance (this));
        return true;
    }

    return false;
}

s_int32 group::estimate_speed_for_pathfinding(const std::string & terrain) const
{
    // Call the function and get the return value
    PyObject *args = PyTuple_New (1);
    PyTuple_SetItem (args, 0, python::pass_instance (terrain.c_str()));
    PyObject * ret = call_method_ret("estimate_speed_for_pathfinding", args);

    // Convert it to an integer
    s_int32 ret_int = python::retrieve_instance<s_int32, s_int32>(ret);

    Py_XDECREF(ret);
    return ret_int;
}

s_int32 group::estimate_speed(const std::string & terrain) const
{
    // Call the function and get the return value
    PyObject *args = PyTuple_New (1);
    PyTuple_SetItem (args, 0, python::pass_instance (terrain.c_str()));
    PyObject * ret = call_method_ret("estimate_speed", args);

    // Convert it to an integer
    s_int32 ret_int = python::retrieve_instance<s_int32, s_int32>(ret);

    Py_XDECREF(ret);

    // If ret_int is -1 then the specie is not valid (doesn't exists or its files don't exists
    // or are corrupted) if that happens we fallback to a default value in order to prevent strange errors.
    return (ret_int == -1) ? 2 : ret_int;
}

bool group::put_state (const string & file, const base::diskio::file_format & format) const
{
    // try to save group
    base::diskio record (format);
    if (!put_state (record))
    {
        fprintf (stderr, "*** group::put_state: saving '%s' failed!\n", file.c_str ());
        return false;
    }

    // write group to disk
    return record.put_record (file);
}


bool group::put_state (base::flat & file) const
{
    base::flat record;

    // save the attributes
    record.put_string ("gnm", Name);

    record.put_string ("pcp", Pathfinding_Costs_Path);

    // save the template this group uses
    record.put_string ("gcn", class_name ());

    // pass record
    PyObject *args = PyTuple_New (1);
    PyTuple_SetItem (args, 0, python::pass_instance ((base::flat*) &record));

    // save the actual group data
    call_method ("put_state", args);
    Py_DECREF (args);


    file.put_flat ("grp", record);
    return true;
}

// load a single group from file
bool group::get_state(const string & file)
{
    // try to load group
    base::diskio record (base::diskio::XML_FILE);

    if (record.get_record (file))
        return get_state (record);

    return false;
}

bool group::get_state(base::flat & file)
{
    base::flat record = file.get_flat ("grp");
    if (!file.success ()) return false;


    // get attributes
    set_name(record.get_string("gnm"));

    // get the path to the %pathfinding_costs data
    Pathfinding_Costs_Path = record.get_string("pcp");

    // get template to use for group
    std::string tmpl = record.get_string("gcn");

    // instanciate
    if (!create_instance (tmpl)) return false;

    // pass file
    PyObject *args = PyTuple_New (1);
    PyTuple_SetItem (args, 0, python::pass_instance (&record));

    // load actual group data
    call_method ("get_state", args);
    Py_DECREF (args);

    // add reference to item
    set_attribute ("this", python::pass_instance (this));

    return record.success ();
}
