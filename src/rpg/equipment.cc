/*
   $Id: equipment.cc,v 1.1 2004/05/31 11:44:50 ksterker Exp $
   
   Copyright (C) 2003/2004 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

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

/**
 * Mapping from itm categories to equipment slots.
 */
std::hash_map<std::string, std::vector<std::string>,
    hash<std::string> > equipment::SlotCategoryMap;

// retrieve a list of slots the item would fit into    
const std::vector<std::string> equipment::fits (item *itm) const
{
    if (itm == NULL) return slot_list ();
    
    slot_list slots;
    std::vector<std::string>::iterator i;
    std::vector<std::string> categories = itm->categories ();
    std::hash_map<std::string, slot_list, hash<std::string> >::iterator s;

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
    std::hash_map<std::string, slot_list,
        hash<std::string> >::iterator s = SlotCategoryMap.find (category);
    
    // does the given category already exist?
    if (s != SlotCategoryMap.end ())
    {
        // add key to existing category
        slot_list *slots = &(*s).second;
        slots->insert (slot);
    }
    else
    {
        // create new category
        slot_list slots ();
        slots.insert (slot);

        SlotCategoryMap[category] = slots;
    }
}
