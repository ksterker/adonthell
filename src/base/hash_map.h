/*
   $Id: hash_map.h,v 1.1 2004/05/31 11:44:50 ksterker Exp $

   (C) Copyright 2001 Alexandre Courbot <alexandrecourbot@linuxgames.com>
   (C) Copyright 2004 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file   base/hash_map.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Declares the hash<string> type, to be able to declare
 *         hash_maps with strings as keys.
 */ 

#ifndef BASE_HASHMAP_H
#define BASE_HASHMAP_H
 
#if __GNUG__ > 2
#include <ext/hash_map>
#else
#include <hash_map>
#endif
#include <string>

#if __GNUG__ > 2
namespace __gnu_cxx
#else
namespace std
#endif
{
    /**
     * Hash function for strings.
     */
    template<> struct hash<std::string>
    {
        size_t operator() (const std::string & __s) const 
        { 
            return __stl_hash_string (__s.c_str()); 
        }
    };
}

#if __GNUG__ > 2
namespace std { using namespace __gnu_cxx; }
#endif

#endif // BASE_HASHMAP_H
