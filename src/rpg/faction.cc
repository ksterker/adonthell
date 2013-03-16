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

/**
 * @file   rpg/faction.cc
 * @author Frederico Cerveira <frederico.cerveira@gmail.com>
 *
 * @brief  Faction class.
 */

#include "faction.h"
#include <adonthell/python/python.h>

using rpg::faction;

std::vector<faction *> faction::Factions;

#define FACTION_DATA "faction.data"
#define FACTION_PACKAGE std::string("characters.factions.")

faction::faction(const std::string & name)
{
    Name = name;
    Factions.push_back(this);
}

bool faction::create_instance (const std::string & templ, PyObject * args)
{
    if (python::script::create_instance (FACTION_PACKAGE + templ, templ, args))
    {
        // add reference to faction
        set_attribute ("this", python::pass_instance (this));
        return true;
    }

    return false;
}

bool faction::verify_requirements() const
{
    /*PyObject * ret = call_method_ret("requirements");

    s_int32 ret_int = python::retrieve_instance<s_int32, s_int32>(ret);

    if (ret_int == 1)
    {
        // Success. Requirements fulfilled
        return true;

    } else if (ret_int != 0) {
        // Invalid return code
        printf ("*** faction::verify_requirements. %s Faction. Invalid return code for the \"requirements\" python func.\n",
                name().c_str());
    }

    // Failed
    return false;*/
    // TODO: Fix problem with requirements
    return true;
}

faction * faction::get_faction(const std::string & name)
{
    std::vector<faction *>::iterator i;

    for (i = Factions.begin(); i != Factions.end(); i++)
    {
        if ((*i)->name() == name)
            return (*i);
    }

    return NULL;
}

s_int32 faction::estimate_speed(const std::string & terrain) const
{
    // Call the function and get the return value
    PyObject *args = PyTuple_New (1);
    PyTuple_SetItem (args, 0, python::pass_instance (terrain.c_str()));
    PyObject * ret = call_method_ret("estimate_speed", args);

    // Convert it to an integer
    s_int32 ret_int = python::retrieve_instance<s_int32, s_int32>(ret);

    Py_XDECREF(ret);
    Py_DECREF(args);

    // If ret_int is -1 then the specie is not valid (doesn't exists or its files don't exists
    // or are corrupted) if that happens we fallback to a default value in order to prevent strange errors.
    return (ret_int == -1) ? 2 : ret_int;
}

// load factions from file
bool faction::load ()
{
    base::diskio file;

    // try to load factions
    if (!file.get_record (FACTION_DATA)) return false;

    u_int32 size;
    char *data;
    char *id;

    faction *f;
    bool result = true;

    // iterate over all saved factions
    while (file.next ((void**) &data, &size, &id) == base::flat::T_FLAT)
    {
        f = new faction ();
        base::flat record (data, size);
        if (f->get_state(record))
        {
            // loading successful
            Factions.push_back(f);
            continue;
        }

        LOG(ERROR) << "faction::load: loading faction '" << id << "' failed.";
        result = false;
        delete f;
    }

    return result;
}

// save factions to file
bool faction::save (const string & path)
{
    // try to save factions
    base::diskio file;

    std::vector<faction*>::iterator i;
    for (i = Factions.begin(); i != Factions.end(); i++)
    {
        base::flat record;
        if (!(*i)->put_state (record))
        {
            LOG(ERROR) << "faction::save: saving faction '" << (*i)->name() << "' failed!";
            return false;
        }
        file.put_flat ((*i)->name(), record);
    }

    // write factions to disk
    return file.put_record (path + "/" + FACTION_DATA);
}

void faction::cleanup()
{
    for (std::vector<faction *>::iterator i = Factions.begin(); i != Factions.end(); i++)
    {
        delete *i;
    }
    Factions.clear();
}

bool faction::put_state (base::flat & file) const
{
    // save the attributes
    file.put_string ("gnm", Name);

    // save the template this faction uses
    file.put_string ("gcn", class_name ());

    // pass record
    PyObject *args = PyTuple_New (1);
    PyTuple_SetItem (args, 0, python::pass_instance ((base::flat*) &file));

    // save the actual faction data
    call_method ("put_state", args);
    Py_DECREF (args);

    return true;
}

bool faction::get_state(base::flat & file)
{
    // get attributes
    Name = file.get_string("gnm");

    // get template to use for faction
    std::string tmpl = file.get_string("gcn");

    // instantiate
    if (!create_instance (tmpl)) return false;

    // pass file
    PyObject *args = PyTuple_New (1);
    PyTuple_SetItem (args, 0, python::pass_instance (&file));

    // load actual faction data
    call_method ("get_state", args);
    Py_DECREF (args);

    return file.success ();
}
