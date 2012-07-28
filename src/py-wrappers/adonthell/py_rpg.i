%module rpg
%feature("autodoc", "1");   // enable docstrings for python wrappers

%{
#include <adonthell/rpg/character.h>
#include <adonthell/rpg/dialog.h>
#include <adonthell/rpg/dialog_line.h>
#include <adonthell/rpg/equipment.h>
#include <adonthell/rpg/item_storage.h>
#include <adonthell/rpg/inventory.h>
#include <adonthell/rpg/log_entry.h>
#include <adonthell/rpg/log_index.h>
#include <adonthell/rpg/quest_event.h>
#include <adonthell/rpg/group.h>
#include <adonthell/rpg/quest.h>
#include <adonthell/rpg/specie.h>
#include <adonthell/rpg/faction.h>

using rpg::slot;
using rpg::quest_part;

extern "C" {
    void check_module_version (const char *name, const unsigned int & module_ver);
}
%}

%init %{
    check_module_version (SWIG_name, SWIGVERSION);
%}

namespace rpg {
    // make sure that C++ gets ownership of the item passed to item_storage::add,
    // but only if it has been actually added
    %rename (_forbidden_add) item_storage::add;   
    
    %extend item_storage {
        %pythoncode {
            def add (item):
                if _rpg.item_storage__forbidden_add (item) == 1:
                    item.thisown = 0
                    return 1
                return 0
            add = staticmethod(add)
        }
    }

    %feature("shadow") inventory::add {
        def add (self, item, count = 1):
            if _rpg.inventory_add (self, item, count) != count:
                item.thisown = 0
            return count
    }
    
    %feature("shadow") slot::add {
        def add (self, item, count = 1):
            if _rpg.slot_add (self, item, count) != count:
                item.thisown = 0
            return count
    }   
}

// typemap for returning a vector<const char*> as python list
%typemap(out) std::vector<const char*> {
    unsigned int index = 0;
    $result = PyList_New (result.size ());
    for (std::vector<const char*>::iterator i = result.begin (); i != result.end (); i++)
        PyList_SET_ITEM ($result, index++, PyString_FromString (*i));
}

%include "stdint.i"
%include "std_string.i"

// typemap for passing python list as vector<string>
%typemap(in) const std::vector<std::string> & {
    $1 = new std::vector<std::string>;
    if (PyList_Check ($input)) {
        int size = PyList_Size ($input);
        for (int i = 0; i < size; i++) {
            PyObject *o = PyList_GetItem ($input, i);
            if (PyString_Check(o)) $1->push_back (PyString_AsString (o));
            else fprintf (stderr, "*** $symname: expected list of strings!\n");
        }
    }
    else fprintf (stderr, "*** $symname: expected list of strings!\n");
}

%typemap(freearg) const std::vector<std::string> & "delete $1;"

%import "base/types.h"
%import(module="event") "event/event.h"
%include <adonthell/python/script.h>
%include <adonthell/rpg/item.h>
%include <adonthell/rpg/slot.h>
%include <adonthell/rpg/character.h>
%include <adonthell/rpg/dialog.h>
%include <adonthell/rpg/dialog_line.h>
%include <adonthell/rpg/inventory.h>
%include <adonthell/rpg/equipment.h>
%include <adonthell/rpg/item_storage.h>
%include <adonthell/rpg/log_entry.h>
%include <adonthell/rpg/log_index.h>
%include <adonthell/rpg/quest.h>
%include <adonthell/rpg/quest_event.h>
%include <adonthell/rpg/group.h>
%include <adonthell/rpg/specie.h>
%include <adonthell/rpg/faction.h>
