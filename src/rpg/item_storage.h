/*
   $Id: item_storage.h,v 1.2 2004/06/27 11:20:59 ksterker Exp $
   
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
 * @file   rpg/item_storage.h
 * @author Kai Sterker <kaisterker@linuxgames.com> 
 * 
 * @brief  Global storage for immutable items.
 */

#ifndef RPG_ITEM_STORAGE_H
#define RPG_ITEM_STORAGE_H

#include "rpg/item.h"
#include "base/hash_map.h"

using std::string;

namespace rpg
{
    /**
     * For efficiency reasons, immutable items are only instanciated once
     * throughout a game. In order to access them, they are kept in a globally
     * available %storage.
     */
    class item_storage
    {
    public:
        /**
         * Initialization at engine startup.
         */
        static void init () {}
    
        /**
         * Remove all items from the %storage. This should be done whenever
         * a %game is unloaded.
         */
        static void cleanup ();
    
        /**
         * @name Item handling
         */
        //@{
        /**
         * Add an %item to the %storage. The %item needs to be immutable,
         * otherwise it won't be added. It should also have a unique name.
         * @param itm The %item to add to the storage
         * @return \b true if it was added, \b false otherwise
         */
        static bool add (item *itm);
        
        /**
         * Retrieve immutable %item with given name from the %storage. 
         * Instead of an immutable item's name, one may also pass the file
         * name of a mutable %item. In that case, the mutable %item will be
         * instanciated and returned.
         * @param name name of the %item to retrieve.
         * @return %item with given name, or \c NULL if no such %item exists.
         */
        static item *get (const string & name); 
        //@}
        
    #ifndef SWIG
        /**
         * @name Loading/Saving
         */
        //@{
        /**
         * Load all items kept in the %storage from stream. 
         * @param file stream to load items from.
         * @return \b true if loading successful, \b false otherwise.
         */
        static bool get_state (base::flat & file);
        
        /**
         * Save contents of the %storage to a stream.
         * @param file stream to save %storage to.
         * @return \b true if saving successful, \b false otherwise.
         */
        static bool put_state (base::flat & file);
        //@}
            
    private:
        /// Hashmap for keeping the items
        static std::hash_map<string, item*> Items;
    #endif // SWIG
    };
}

#endif // RPG_ITEM_STORAGE_H
