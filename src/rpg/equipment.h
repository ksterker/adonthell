/*
   $Id: equipment.h,v 1.3 2004/10/18 07:40:23 ksterker Exp $
   
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
 * @file   equipment.h
 * @author Kai Sterker <kaisterker@linuxgames.com> 
 * 
 * @brief  Takes care of character/item issues.
 */

#ifndef RPG_EQUIPMENT_H
#define RPG_EQUIPMENT_H

#include <set>
#include "base/hash_map.h"
#include "rpg/item.h"

namespace rpg
{
    struct lstr {
        bool operator() (const std::string & s1, const std::string & s2) const {
            return s1 < s2;
        }
    };

    typedef std::set<std::string, lstr> slot_list;

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
         * Return list of %equipment slots this %item fits into.
         * @param %itm The %item whose slots to retrieve.
         * @returns List of slot ids.
         */
        const slot_list fits (item *itm) const;
        
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
         * @return The %item from the %slot or \c NULL if the %slot was empty.
         *     \c count will contain the number of items actually removed.
         */
        item* unequip (slot *target, u_int32 *count);
    
        /** 
         * Tell %equipment that the given %item category may go in the given %slot.
         * @param slot The %slot id.
         * @param category An %item category.
         */
        static void add_mapping (const std::string & slot, const std::string & category);
    
        /**
         * Define a %equipment set, i.e. a list of equipment slots available to a
         * certain type of character. This list of slots is used to return an
         * inventory to hold the equipped items.
         * @param type of character
         * @param slots list of slot names
         */
        static void add_definition (const std::string & type, const std::vector<std::string> & slots);
        
        /**
         * A factory method to create a new, empty equipment storage for the given type
         * of character (i.e. an inventory with the named slots for the given type).
         * @param type of character
         * @return new %inventory or \b NULL, if the type is unknown.
         */
        static inventory* create (const std::string & type);
        
        /**
         * @name Loading/Saving
         */
        //@{
        /**
         * Save %equipment definitions to stream.
         * @param out stream to save %equipment to.
         */
        static void put_state (base::flat & out);

        /**
         * Load %equipment definitions from stream. 
         * @param in stream to load %equipment from.
         * @return \b true if loading successful, \b false otherwise.
         */
        static bool get_state (base::flat & in);
        //@}
        
    private:
        /// storage for item-category/equipment-slot mappings
        static std::hash_map<std::string, slot_list, std::hash<std::string> > SlotCategoryMap;

        /// storage for item-category/equipment-slot mappings
        static std::hash_map<std::string, std::vector<std::string>, std::hash<std::string> > EquipmentDefs;
    };
}
#endif // RPG_EQUIPMENT_H
