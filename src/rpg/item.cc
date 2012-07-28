/*
   $Id: item.cc,v 1.8 2009/04/08 19:36:02 ksterker Exp $
   
   Copyright (C) 2003/2004 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file   rpg/item.cc
 * @author Kai Sterker <kaisterker@linuxgames.com> 
 * 
 * @brief  Superclass for all items.
 */

//#include <stdio.h>
#include "item.h"
#include "character.h"
#include <adonthell/base/diskio.h>
#include <iostream>
#include <algorithm>

using rpg::item;
using rpg::character;

// ctor
item::item (const string & item) : python::script ()
{
    Slot = NULL;

    get_state (item);    
}

// dtor
item::~item ()
{
    if (Mutable)
    {
        // if the item is still inside an inventory, remove it
        if (Slot) Slot->remove (this);
    
        // if we have a stack of items, delete the whole stack
        if (Next) delete Next;
    }
        
    python::script::clear ();
}

// can the given character equip this item?
bool item::can_equip (character *character)
{
    // no 
    if (!has_attribute ("can_equip")) return false;
    
    int result = 0;
    
    // pass character
    PyObject *args = PyTuple_New (1);
    PyTuple_SetItem (args, 0, python::pass_instance (character));
    
    // call method
    PyObject *retval = call_method_ret ("can_equip", args);
    
    // if we have valid retval retrieve it
    if (PyInt_Check (retval)) result = PyInt_AS_LONG (retval);

    // cleanzp
    Py_XDECREF (retval);
    Py_DECREF (args);
    
    return (result != 0);
}

// tell item it has been equipped
void item::equipped (slot *target)
{
    // no effect
    if (!has_attribute ("equipped")) return;
    
    if (!Mutable) Slot = target;

    // call method
    call_method ("equipped");
    
    if (!Mutable) Slot = NULL;
}

// tell item it has been removed
void item::unequipped (slot *target)
{
    // no effect
    if (!has_attribute ("unequipped")) return;
    
    if (!Mutable) Slot = target;
    
    // call method
    call_method ("unequipped");
    
    if (!Mutable) Slot = NULL;
}

// trigger item's main functionality
bool item::use (character *character)
{
    // can't be used
    if (!has_attribute ("use")) return false;

    int result = 1;
    
    // pass character
    PyObject *args = PyTuple_New (1);
    PyTuple_SetItem (args, 0, python::pass_instance (character));
    
    // call method
    PyObject *retval = call_method_ret ("use", args);
    
    // if we have valid retval retrieve it
    if (PyInt_Check (retval)) result = PyInt_AS_LONG (retval);
    
    // cleanzp
    Py_XDECREF (retval);
    Py_DECREF (args);
    
    return (result != 0);
}        

// combine two items
item *item::combine (item *itm)
{
    // can't be combined
    if (!has_attribute ("combine")) return NULL;
     
    // pass item
    PyObject *args = PyTuple_New (1);
    PyTuple_SetItem (args, 0, python::pass_instance (itm));
    
    // call method
    PyObject *retval = call_method_ret ("combine", args);
    
    // try to retrieve result
    item *result = python::retrieve_instance<item*, item> (retval);
    
    // cleanzp
    Py_XDECREF (retval);
    Py_DECREF (args);

    return result;
}

// save a single item to file
bool item::put_state (const string & file, const base::diskio::file_format & format) const
{
    // try to save item
    base::diskio record (format);
    if (!put_state (record))
    {
        fprintf (stderr, "*** item::put_state: saving '%s' failed!\n", file.c_str ());        
        return false;
    }
    
    // write item to disk
    return record.put_record (file);
}

// save item to stream
bool item::put_state (base::flat & file) const
{
    // do we have a valid item?
    if (!Instance) return false;
    
    base::flat record;
    
    // save the attributes
    record.put_bool ("ime", Mutable);
    record.put_uint32 ("ims", MaxStack);

    // save the template this item uses
    record.put_string ("icn", class_name ());

    // pass record
    PyObject *args = PyTuple_New (1);
    PyTuple_SetItem (args, 0, python::pass_instance ((base::flat*) &record));
    
    // save the actual item data
    call_method ("put_state", args);
    Py_DECREF (args);
    
    // recursively save stack
    record.put_bool ("inx", Next != NULL);
    if (Next != NULL) Next->put_state (record);
    
    file.put_flat ("itm", record);
    return true;
}

// load a single item from file
bool item::get_state (const string & file)
{
    // try to load item
    base::diskio record (base::diskio::BY_EXTENSION);
    
    if (record.get_record (file)) 
        return get_state (record);
    
    return false;
}

// load item from stream
bool item::get_state (base::flat & file)
{
    base::flat record = file.get_flat ("itm");
    if (!file.success ()) return false;
    
    // clean up, if neccessary
    if (Instance) clear ();
    
    // get attributes
    Mutable = record.get_bool ("ime");
    MaxStack = record.get_uint32 ("ims");
    
    // get template to use for item
    std::string tmpl = record.get_string ("icn");
    
    // instanciate
    if (!create_instance (tmpl)) return false;

    // pass file
    PyObject *args = PyTuple_New (1);
    PyTuple_SetItem (args, 0, python::pass_instance (&record));
    
    // load actual item data
    call_method ("get_state", args);
    Py_DECREF (args);
    
    // add reference to item
    set_attribute ("this", python::pass_instance (this));
    
    // recursively get stack
    if (record.get_bool ("inx") == true)
    {
        Next = new item (true);
        Next->get_state (record);
    }
    else Next = NULL;
    
    return record.success ();
}

// return list of categories this item belongs to
std::vector<string> item::categories () const
{
    std::vector<string> categories;
    PyObject *list = get_attribute ("Categories");
    PyObject *str;
            
    if (!PySequence_Check (list)) return categories;
    
    // get all category strings
    for (int i = 0; i < PySequence_Size (list); i++)
    {
        str = PySequence_GetItem (list, i);
        if (PyString_Check (str))
            categories.push_back (PyString_AsString (str));
        Py_XDECREF (str);
    }
    
    // cleanup
    Py_DECREF (list);
    
    return categories;
}


/*
// recharge item
u_int16 item::recharge (u_int16 &charge)
{
    Charge += charge;

    if (Charge > MaxCharge)
    {
        charge = Charge - MaxCharge;
        Charge = MaxCharge;
    }
    else charge = 0;
    
    return charge;
}
*/

// compare two items for equality
bool item::equals (item *item)
{
    if (!Mutable) return this == item;
    else return name () == item->name ();
}
        
// add item(s) to this stack
void item::stack (item *itm)
{
    item *bottom = this;
    
    // add given stack on top of our own stack
    while (bottom->Next) bottom = bottom->Next;
    bottom->Next = itm;
}

// remove item(s) from this stack
item *item::split (u_int32 count)
{
    // in case of count = 0, this is a noop
    if (count == 0) return this;
    
    item *remaining, *stack = this;
    
    // retrieve the topmost items
    while (--count) stack = stack->Next;
    remaining = stack->Next;
    
    // split this stack
    stack->Next = NULL;
    set_slot (NULL);
    
    return remaining;    
}
