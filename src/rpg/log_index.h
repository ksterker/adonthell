/*
   $Id: log_index.h,v 1.5 2006/10/30 05:55:12 ksterker Exp $
   
   Copyright (C) 2004 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file   log_index.h
 * @author Kai Sterker <kaisterker@linuxgames.com> 
 * 
 * @brief  Index to log entries.
 */

#ifndef RPG_LOGINDEX_H
#define RPG_LOGINDEX_H

#include <vector>
#include <map>

#include <adonthell/base/hash_map.h>
#include "log_entry.h"

namespace rpg
{
#ifndef SWIG
    /**
     * This helper class encapsulates the properties of an individual index entry.
     * It stores the key and the log entries referenced by that key.
     */
    class log_index_entry
    {
        public:
            /**
             * Create a new index entry.
             * @param key name of the index entry.
             * @param pos first appearance of key in log entry.
             */
            log_index_entry (const std::string & key, const u_int32 & pos = 0);
        
            /**
             * Return the key of this index entry, i.e. the name to display in the index.
             * @return complete key.
             */
            std::string key () const { return Key; }
        
            /**
             * Return first part of the key.
             * @return
             */
            std::string part () const { return Parts ? Key.substr (0, Parts) : Key; }
            
            /**
             * Return length of index entry
             */
            u_int32 length () const { return Parts ? Parts : Key.length (); }
            
            /**
             * Return whether any log entries are referenced by that index entry.
             * @return \b true if there is at least one reference, \b false otherwise.
             */
            bool empty () const { return Uids.empty (); }
        
        private:
            friend class log_index;
        
            /// Index key
            std::string Key;
            
            /// Multi-part key?
           	u_int16 Parts;
            
            /// First occurance of key in log entry
            u_int32 Pos;
            
            /// Unique IDs of log entries referenced by that keyowrd
            std::vector<std::string> Uids;
    };
}

#ifdef __clang__
#include <functional>
using std::hash;
#else
#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 4
namespace tr1
{
#define hash tr1::hash
}
#else
#if __GNUG__ > 2
namespace __gnu_cxx
#else
namespace std
#endif
{
    /**
     * Hash function for log_index_entries.
     */
    template<> struct hash<rpg::log_index_entry*>
    {
        size_t operator() (const rpg::log_index_entry *e) const 
        { 
            return __stl_hash_string (e->part ().c_str()); 
        }
    };
}
#endif // CLANG
#endif

namespace rpg 
{
    /**
     * Compare two log_index_entries for equality.
     */
    struct eqlie
    {
        bool operator() (const rpg::log_index_entry *e1, const rpg::log_index_entry *e2) const
        {
            return strcmp (e1->part ().c_str (), e2->part ().c_str ()) == 0;
        }
    };

    /**
     * Compare two log_index_entries for lexical order.
     */
    struct ltlie
    {
        bool operator() (const rpg::log_index_entry *e1, const rpg::log_index_entry *e2) const
        {
            return strcmp (e1->key ().c_str (), e2->key ().c_str ()) < 0;
        }
    };

    /**
     * short name for the hashtable where log index entries are kept
     */
    typedef std::hash_set<rpg::log_index_entry*, std::hash<rpg::log_index_entry*>, eqlie> log_index_entries;
#endif // SWIG

    /**
     * Index for log entries. For one it stores a list of index keys, i.e. words which
     * are used to categorize log entries. This is done by searching a log entry for these
     * words. If it contains them, a reference to that log entry is stored under the matching
     * index key.
     *
     * @bug multi word index keys with the same prefix (e.g. 'Mountains of Dawn' and 
     * 'Mountains of Making') can not both be used, as the hash_set used to store index
     * keys is not able to store both (only first word is used as key). 
     *
     * @note To find a key in a log entry, it must contain that key. Otherwise there 
     * will be no match.
     */
    class log_index
    {
        public:
            /**
             * Constructor
             */
            log_index () : Changed (true) { }
            
            /**
             * Return an alphabetical list of keys currently known by the index. Only
             * those keys that actually refer to a log entry are returned. For efficiency,
             * the list is cached and only recalculated if new keys are added to the index.
             * @return list of keys pointing at least to one log entry.
             */
            std::vector<const char*> keys ();
        
            /**
             * Get a list of log entries (their uid) that contain the given key.
             * This is preferably one of the keys returned by the keys () method;
             * other keys won't return any entries.
             * @param key keyword to which returned log entries refer
             * @return list of log entries indexed by key. 
             */
            std::vector<const char*> refs (const std::string & key) const;
            
            /**
             * Index the given log entry. Its text is searched for all keys contained 
             * in the index. Its uid is then added to all index keys that are found in 
             * the text.
             * @param entry a log entry to be indexed.
             */
            void add (const log_entry *entry);
            
            /**
             * Adds a new key to the index. This should happen before any log entries
             * have been indexed, since they will not be re-indexed. 
             * @param key keyword to be added to the index.
             */
            void add_key (const std::string & key);
            
        private:
            /// Index entries sorted by their first letter
            std::map<u_int16, log_index_entries> Entries;
            
            /// Length of the shortest key of each index entries
            std::map<u_int16, u_int16> MinLengths;
            
            /// cache of index keys
            std::vector<const char*> Keys;
            
            /// whether index cache needs to be refreshed
            bool Changed;
    };
}

#endif // RPG_LOGINDEX_H
