/*
   $Id: item_storage.cc,v 1.2 2004/06/27 11:20:59 ksterker Exp $
   
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
 * @file   rpg/item_storage.cc
 * @author Kai Sterker <kaisterker@linuxgames.com> 
 * 
 * @brief  Global storage for immutable items.
 */

#include "rpg/item_storage.h"

using rpg::item_storage;
using rpg::item;

/**
 * Hashmap for keeping immutable items
 */
std::hash_map<std::string, item*> item_storage::Items;

// delete all items
void item_storage::cleanup ()
{
    std::hash_map<std::string, item*>::iterator i;
    
    // just a precaution
    for (i = Items.begin (); i != Items.end (); i++)
        (*i).second->set_slot (NULL);

    Items.clear ();
}

// add an (immutable) item
bool item_storage::add (item *item)
{
    if (!item || item->is_mutable ()) return false;

    // items are stored by name    
    Items[item->name ()] = item;
    return true;
}

// get an item from storage
item *item_storage::get (const string & name)
{
    std::hash_map<std::string, item*>::iterator i = Items.find (name);
    item *itm;
    
    // no such item
    if (i == Items.end ())
    {
        // assume that it is a mutable item ...
        itm = new item (true);
        
        // ... and return it if that is the case
        if (itm->get_state (name) && itm->is_mutable ()) return itm;
        
        fprintf (stderr, "*** error: item_storage::get: no item called '%s'\n", name.c_str ());
        
        delete itm;
        return NULL;
    }
    
    // retrieve item
    itm = (*i).second;
    
    // make sure that items from the storage have no inventory assigned
    itm->set_slot (NULL);
    
    return itm;
}

// save storage to disk
bool item_storage::put_state (base::flat & file)
{
    std::hash_map<std::string, item*>::iterator i;
    
    // iterate over hash_map and save all the items
    for (i = Items.begin (); i != Items.end (); i++)
    {
        file.put_bool ("imi", true);
        (*i).second->put_state (file);    
    }
    
    // no more items follow
    file.put_bool ("imi", false);
    
    return true;
}

// load storage from disk
bool item_storage::get_state (base::flat & file)
{
    item *itm;
    
    // check whether more items follow
    while (file.get_bool ("imi") == true)
    {
        // error?
        if (!file.success ()) return false;
    
        // retrieve item from file
        itm = new item (false);
        itm->get_state (file);
        
        // add item to storage
        Items[itm->name ()] = itm;
    }
    
    return file.success ();
}
