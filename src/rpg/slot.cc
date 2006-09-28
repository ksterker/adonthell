/*
   $Id: slot.cc,v 1.4 2006/09/28 19:13:27 gnurou Exp $
   
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
 * @file   rpg/slot.cc
 * @author Kai Sterker <kaisterker@linuxgames.com> 
 * 
 * @brief  One unit of the inventory.
 */

#include "rpg/item_storage.h"
#include "rpg/slot.h"

using rpg::inventory;
using rpg::item;
using rpg::slot;

// ctor
slot::slot (inventory *owner, const string & id, const bool & equipment)
{
    Owner = owner;
    Id = id;
    Count = 0;
    Item = NULL;
    Negated = false;
    Equipment = equipment;
}

// dtor
slot::~slot ()
{
    // if the slot contains item(s), delete them
    if (Item && Item->is_mutable ())
    {
        Item->set_slot (NULL);
        delete Item;
    }
}

// retrieve item(s) in slot
item *slot::get_item ()
{
    // temporarily assign slot to immutable items
    if (Item && !Item->is_mutable ()) Item->set_slot (this);
    return Item;
}

// check whether slot accepts given item
bool slot::accepts (item *item)
{    
    if (!Item) return true;
    else return Item->equals (item);
}

// add item(s) to slot
u_int32 slot::add (item *item, const u_int32 & count)
{
    // nothing needs be done
    if (count == 0) return 0;

    // if we have an empty slot, we can just add the items
    if (!Item) Item = item;
    
    // otherwise check whether given items may go into this slot 
    else 
    {
        // items from same slot?
        if (this == item->get_slot ()) return 0;
        
        // item stackable?
        if (Count >= Item->max_stack ()) return count;
        
        // items of same kind?
        if (!(Item->equals (item))) return count;
        
        // actually stack mutable items
        if (item->is_mutable ()) Item->stack (item);
    }
    
    // calculate how many items will fit into the slot
    u_int32 fitting = item->max_stack () - Count;
    fitting = (fitting > count) ? count : fitting;
    
    // remove items from the inventory they came
    if (item->get_slot ())
        // make sure we don't add more items than we remove
        fitting -= item->get_slot ()->remove (item, fitting);
    
    // adjust counts
    Count += fitting;
    
    // adjust slots
    if (item->is_mutable ()) item->set_slot (this);
    
    // return number of items that weren't added
    return count - fitting;
}

// remove item(s) from slot
u_int32 slot::remove (item *itm, const u_int32 & count)
{
    // nothing needs be done
    if (!itm || count == 0) return 0;

    // in case items match, remove count items
    if (Item == itm)
    {
        u_int32 remove = (Count > count) ? count : Count;
        
        // for immutable items, simply decrease the refcount
        Count -= remove;
        
        // for mutable items, also remove them from the stack
        if (Item->is_mutable ()) Item = Item->split (remove);
        
        // slot empty?
        if (Count == 0) Item = NULL;
        
        // return number of items that couldn't be removed
        return count - remove;
    }
    
    // otherwise search through the stack to remove the item
    for (item *i = Item; i != NULL; i = i->Next)
        if (i->Next == itm)
        {
            i->Next = itm->Next;
            itm->Next = NULL;
            itm->Slot = NULL;
            Count--;
            
            return count - 1;
        }
        
    return count;
}

// empty the slot
void slot::clear ()
{
    // if the slot contains mutable item(s), delete them
    if (Item && Item->is_mutable ()) 
    {
        Item->set_slot (NULL);
        delete Item;
    }
    
    Item = NULL;
    Count = 0;
}

// save slot to disk
bool slot::put_state (base::flat & file) const
{
    // save slot attributes
    file.put_string ("sid", Id);
    file.put_uint32 ("sct", Count);
    file.put_bool ("snt", Negated);
    file.put_bool ("set", Equipment);
    
    // save the item(s) in the slot
    if (Count > 0) 
    {
        file.put_bool ("sim", Item->is_mutable ());
        
        // completely save mutable items
        if (Item->is_mutable ()) Item->put_state (file);
        
        // otherwise only save unique item id
        else file.put_string ("sin", Item->name ()); 
    }
    
    return true;
}

// load slot from disk
bool slot::get_state (base::flat & file)
{
    // get slot attributes
    Id = file.get_string ("sid");
    Count = file.get_uint32 ("sct");
    Negated = file.get_bool ("snt");
    Equipment = file.get_bool ("set");
    
    // get item(s) if any
    if (Count > 0)
    {
        // mutable items need to be instanciated and loaded
        if (file.get_bool ("sim") == true) 
        {
            Item = new item (true);
            Item->get_state (file);
            Item->set_slot (this);
        }
        
        // immutable items can be retrieved from the global item storage
        else
        {
            std::string name = file.get_string ("sin");
            if (file.success ()) Item = item_storage::get (name);
        }
    }
    
    return file.success ();
}
