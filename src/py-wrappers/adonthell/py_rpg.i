%module rpg
%{

#include <string>

#include "rpg/character.h"
#include "rpg/equipment.h"
#include "rpg/item_storage.h"
#include "rpg/inventory.h"
#include "rpg/log_entry.h"
#include "rpg/log_index.h"
#include "rpg/quest.h"

using rpg::slot;
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

// typemap for returning a vector<std::string> as python list
%typemap(out) std::vector<const char*> {
    unsigned int index = 0;
    $result = PyList_New (result.size ());
    for (std::vector<const char*>::iterator i = result.begin (); i != result.end (); i++)
        PyList_SET_ITEM ($result, index++, PyString_FromString (*i));
}

%include "std_string.i"

%include "base/types.h"
%include "python/script.h"
%include "rpg/item.h"
%include "rpg/slot.h"
%include "rpg/character.h"
%include "rpg/inventory.h"
%include "rpg/equipment.h"
%include "rpg/item_storage.h"
%include "rpg/log_entry.h"
%include "rpg/log_index.h"
%include "rpg/quest.h"
