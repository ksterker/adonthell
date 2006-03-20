/*
   $Id: equipment.cc,v 1.6 2006/03/20 21:45:25 ksterker Exp $
   
   Copyright (C) 2003/2004/2006 Kai Sterker <kaisterker@linuxgames.com>
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
using rpg::slot_definition;
using rpg::slot;
using rpg::item;

/**
 * Mapping from %item categories to equipment slots.
 */
std::hash_map<std::string, rpg::slot_definition*,
    std::hash<std::string> > equipment::DefinedSlots;

/**
 * Predefined %equipment for certain %character types
 */
std::hash_map<std::string, std::vector<std::string>, 
    std::hash<std::string> > equipment::EquipmentSets;

// ctor
slot_definition::slot_definition (const std::string & name, const std::vector<std::string> & categories, const double & modifier)
{
    Name = name;
    Categories.insert (Categories.begin(), categories.begin(), categories.end());
    Modifier = modifier;
}

// see whether item fits into this slot
bool slot_definition::fits (item *itm) const
{
    std::vector<std::string>::iterator i;
    std::vector<std::string> categories = itm->categories ();
    
    for (i = categories.begin (); i != categories.end (); i++)
    {
        if (find (Categories.begin(), Categories.end(), *i) != Categories.end())
            return true;
    }
    
    return false;
}

// save slot definition to disk
void slot_definition::put_state (base::flat & out) const
{
    // collect categories
    base::flat categories;
    for (std::vector<std::string>::const_iterator i = Categories.begin(); i != Categories.end(); i++)
    {
        categories.put_string ("", *i);
    }
    
    // save all data
    out.put_string ("sdn", Name);    
    out.put_flat ("sdc", categories);
    out.put_double ("sdm", Modifier);
}

// read slot definition from disk
bool slot_definition::get_state (base::flat & in)
{
    char *cat;
    base::flat categories;
    
    // read variables
    Name = in.get_string ("sdn");
    categories = in.get_flat ("sdc");
    Modifier = in.get_double ("sdm");
    
    // read categories
    while (categories.next ((void **) &cat) != -1)
    {
        Categories.push_back (std::string (cat));
    }
    
    return in.success();
}

// dtor
void equipment::cleanup ()
{
    // delete all slot definitions
    std::hash_map<std::string, rpg::slot_definition*, std::hash<std::string> >::iterator s;
    for (s = DefinedSlots.begin (); s != DefinedSlots.end (); s++)
    {
        delete (*s).second;
    }
    
    DefinedSlots.clear ();
    EquipmentSets.clear ();
}

// retrieve a list of slots the item would fit into    
const rpg::slot_list equipment::available_slots (item *itm) const
{
    if (itm == NULL) return slot_list ();
    
    slot_list slots;
    std::hash_map<std::string, rpg::slot_definition*, std::hash<std::string> >::iterator s;

    // find all slots that accept the given category
    for (s = DefinedSlots.begin (); s != DefinedSlots.end (); s++)
    {
        if ((*s).second->fits (itm)) slots.insert ((*s).first);
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
    const slot_list slots = available_slots (itm);
    
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
void equipment::define_slot (const std::string & name, const std::vector<std::string> & categories, const double & modifier)
{
    std::hash_map<std::string, rpg::slot_definition*,
        std::hash<std::string> >::iterator s = DefinedSlots.find (name);
    
    // does the given slot definition already exist?
    if (s != DefinedSlots.end ())
    {
        fprintf (stderr, "*** equipment::define_slot: slot '%s' is already defined!\n", name.c_str ());
    }
    else
    {
        // create new slot definition
        DefinedSlots[name] = new slot_definition (name, categories, modifier);
    }
}

// define equipment for certain type of characters
void equipment::define_set (const std::string & type, const std::vector<std::string> & slots)
{
    std::hash_map<std::string, std::vector<std::string>,
        std::hash<std::string> >::iterator s = EquipmentSets.find (type);
    
    // does the given equipment set already exist?
    if (s != EquipmentSets.end ())
    {
        fprintf (stderr, "*** equipment::define_set: equipment set '%s' already defined!\n", type.c_str ());
    }
    else
    {
        // create new equipment set        
        EquipmentSets[type] = slots;
    }
}

// get modifier for given slot
double equipment::get_modifier (const std::string & name)
{
    std::hash_map<std::string, rpg::slot_definition*,
    std::hash<std::string> >::iterator s = DefinedSlots.find (name);
    
    // does the given slot definition exist?
    if (s == DefinedSlots.end ())
    {
        fprintf (stderr, "*** equipment::get_modifier: slot '%s' is not defined!\n", name.c_str ());
        return 0.0;
    }
    else
    {
        // create new slot definition
        return (*s).second->modifier ();
    }    
}

// get total modifier for given equipent set
double equipment::get_set_modifier (const std::string & name)
{
    double modifier = 0.0;
    std::hash_map<std::string, std::vector<std::string>,
    std::hash<std::string> >::iterator s = EquipmentSets.find (name);
    
    // does the given equipment set exist?
    if (s == EquipmentSets.end ())
    {
        fprintf (stderr, "*** equipment::get_set_modifier: equipment set '%s' not defined!\n", name.c_str ());
    }
    else
    {
        // collect all slot modifiers of equipment set
        const std::vector<std::string> &set = (*s).second;
        for (std::vector<std::string>::const_iterator i = set.begin(); i != set.end (); i++)
        {
            modifier += get_modifier (*i);
        }
    }
    
    return modifier;
}

// create equipment storage for given type of characters
rpg::inventory *equipment::create_inventory (const std::string & type)
{
    rpg::inventory *inv = new rpg::inventory ();
    std::hash_map<std::string, std::vector<std::string>,
        std::hash<std::string> >::iterator t = EquipmentSets.find (type);
    
    // does given type exist?
    if (t != EquipmentSets.end ())
    {
        const std::vector<std::string> &slots = (*t).second;
        for (std::vector<std::string>::const_iterator i = slots.begin(); i != slots.end (); i++)
        {
            // fill inventory with copies predefined slots
            inv->add_slot (*i, true);
        }
    }
    else
    {
        fprintf (stderr, "*** equipment::create_inventory: equipment set '%s' undefined!\n", type.c_str ());
    }
    
    return inv;
}

// save equipment setup to disk
void equipment::put_state (base::flat & out)
{
    base::flat record, slots;
    
    // save defined slots
    std::hash_map<std::string, rpg::slot_definition*, std::hash<std::string> >::const_iterator s;
    record.put_uint8 ("esl", (u_int8) DefinedSlots.size ());
    for (s = DefinedSlots.begin (); s != DefinedSlots.end (); s++)
    {
        (*s).second->put_state (slots);
        record.put_flat ("", slots);
        slots.clear ();
    }
    
    // save equipment sets
    std::hash_map<std::string, std::vector<std::string>, std::hash<std::string> >::const_iterator t;
    record.put_uint8 ("est", (u_int8) EquipmentSets.size ());
    for (t = EquipmentSets.begin (); t != EquipmentSets.end (); t++)
    {
        slots.put_string ("etn", (*t).first);
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
    // remove current equipment setup
    cleanup ();
    
    char *val;
    base::flat list, record = in.get_flat ("eqp");
    
    // load slot definitions
    u_int32 size = record.get_uint8 ("esl");
    while (size > 0)
    {
        list = record.get_flat ("");
        slot_definition *slot = new slot_definition ("", std::vector<std::string>(), 0.0);
        slot->get_state (list);
        DefinedSlots[slot->name()] = slot;
        size--;
    }
    
    // load equipment sets
    size = record.get_uint8 ("est");
    while (size > 0)
    {
        list = record.get_flat ("");
        std::string name = list.get_string ("etn");
        std::vector<std::string> slots;
        while (list.next ((void **) &val) == base::flat::T_STRING)
        {
            slots.push_back (std::string (val));
        }
        define_set (name, slots);
        size--;
    }
    
    return in.success ();
}
