/*
   $Id: equipment.cc,v 1.4 2004/10/18 07:40:23 ksterker Exp $
   
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
 * @file   equipment.cc
 * @author Kai Sterker <kaisterker@linuxgames.com> 
 * 
 * @brief  Takes care of equipping/unequipping items.
 */

#include <algorithm>
#include "rpg/equipment.h"
#include "rpg/inventory.h"

using rpg::equipment;
using rpg::slot;
using rpg::item;

/**
 * Mapping from %item categories to equipment slots.
 */
std::hash_map<std::string, rpg::slot_list,
    std::hash<std::string> > equipment::SlotCategoryMap;

/**
 * Predefined %equipment for certain %character types
 */
std::hash_map<std::string, std::vector<std::string>, 
    std::hash<std::string> > equipment::EquipmentDefs;

// retrieve a list of slots the item would fit into    
const rpg::slot_list equipment::fits (item *itm) const
{
    if (itm == NULL) return slot_list ();
    
    slot_list slots;
    std::vector<std::string>::iterator i;
    std::vector<std::string> categories = itm->categories ();
    std::hash_map<std::string, rpg::slot_list, std::hash<std::string> >::iterator s;

    // find all slots that accept the given category
    for (i = categories.begin (); i != categories.end (); i++)
    {
        s = SlotCategoryMap.find (*i);
        if (s == SlotCategoryMap.end ()) continue;
        
        // append some slots
        slots.insert ((*s).second.begin (), (*s).second.end ());
    }
    
    return slots;
}

// check whether the item can be equipped in given slot
bool equipment::accepts (slot* target, item* itm) const
{
    // sanity check
    if (!target || !target->owner () || !itm || !target->is_equipment () ||
         target->is_negated ()) return false;
    
    // get slots suitable for this item
    const slot_list slots = fits (itm);
    
    // is our slot among them?
    if (slots.find (target->id ()) == slots.end ())
        return false;
    
    // whether the item can be actually equipped depends on the character
    return itm->can_equip (target->owner ()->owner ());
}

// try to equip given item
item *equipment::equip (slot *target, item *itm, u_int32 *count)
{
    // nothing to do
    if (*count == 0) return NULL;
    
    // item doesn't fit in slot for some reason.
    if (!accepts (target, itm)) return itm;

    slot *source = itm->get_slot ();

    // make sure that unequip is invoked before item is equipped elsewhere
    if (source && source->is_equipment () && source->count () == *count)
        itm->unequipped (source);
    
    // target slot occupied?
    if (target->count () > 0 && !target->accepts (itm))
    {
        // exchange items if they are not stackable
        u_int32 old_count = target->count ();
        item *old_item = unequip (target, &old_count);
    
        target->add (itm, *count);
        itm->equipped (target);
        
        *count = old_count;
        return old_item;
    }
    else
    {
        // otherwise try to add as many items as possible
        u_int32 remaining = target->add (itm, *count);
        
        // have any items been equipped?
        if (remaining < *count)
            itm->equipped (target);
        
        // adjust count
        *count = remaining;
        
        if (*count > 0) return itm;
        else return NULL;
    }
}

// unequip items from equipment slot
item *equipment::unequip (slot *target, u_int32 *count)
{
    // nothing to do
    if (*count == 0) return NULL;

    // the item we will remove from the slot
    item *itm = target->get_item ();

    if (itm != NULL)
    {
        // try to remove the requested number of items    
        *count -= target->remove (itm, *count);
    
        // let the character know when a slot was totally emptied.
        if (target->count () == 0) itm->unequipped (target);
    
        // returned item(s) are no longer within a slot
        itm->set_slot (NULL);
    }
   
    return itm;
}

// tell equipment that given slot accepts given category
void equipment::add_mapping (const std::string & slot, const std::string & category)
{
    std::hash_map<std::string, rpg::slot_list,
        std::hash<std::string> >::iterator s = SlotCategoryMap.find (category);
    
    // does the given category already exist?
    if (s != SlotCategoryMap.end ())
    {
        // add key to existing category
        slot_list &slots = (*s).second;
        slots.insert (slot);
    }
    else
    {
        // create new category
        slot_list slots;
        slots.insert (slot);

        SlotCategoryMap[category] = slots;
    }
}

// define equipment for certain type of characters
void equipment::add_definition (const std::string & type, const std::vector<std::string> & slots)
{
    EquipmentDefs[type] = slots;
}

// create equipment storage for given type of characters
rpg::inventory *equipment::create (const std::string & type)
{
    rpg::inventory *inv = new rpg::inventory ();
    std::hash_map<std::string, std::vector<std::string>,
        std::hash<std::string> >::iterator t = EquipmentDefs.find (type);
    
    // does given type exist?
    if (t != EquipmentDefs.end ())
    {
        const std::vector<std::string> &slots = (*t).second;
        for (std::vector<std::string>::const_iterator i = slots.begin(); i != slots.end (); i++)
        {
            // fill inventory with predefined slots
            inv->add_slot (*i, true);
        }
    }
    else
    {
        fprintf (stderr, "*** equipment::create: character type '%s' undefined!\n", type.c_str ());
    }
    
    return inv;
}

// save to disk
void equipment::put_state (base::flat & out)
{
    base::flat record, slots;
    
    // save mappings
    std::hash_map<std::string, rpg::slot_list,
        std::hash<std::string> >::const_iterator s = SlotCategoryMap.begin ();
    for (; s != SlotCategoryMap.end (); s++)
    {
        slots.put_string ("emp", (*s).first);
        for (rpg::slot_list::const_iterator i = (*s).second.begin (); i != (*s).second.end (); i++)
        {
            slots.put_string ("", *i);
        }
        record.put_flat ("", slots);
        slots.clear ();
    }
    
    // save equipment definitions
    std::hash_map<std::string, std::vector<std::string>,
        std::hash<std::string> >::const_iterator t = EquipmentDefs.begin ();
    for (; t != EquipmentDefs.end (); t++)
    {
        slots.put_string ("edf", (*t).first);
        for (std::vector<std::string>::const_iterator i = (*t).second.begin (); i != (*t).second.end (); i++)
        {
            slots.put_string ("", *i);
        }
        record.put_flat ("", slots);
        slots.clear ();
    }
    
    out.put_flat ("eqp", record);
}

// load from disk
bool equipment::get_state (base::flat & in)
{
    base::flat list, record = in.get_flat ("eqp");
    
    while (in.next ((void **) &list) != -1)
    {
        char *val, *name;
        if (list.next ((void **) &val, (int*) NULL, &name) != base::flat::T_STRING)
            continue;
        
        // load mapping
        if (strncmp (name, "emp", 3) == 0)
        {   
            std::string category = val;
            while (list.next ((void **) &val) == base::flat::T_STRING)
            {
                add_mapping (val, category);
            }
            continue;
        }

        // load equipment def
        if (strncmp (name, "edf", 3) == 0)
        {
            std::string type = val;
            std::vector<std::string> slots;
            while (list.next ((void **) &val) == base::flat::T_STRING)
            {
                slots.push_back (std::string (val));
            }
            add_definition (type, slots);
        }
    }
    
    return in.success ();
}
