/*
   $Id: slot.h,v 1.2 2004/08/23 06:33:47 ksterker Exp $
   
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
 * @file   rpg/slot.h
 * @author Kai Sterker <kaisterker@linuxgames.com> 
 * 
 * @brief  One unit of the inventory.
 */

#ifndef RPG_SLOT_H
#define RPG_SLOT_H

#include "base/flat.h"

namespace rpg
{
    class item;
    class inventory;
    
    /**
     * The %slot class may contain one item or a stack of (roughly) equal
     * items. It is used by inventories, which are basically a list of
     * slots.
     */
    class slot
    {
    public:
        /**
         * Create a new %slot with an optional id. The id can be used to
         * retrieve certain slots from the %inventory. The equipment flag
         * determines whether this %slot will be able to contain equipped items,
         * i.e. those worn or held by a %character. The manager::equip () method
         * will only accept slots with the equipment flag set to true.
         * @param owner The %inventory the %slot belongs to
         * @param id Identifier for the %slot
         * @param equipment \b true to allow equipping items in that slot
         */
        slot (inventory *owner, const string & id = "", const bool & equipment = false);
    
        /**
         * Delete a slot.
         */
        ~slot ();
        
        /**
         * @name Item handling
         */
        //@{
        /**
         * Retrieve pointer to the item kept in this %slot. In case of an
         * immutable item, it will also adjust item::Slot. To check
         * whether a slot is empty, use the count() method below; it's faster.
         * @return item in this %slot, or \c NULL in case it is empty.
         */
        item *get_item ();
    
        /**
         * Checks whether the given item is allowed to go into this %slot.
         * This is the case when the %slot is empty, or if the items
         * already in the %slot (roughly) equal the given item.
         * @param item Item to test against.
         * @return \b true if the item may go into the %slot, \b falso otherwise
         */
        bool accepts (item *item);
        
        /**
         * Add an item or stack of items to this %slot. If the %slot is not
         * empty, the given item must be of roughly the same kind (i.e. have 
         * the same name) as the item kept in the %slot and it must be stackable. 
         * If more items are given than fit, the remaining number will be
         * returned.
         *
         * If items that are added come from another %inventory, they will be
         * removed from there automatically, as long as they fit into this %slot.
         *
         * @param item the item or stack of items to be added.
         * @param count number of items in the stack.
         * @return number of items that didn't fit into the %slot.
         */
        u_int32 add (item * item, const u_int32 & count = 1);
        
        /**
         * Removes the topmost item(s) from the %slot, if possible. Up to count
         * items are removed, unless there aren't as many items in the slot.
         * Note that mutable items are only removed, but not deleted.
         *
         * @param item the item to be removed from the slot.
         * @param count number of items to be removed.
         * @return number of items that could not be removed.
         */
        u_int32 remove (item * item, const u_int32 & count = 1);
        
        /**
         * Remove all items from the slot, no matter whether they are mutable
         * or not.
         */
        void clear ();
        
        /**
         * Retrieve the number of items in this %slot. This is the preferred
         * method to check whether a slot is empty or not, as it has much less
         * overhead than get_item() above.
         * @return number of items this %slot contains.
         */
        u_int32 count () const
        {
            return Count;
        }
        //@}
    
        /**
         * Return the inventory this slot belongs to.
         * @return inventory this slot belongs to.
         */
        inventory *owner () const
        {
            return Owner;
        }
            
        /**
         * Attribute access
         */
        //@{
        /**
         * Retrieve the slot's id.
         * @return id of the %slot.
         */
        const string & id () const
        {
            return Id;
        }
        
        /**
         * Give this %slot a new Id.
         * @param id A new identifier for this %slot. 
         */
        void set_id (const string & id)
        {
            Id = id;
        }
    
        /**
         * Is that slot 'disabled'?
         * @return \b true if that is the case, \b false otherwise.
         */
        bool is_negated () const
        {
            return Negated;
        }    
        
        /**
         * Disable or enable this slot. It's up to users of the slot to
         * make use of this flag.
         * @param negated \b false to enable, \b true to disable the slot. 
         */
        void set_negated (const bool & negated)
        {
            Negated = negated;
        }
        
        /**
         * Is this slot meant for equipped items?
         * @return \b true if that is the case, \b false otherwise.
         */
        bool is_equipment () const
        {
            return Equipment;
        }    
        
        void set_equipment (const bool & equipment)
        {
            Equipment = equipment;
        }
        //@}
        
        /**
         * @name Loading/Saving
         */
        //@{
        /**
         * Load %slot and its contents from stream. 
         * @param file stream to load %slot from.
         * @return \b true if loading successful, \b false otherwise.
         */
        bool get_state (base::flat & file);
        
        /**
         * Save %slot and its contents to stream.
         * @param file stream to save %slot to.
         * @return \b true if saving successful, \b false otherwise.
         */
        bool put_state (base::flat & file) const;
        //@}
        
    private:
        /// Identifier for this %slot, to retrieve it from its %inventory
        string Id;
    
        /// The %inventory this %slot belongs to.
        inventory *Owner;
        
        /// The item(s) currently kept in this %slot.
        item *Item;
        
        /// The number of items in this %slot.
        u_int32 Count;
        
        /// Whether the slot is disabled.
        bool Negated;
        
        /// Whether this slot is for equipping items.
        bool Equipment;
    };
}
#endif // RPG_SLOT_H
