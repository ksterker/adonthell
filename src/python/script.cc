/*
   $Id: script.cc,v 1.6 2005/06/03 17:29:13 ksterker Exp $
  
   Copyright (C) 1999/2000/2001/2003/2004 Kai Sterker
   Copyright (C) 2001 Alexandre Courbot
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
 * @file   python/script.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the script class.
 * 
 * 
 */

#include "python/script.h"

using python::script;
using std::string;

script::script ()
{
    Instance = NULL;
    Filename = "";
    Classname = "";
}

script::~script ()
{
    clear (); 
}

// Cleanup (and re-initialisation)
void script::clear ()
{
    // Delete our Instance
    Py_XDECREF (Instance);
    Instance = NULL;

    Filename = "";
    Classname = "";
}

// Pass a (new) Python module to be used
bool script::create_instance (const string & file, const string & classname, PyObject * args)
{
    // Try to import the given script
    PyObject *module = python::import_module (file);
    if (!module) return false;

    // Instanciate!
    return instanciate (module, file, classname, args);
}

// Reload a python module in case it has changed on disk
bool script::reload_instance (const string & file, const string & classname, PyObject * args)
{
    // Try to import the given script
    PyObject *module = python::import_module (file);
    if (!module) return false;

    // Now Reload
    PyObject *reload = PyImport_ReloadModule (module);
    Py_DECREF (module);
    if (!reload) return false;

    return instanciate (reload, file, classname, args);
}

// Instanciate the given class from the module
bool script::instanciate (PyObject *module, const string & file, const string & classname, PyObject * args)
{
    // Cleanup
    clear ();

    PyObject * classobj = PyObject_GetAttrString (module, (char *) classname.c_str ());
    Py_DECREF (module);
    if (!classobj)
    {
        python::show_traceback ();
        return false;
    }
    
    // Create the Instance
    Instance = PyObject_CallObject (classobj, args);
    Py_DECREF (classobj);
    if (!Instance)
    {
        python::show_traceback ();
        return false;
    }

    Filename = file;
    Classname = classname;

    return true;
}

// Execute a method of the script
PyObject* script::call_method_ret (const string &name, PyObject *args) const
{
    PyObject *result = NULL;

    if (Instance)
    {
        PyObject *tocall = PyObject_GetAttrString (Instance, (char *) name.c_str ());

        if (PyCallable_Check (tocall) == 1)
        {
            result = PyObject_CallObject (tocall, args);
            if (!result) python::show_traceback ();
            Py_DECREF (tocall);
        }
        else
        {
            fprintf (stderr, "*** script::call_method_ret: '%s' is not callable!\n", name.c_str ());
        }
    }

    return result;
}

// check for a certain attribute
bool script::has_attribute (const string & name) const
{
    if (Instance)
        return PyObject_HasAttrString (Instance, (char *) name.c_str ());
    else
        return false;
}

// Get an attribute of the instance
PyObject *script::get_attribute (const string &name) const
{
    if (Instance)
        return PyObject_GetAttrString (Instance, (char *) name.c_str ());
    else
        return NULL;
}

// Get an int attribute of the instance
s_int32 script::get_attribute_int (const string &name) const
{
    if (Instance)
    {
        PyObject *attribute = PyObject_GetAttrString (Instance, (char *) name.c_str ());
        if (!attribute) return 0;

        s_int32 value = PyInt_AsLong (attribute);
        Py_DECREF (attribute);
        
        return value;
    }
    else
        return 0;
}

 // Get a string attribute of the instance
string script::get_attribute_string (const string &name) const
{
    if (Instance)
    {
        PyObject *attribute = PyObject_GetAttrString (Instance, (char *) name.c_str ());
        if (!attribute) return 0;

        string value = PyString_AsString (attribute);
        Py_DECREF (attribute);

        return value;
    }
    else
        return string ("");
}

// Set an attribute of the instance
void script::set_attribute (const string &name, PyObject *value)
{
    if (Instance)
        if (PyObject_SetAttrString (Instance, (char *) name.c_str (), value) == -1)
            python::show_traceback ();
    else return;
}

// Set an int attribute of the instance
void script::set_attribute_int (const string &name, s_int32 value)
{
    if (Instance)
    {
        PyObject *val = PyInt_FromLong (value);

        if (PyObject_SetAttrString (Instance, (char *) name.c_str (), val) == -1)
            python::show_traceback ();

        Py_DECREF (val);
    }
    else return;
}

// Set a string attribute of the instance
void script::set_attribute_string (const string &name, const string & value)
{
    if (Instance)
    {
        PyObject *val = PyString_FromString (value.c_str ());

        if (PyObject_SetAttrString (Instance, (char *) name.c_str (), val) == -1)
            python::show_traceback ();

        Py_DECREF (val);
    }
    else return;
}
