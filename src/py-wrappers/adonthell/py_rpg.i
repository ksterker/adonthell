%module rpg
%feature("autodoc", "1");   // enable docstrings for python wrappers

%{
#include "rpg/character.h"
#include "rpg/dialog.h"
#include "rpg/dialog_line.h"
#include "rpg/equipment.h"
#include "rpg/item_storage.h"
#include "rpg/inventory.h"
#include "rpg/log_entry.h"
#include "rpg/log_index.h"
#include "rpg/quest_event.h"
#include "rpg/group.h"
#include "rpg/quest.h"
#include "rpg/specie.h"
#include "rpg/faction.h"

using rpg::slot;
using rpg::quest_part;
%}

namespace rpg {
    // make sure that C++ gets ownership of the item passed to item_storage::add,
    // but only if it has been actually added
    %feature("shadow") item_storage::add {
        def add (self, item):
            if _rpg.item_storage_add (self, item) == 1:
                item.thisown = 0
                return 1
            return 0
    }

    %feature("shadow") inventory::add {
        def add (self, item, count = 1):
            if _rpg.inventory_add (self, item, count) != count:
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
%include "python/script.h"
%include "rpg/item.h"
%include "rpg/slot.h"
%include "rpg/character.h"
%include "rpg/dialog.h"
%include "rpg/dialog_line.h"
%include "rpg/inventory.h"
%include "rpg/equipment.h"
%include "rpg/item_storage.h"
%include "rpg/log_entry.h"
%include "rpg/log_index.h"
%include "rpg/quest.h"
%include "rpg/quest_event.h"
%include "rpg/group.h"
%include "rpg/specie.h"
%include "rpg/faction.h"
