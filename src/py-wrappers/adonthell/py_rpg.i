%module rpg
%{

#include <string>
#include "rpg/character.h"
#include "rpg/equipment.h"
#include "rpg/item_storage.h"
#include "rpg/inventory.h"

using rpg::slot;
%}

%include "std_string.i"

%include "base/types.h"
%include "rpg/item.h"
%include "rpg/slot.h"
%include "rpg/character.h"
%include "rpg/inventory.h"
%include "rpg/equipment.h"
%include "rpg/item_storage.h"
