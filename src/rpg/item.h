/*
   $Id: item.h,v 1.1 2004/05/31 11:44:50 ksterker Exp $
   
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
 * @file   rpg/item.h
 * @author Kai Sterker <kaisterker@linuxgames.com> 
 * 
 * @brief  Superclass for all items.
 */


#ifndef RPG_ITEM_H
#define RPG_ITEM_H

#include <vector>
#include "python/script.h"
#include "rpg/slot.h"

using std::string;

namespace rpg {
    class character;

    /**
     * The Python package containing %item templates.
     */
#ifndef SWIG
#   define ITEM_PACKAGE "item."
#endif

    /**
     * It is a wrapper for item.py, which is the actual %item superclass.
     * For flexibility, items are implemented on python side. But since they
     * are often used on C++ side, this class provides methods to the most
     * basic %item attributes and all possible actions an %item might support.
     *
     * So while each item does have the wrapped attributes, an %item may only
     * implement a subset of the available actions ("pick_up", "drop", "equip",
     * "unequip", "combine" and "use"). Further, an %item may have additional
     * attributes. If required, they can be accessed via methods provided by
     * the %script class.
     *
     * There are two 'classes' of items. Of immutable items, only one %object
     * will be instanciated when loading a saved %game. All copies of such an
     * %item are actually references to the one object. Mutable items however 
     * will be instanciated as often as copies of that %item exist.
     */
    class item : public python::script 
    {
    public:
    
        /**
         * Default constructor. Creates an empty %item. For safety reasons
         * the items 'class', i.e whether it is mutable or immutable has 
         * to be specified at construction time. It cannot be changed later.
         * @param is_mutable whether the %item may change its state or not.
         * @see is_mutable ()
         */
        item (bool is_mutable) : script () 
        {
            Mutable = is_mutable;
            MaxStack = 1;
            Next = NULL;
            Slot = NULL;
        }
        
    #ifndef SWIG    
        /**
         * Create a new %item from the given %item data. 
         * @param itm Name of the %item data file defining the desired item.
         */
        item (const string & itm);
    #endif
            
        /**
         * Destructor.
         */
        ~item ();
        
        /**
         * Removes an %item from its slot and deletes it if necessary. This
         * method should be preferred over deleting an item directly, as it
         * takes care of both mutable and immutable items.
         */
        void destroy ()
        {
            if (!Mutable)
            {
                if (Slot) Slot->remove (this);
            }
            else delete this;
        }
        
        /**
         * @name Item Actions
         */
        //@{
        /**
         * This method is invoked when the %item is equipped by a character. It can
         * be used to trigger special events.
         * @param target the slot to which the items where added
         */
        void equipped (slot *target);
        
        /**
         * This method is invoked when this %item is unequipped by a character.
         * It can be used to trigger special events.
         * @param target the slot from which the items where removed
         */
        void unequipped (slot *target);
        
        /**
         * Check whether this %item can be equipped by the given character.
         * @param character The character that wants to equip this %item.
         * @return \b true if the character can equip the %item, \b false otherwise.
         */
        bool can_equip (character *character);
        
        /**
         * Use an item. Triggers its main functionality if it can be used at all.
         * @param character The character using this item.
         * @return \b true if item has been used successfully, \b false otherwise. 
         */
        bool use (character *character);
    
        /**
         * Combine this %item with another %item. Returns the result of the
         * combination, or NULL if the combination fails.
         * @param itm the %item to combine with this one
         * @return %item resulting from combination, or \c NULL if combination fails.
         */
        item *combine (item * itm);
        //@}
                
        /**
         * @name Attribute access
         */
        //@{
        /**
         * Get the items name.
         * @return name of the %item.
         */
        string name ()
        {
            return get_attribute_string ("Name");
        }
    
        /**
         * Set the items name.
         * @param name the items new name.
         */
        void set_name (const string & name)  
        {
            set_attribute_string ("Name", name); 
        }
        
        /**
         * Make the %item member of the given category.
         * @param category the category this %item should belong to.
         */
        void add_category (const string & category)
        {
            PyObject *args = Py_BuildValue ("(s)", (char *) category.c_str ());
            call_method ("add_category", args);
        }
        
        /**
         * Take a category away from the %item.
         * @param category the category to remove from the %item. 
         */
        void remove_category (const string & category)
        {
            PyObject *args = Py_BuildValue ("(s)", (char *) category.c_str ());
            call_method ("remove_category", args);
        }
    
        /**
         * Check whether %item belongs to a certain category.
         * @param category the category to test against.
         * @return \b true if %item belongs to given category, \b false otherwise.
         */
        bool is_a (const string & category)
        {
            int result = 0;
            PyObject *args = Py_BuildValue ("(s)", (char *) category.c_str ());
            PyObject *retval = call_method_ret ("is_a", args);
            
            if (retval && PyInt_Check (retval)) result = PyInt_AS_LONG (retval);
            Py_XDECREF (retval);
            
            return (result != 0);
        }
    
        /**
         * Retrieve a list of categories this %item belongs to.
         * @return List of %item categories.
         */
        std::vector<string> categories () const;
        
        /**
         * Get the charge the %item has left.
         * @return number of charges the %item has.
         */
        u_int16 charge ()
        {
            return (u_int16) get_attribute_int ("Charge");
        }
    
        /**
         * Set the number of charges the %item may hold.
         * @param charge items number of charges.
         */
        void set_charge (u_int16 charge)  
        {
            set_attribute_int ("Charge", charge); 
        }
        
        /**
         * Get the maximum number of charges the %item may hold.
         * @return items maximum number of charges.
         */
        u_int16 max_charge ()
        {
            return (u_int16) get_attribute_int ("MaxCharge");
        }
    
        /**
         * Set the maximum number of charges the %item may hold.
         * @param max_charge items maximum number of charges.
         */
        void set_max_charge (u_int16 max_charge)
        { 
            set_attribute_int ("MaxCharge", max_charge); 
        }
    
        /**
         * Check whether the %item is mutable or immutable. Mutable
         * items are those that may change their attributes during
         * livetime. Therefore they need to be treated different
         * from items that do never change as long as they live.
         * @return \b true if the item is mutable, \b false otherwise.
         */
        bool is_mutable () const
        {
            return Mutable;
        }
        
        /**
         * If the item is kept in an inventory, it will be assigned
         * to a certain slot. For mutable items, the assignment will be
         * pertinent. For immutable items however, it will be only temporary
         * and only valid for the %item that is currently manipulated. In
         * reality, no difference should be notable though.
         * @return slot holding the %item or \c NULL, if it isn't kept in
         *      an inventory.
         */
        slot *get_slot () const
        {
            return Slot;
        }
        
        /**
         * Set the slot this (stack of) item(s) is kept in.
         * @param s the slot to assign to the item(s).
         */
        void set_slot (slot *s)
        {
            Slot = s;
            if (Next != NULL) Next->set_slot (s);
        }
        
        /** 
         * Check how many items of this kind may be stacked in one slot.
         * @return maximum size of a stack of this kind of %item.
         */    
        u_int32 max_stack () const
        {
            return MaxStack;
        }
        
        /** 
         * Set how many items of this kind may be stacked in one slot.
         * @param max_stack maximum size of a stack of this kind of %item.
         */    
        void set_max_stack (u_int32 max_stack)
        {
            MaxStack = max_stack;
        }
        //@}
        
        /**
         * @name Loading/Saving
         */
        //@{
        /**
         * Load item from named file. This will first load the %item template
         * to instanciate the underlying Python item class. Then it will
         * restore the actual %item data. If an item is already instanciated,
         * it will be replaced.
         *
         * @param file name of the file to load %item from.
         * @return \b true if loading successful, \b false otherwise.
         */
        bool get_state (const string & file);
        
        /**
         * Save %item to named file. This will save both the item template
         * plus the actual data to the given file. The file will be replaced
         * if it already exists.
         *
         * @param file name of the file to save %item to.
         * @return \b true if saving successful, \b false otherwise.
         */
        bool put_state (const string & file) const;
            
        /**
         * Load %item from stream. This will first load the %item template
         * to instanciate the underlying Python item class. Then it will
         * restore the actual %item data. If an item is already instanciated,
         * it will be replaced.
         *
         * @param file stream to load item from.
         * @return \b true if loading successful, \b false otherwise. 
         */
        bool get_state (base::flat & file);
        
        /**
         * Save %item to stream. This will save both the %item template
         * plus the actual data to the given file. The file will be replaced
         * if it already exists.
         *
         * @param file stream to save %item to.
         * @return \b true if saving successful, \b false otherwise.
         */
        bool put_state (base::flat & file) const;
        //@}
    
        /**
         * Chack whether two items are of the same kind. For mutable items this is
         * the case if they have the same name. For immutable items, they need
         * to be the same instances.
         * @param itm the %item to compare this one to.
         * @return \b true if items are (roughly) equal, \b false otherwise.
         */
        bool equals (item *itm);
                
    protected:
        /**
         * The number of items that can be stacked
         */
        u_int32 MaxStack;
                
        /**
         * The slot this item is kept in.
         */
        slot *Slot;
    
        /**
         * Whether the item is mutable or immutable
         */
        bool Mutable;
        
        /**
         * If we're dealing with a stack of mutable items, Next will point
         * to the next item in the stack.
         */
        item *Next;
        
        /**
         * @name Item Stack handling
         */
        //@{
        friend u_int32 slot::add (item * itm, const u_int32 & count = 1);
        friend u_int32 slot::remove (item * itm, const u_int32 & count = 1);
    
        /**
         * Add an %item (or stack of items) to this item's stack. We assume
         * that checks for item equality and number of items on the stack
         * have been made already. That's why this method is only available 
         * to friends of %item_base. In fact, it should only be used by
         * slot::add_item and slot::remove_item.
         * @param stack Item (stack) to be added to this item. It will be the
         *      new top of stack.
         */
        void stack (item *stack);
        
        /**
         * Retrieve number items from this stack. This method will return
         * the topmost item(s) of the stack. Like item::stack, this
         * method is only available to the slot class.
         * @param number Number of items to retrieve from the stack.
         * @return the item(s) remaining in this stack.
         */
        item *split (u_int32 number = 1);
        //@}
    };
}
#endif // RPG_ITEM_H
