/*
   $Id: equipment.h,v 1.9 2009/04/08 19:36:02 ksterker Exp $
   
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
 * @file   equipment.h
 * @author Kai Sterker <kaisterker@linuxgames.com> 
 * 
 * @brief  Takes care of character/item issues.
 */

#ifndef RPG_EQUIPMENT_H
#define RPG_EQUIPMENT_H

#include "rpg/item.h"
#include "base/hash_map.h"
#include <set>

namespace rpg
{
    /// comparator for storing strings in a set
    struct lstr {
        /**
         * lexical comparation of strings.
         * @param s1 first string
         * @param s2 second string
         * @return whether s1 comes before s2 or not
         */
        bool operator() (const std::string & s1, const std::string & s2) const {
            return s1 < s2;
        }
    };

    /// abbrevation for a set of strings
    typedef std::set<std::string, lstr> slot_list;

    /**
     * Equipment slot properties.
     */
    class slot_definition
    {
    public:
        /**
         * Create new equipment slot definition.
         * @param name unique name for the %slot
         * @param categories %item categories that may go into this %slot.
         * @param modifier stat modifier for the %item contained in this %slot
         */
        slot_definition (const std::string & name, const std::vector<std::string> & categories, const double & modifier);

        /**
         * Check whether the given item fits into this slot.
         * @param itm item that should go into this slot.
         * @return true if item fits, false otherwise
         */
        bool fits (rpg::item *itm) const;
        
        /**
         * @name member access
         */
        //@{
        /**
         * Get %slot name.
         * @return %slot name. 
         */
        std::string name () const { return Name; }
        
        /**
         * Get %slot modifier.
         * @return %slot modifier.
         */
        double modifier () const { return Modifier; }
        //@}
        
        /**
         * @name loading/saving
         */
        //@{
        /**
         * Save equipment %slot definition to stream.
         * @param file stream to save %equipment %slot to.
         */
        void put_state (base::flat & file) const;
        
        /**
         * Load equipment %slot definition from stream. 
         * @param file stream to load %equipment %slot from.
         * @return \b true if loading successful, \b false otherwise.
         */
        bool get_state (base::flat & file);
        //@}
        
    private:
        /// slot name
        std::string Name;
        /// %item categories that fit into slots with this name
        std::vector<std::string> Categories;
        /// modifier
        double Modifier;
    };
    
    /**
     * The %equipment class can be used to define what items may go into
     * a certain slot, representing a character's equipped items. This is
     * done by specifiying a mapping between a slot name and item category.
     * If, for example a mapping of slot 'Quiver' and item category 
     * 'Projectile' exists, the equip() method will be able to put any 
     * type projectile into the quiver but not into any other slot.
     * 
     * The methods provided here are used from the character class to
     * handle the equipping / unequipping of items. 
     */
    class equipment
    {
    public:
        /**
         * @name Equipment handling methods
         */
        //@{
        /**
         * Return list of %equipment slots this %item fits into.
         * @param %itm The %item whose slots to retrieve.
         * @returns List of slot ids.
         */
        const slot_list available_slots (item *itm) const;
        
        /**
         * Check whether a given %item can be equipped in the given %slot.
         * @param target An %equipment %slot.
         * @param itm An %item 
         * @return \b true if %item can be equipped, \b false otherwise.
         */
        bool accepts (slot* target, item* itm) const;
    
        /**
         * Actually (try to) equip the given %item in the given %slot. 
         * Replaces the %item currently in the %slot.
         * @param target An %equipment %slot.
         * @param itm The %item to equip.
         * @param count Number of %items to add to the %slot.
         * @return 
         *     - \c NULL if the %slot was empty and the %item could be equipped.
         *     - The previously equipped item if the %item could be equipped.
         *       \c count will contain the number of items that were equipped.
         *     - The given %item if equipping wasn't possible.
         */    
        item* equip (slot *target, item* itm, u_int32 *count);
        
        /**
         * Remove the %item from the given %slot.
         * @param target An equipment %slot.
         * @param count Number of items to remove from the %slot.
         * @return The %item from the %slot or \c NULL if the %slot empty.
         *     \c count will contain the number of items actually removed.
         */
        item* unequip (slot *target, u_int32 *count);
        //@}
        
        /**
         * @name Equipment management methods
         */
        //@{
        /**
         * Remove equipment setup previously created with define_slot and define_set.
         * @see define_slot
         * @see define_set
         */
        static void cleanup ();
        
        /**
         * Define a %equipment %slot with name, a list of item typest that may go  
         * into that slot and an optional armour modifier.
         * @param name unique name for the %slot
         * @param categories %item categories that may go into this %slot.
         * @param modifier stat modifier for the %item contained in this %slot
         */
        static void define_slot (const std::string & name, const std::vector<std::string> & categories, const double & modifier);
        
        /**
         * Define a %equipment set, i.e. a list of equipment slots available to a
         * certain type of character or faction. This list of slots is used to return an
         * inventory to hold the equipped items.
         * @param character type or faction
         * @param slots list of slot names, that should have been defined with the define_slot method
         * @see define_slot
         */
        static void define_set (const std::string & name, const std::vector<std::string> & slots);

        /**
         * Get stat modifier for the given slot set.
         * @param name slot name
         * @return modifier of given slot, or 0.0 on error
         */
        static double get_modifier (const std::string & name);
        
        /**
         * Get accumulated stat modifier for a given %equipment set.
         * @param name set name
         * @return accumulated modifier for %equipment set, or 0.0 on error
         */
        static double get_set_modifier (const std::string & name);
        
        /**
         * A factory method to create a new, empty equipment storage for the given type
         * of character (i.e. an inventory with the named slots for the given type).
         * @param type of character
         * @return new %inventory or \b NULL, if the type is unknown.
         */
        static inventory* create_inventory (const std::string & type);
        //@}
        
        /**
         * @name Loading/Saving
         */
        //@{
        /**
         * Save %equipment definitions to stream.
         * @param file stream to save %equipment to.
         */
        static void put_state (base::flat & file);

        /**
         * Load %equipment definitions from stream. 
         * @param file stream to load %equipment from.
         * @return \b true if loading successful, \b false otherwise.
         */
        static bool get_state (base::flat & file);
        //@}
        
    private:
        /// storage for item-category/equipment-slot mappings
        static std::hash_map<std::string, rpg::slot_definition*, std::hash<std::string> > DefinedSlots;

        /// storage for character-based equipment sets
        static std::hash_map<std::string, std::vector<std::string>, std::hash<std::string> > EquipmentSets;
    };
}
#endif // RPG_EQUIPMENT_H
