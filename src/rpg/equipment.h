/*
   $Id: equipment.h,v 1.1 2004/05/31 11:44:50 ksterker Exp $
   
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
     *
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
    
    private:
        /// storage for item-category/equipment-slot mappings
        static std::hash_map<std::string, slot_list, std::hash<std::string> > SlotCategoryMap;
    };
}
#endif // RPG_EQUIPMENT_H
