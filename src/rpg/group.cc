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

#include "rpg/group.h"
#include "python/python.h"

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
    s_int32 ret_int = python::retrieve_instance<s_int32, s_int32>(&*ret);

    return ret_int;
}

s_int32 group::estimate_speed(const std::string & terrain) const
{
    // Call the function and get the return value
    PyObject *args = PyTuple_New (1);
    PyTuple_SetItem (args, 0, python::pass_instance (terrain.c_str()));
    PyObject * ret = call_method_ret("estimate_speed", args);

    // Convert it to an integer
    s_int32 ret_int = python::retrieve_instance<s_int32, s_int32>(&*ret);

    return ret_int;
}
