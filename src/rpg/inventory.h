/*
   $Id: inventory.h,v 1.1 2004/05/31 11:44:50 ksterker Exp $
   
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
 * @file   rpg/inventory.h
 * @author Kai Sterker <kaisterker@linuxgames.com> 
 * 
 * @brief  Container for items.
 */

#ifndef RPG_INVENTORY_H
#define RPG_INVENTORY_H

#include <vector>
#include "rpg/slot.h"

namespace rpg
{
    class character;
    
    /**
     * Whenever items need to be stored by an object -- be it %character,
     * chest, shop or %map -- an %inventory should be used. Inventories are
     * more than a list of items. They allow combining and using of items
     * (if the items involved provide this functionality), take care of
     * passing items between inventories and provide means to query for
     * items with certain properties.
     */
    class inventory
    {
    public:
        /**
         * Create an %inventory with 'count' anonymous slots.
         * @param size Initial size of the %inventory.
         * @param limited Whether %inventory has fixed size or not.
         */
        inventory (const u_int16 & size = 0, const bool & limited = true,
            character *owner = NULL);
            
        /**
         * Destroy inventory and its contents.
         */
        ~inventory ();
        
        /**
         * @name Slot Handling
         */
        //@{
        /**
         * Adds 'count' anonymous slots to the %inventory, thus increasing its
         * size.
         * @param count Number of slots to add.
         */
        void grow (const u_int16 & count);
        
        /**
         * Add a %slot with the given id. The id should be unique, otherwise
         * you might have troubles in retrieving the %slot with the given id.
         * The size of the %inventory will grow by one.
         * @param id Id of the %slot to add.
         * @param equipment Whether the slot is suitable for equipping items
         */
        void add_slot (const string & id, const bool & equipment = false);
        
        /**
         * Retrieve the %slot with given id from the %inventory.
         * @param id Id of the %slot to retrieve.
         * @return %slot with the given id, or \c NULL if no matching %slot exists.
         */
        slot *get_slot (const string & id);
        //@}
            
        /**
        * @name Inventory iterators
        */
        //@{
        /**
         * Retrieves the first %slot of the %inventory. All subsequent slots
         * can be retrieved via inventory::next ().
         * @return inventories first %slot, or \c NULL if there are no slots.
         */
        slot *first ();
        
        /**
         * After a call to inventory::first (), subsequent calls to this
         * method will return all slots of this %inventory, one by one.
         * Mainly useful for a GUI to display the inventories contents.
         * @return the next slot, or \c NULL if there are no more slots.
         */
        slot *next ();
        //@}
        
        /**
         * @name Item handling
         */
        //@{
        /**
         * Add item(s) to the %inventory. This method should be used when
         * the given items should not be added to a specific %slot, but to
         * this %inventory in general. It then tries to add them in the best
         * possible way: if the item(s) are stackable, they will be added to
         * an already existing stack. Otherwise they will go into the first 
         * empty %slot. If not all items fitted, the number of the remaining
         * items will be returned.
         *
         * Note that there is no explicit remove method, as item removal will
         * happen automatically.
         *
         * @param item The item(s) to add.
         * @param count The number of items in the stack.
         * @return the number of items that did not fit.
         */
        u_int32 add (item *item, const u_int32 & count = 1);
        
        /**
         * Combine item(s) in the target %slot with the item(s) from the
         * agent %slot. The combination's outcome will depend on the items
         * involved. The target may be changed or transformed into a totally
         * different item. The agent could remain unchanged or even be
         * destroyed in the process. If no combination for target and agent
         * is defined, nothing will happen.
         *
         * Note that target and agent need not be from the same inventory.
         * The items resulting from the combination will always be added
         * to this inventory (which should be that of the target).
         *
         * @param target %Slot containing the target items.
         * @param agent %Slot containing the items to combine with target.
         * @return \b true if combination succeeded, \b false otherwise.
         */
        bool combine (slot *target, slot *agent);
        //@}
    
        /**
         * @name Query Methods
         */    
        //@{
        /**
         * Values representing the different query types. It is possible
         * to find items either by their name or by their category.
         */
        enum 
        { 
            MATCH_NAME      = 1,
            MATCH_CATEGORY  = 2
        };
    
        /**
         * Find the first item that matches key. Depending on the
         * match parameter, items are either retrieved by name or by
         * category.
         * @param key The key that items must match.
         * @param match The type of query being performed.
         * @return First item matching key, or \c NULL if no match found.
         */    
        item *find (const string & key, const u_int8 & match);
        
        /**
         * Continue a query started by inventory::find. This will return
         * subsequent items matching the parameters used by the last query.
         * Note that this will not retrieve items in the same stack as the
         * one returned by inventory::find. That is, only the first item
         * of each matching stack is retrieved.
         * @return More matching items, or \c NULL if no more match found.
         */
        item *find_next ();
        //@}
        
        /**
         * @name Loading/Saving
         */
        //@{
        /**
         * Load %inventory (and its contents) from stream. 
         * @param file stream to load %invnetory from.
         * @return \b true if loading successful, \b false otherwise.
         */
        bool get_state (base::flat & file);
        
        /**
         * Save %inventory (and contents) to a stream.
         * @param file stream to save %inventory to.
         * @return \b true if saving successful, \b false otherwise.
         */
        bool put_state (base::flat & file);
        //@}
    
        /** 
         * Retrieve the %character to whom this inventory belongs.
         * @return Owner of the inventory, or \c NULL if nobody owns it. 
         */
        character *owner () const
        {
            return Owner;
        }
        
    private:
        /// The slots this inventory provides.
        std::vector<slot*> Slots;
    
        /// Inventory iterator.
        std::vector<slot*>::iterator I;
        
        /// Whether this inventory provides unlimited space or not. Inventories
        /// with unlimited space will automatically grow as items are added.
        bool Limited;
        
        /// Key of last query.
        string QueryKey;
    
        /// Type of last query.
        u_int8 QueryType;
        
        /// Character to whom this inventory is assigned.
        character *Owner;
    };
}
#endif // RPG_INVENTORY_H
