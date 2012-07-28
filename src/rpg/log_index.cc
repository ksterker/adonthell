/*
   $Id: log_index.cc,v 1.3 2004/08/23 06:33:47 ksterker Exp $
   
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
 * @file   log_index.cc
 * @author Kai Sterker <kaisterker@linuxgames.com> 
 * 
 * @brief  Index to log entries.
 */

#include <set>
#include <iostream>
#include "log_index.h"

using rpg::log_index_entry;
using rpg::log_index;
using rpg::log_entry;

// ctor
log_index_entry::log_index_entry (const std::string & key, const u_int32 & pos)
{
    Key = key;
    Parts = 0;
    Pos = pos;
    
    // multi-part key?
    for (u_int32 i = 0; i < key.length (); i++)
    {
        if (!isalpha (key[i]) && key[i] != '\'' && key[i] != '-')
        {
            Parts = i;
            break;
        }
    }
}

// index given log entry
void log_index::add (const log_entry *entry)
{
    std::map<u_int16, std::set<log_index_entry*, ltlie> > words;
    const std::string & text = entry->text ();
    u_int32 i, start = 0;
    u_int16 first;
    
    // split text into its words
    for (i = 0; i < text.length (); i++)
    {
        // skip over word
        if (isalpha (text[i]) || text[i] == '\'' || text[i] == '-') continue;
        
        // reached end of word
        first = toupper (text[start]);
        
        // don't care if word is shorter than shortest index entry (or no entry begins with that letter)
        if (MinLengths.find (first) != MinLengths.end () && (i - start) >= MinLengths[first])
        {
            // otherwise add it to wordlist
            log_index_entry *e = new log_index_entry (text.substr (start, i - start), start);
            std::pair<std::set<log_index_entry*, ltlie>::iterator, bool> result = words[first].insert (e);
            if (result.second == false) delete e;
        }
        
        // search for beginning of next word
        while (i < text.length () && !isalpha (text[i])) i++; 
        
        // remember beginning of word
        start = i;
    }
    
    // last word (if text ends with alpha)
    if (start != i)
    {
        first = toupper (text[start]);
        
        if (MinLengths.find (first) != MinLengths.end () && (i - start) >= MinLengths[first])
        {
            // otherwise add it to wordlist
            log_index_entry *e = new log_index_entry (text.substr (start, i - start), start);
            std::pair<std::set<log_index_entry*, ltlie>::iterator, bool> result = words[first].insert (e);
            if (result.second == false) delete e;
        }
    }
    
    std::set<log_index_entry*, ltlie>::iterator w;
    std::map<u_int16, std::set<log_index_entry*, ltlie> >::iterator wl;

    // check whether words are keys in our index
    for (wl = words.begin (); wl != words.end (); wl++)
    {
        for (w = (*wl).second.begin (); w != (*wl).second.end (); w++)
        {
            log_index_entries::iterator e;

            // if word is in index, add entry's uid to according index entry
            if ((e = Entries[(*wl).first].find (*w)) != Entries[(*wl).first].end ())
            {
                // for multi-part keys, check that text contains the other parts
                if ((*e)->Parts > 0)
                {
                    if (text.find ((*e)->key (), (*w)->Pos) == text.npos)
                    {
                        delete *w;
                        continue;
                    }
                }
                
                // did index change?
                if ((*e)->empty ()) Changed = true;
                (*e)->Uids.push_back (entry->uid ());
            }
            
            // cleanup
            delete *w;
        }
    }
}

// add key to index
void log_index::add_key (const std::string & key)
{
    // valid key?
    if (key.length () == 0) return;
    
    u_int16 idx = toupper (key[0]);
    log_index_entry* entry = new log_index_entry (key);
    Entries[idx].insert (entry);

    // update minimum key length
    if (Entries[idx].size () == 1 || MinLengths[idx] > entry->length ())
        MinLengths[idx] = entry->length ();
}

// return list of keys with index entry
std::vector<const char*> log_index::keys ()
{
    // if index hasn't changed, return precalculated list
    if (!Changed) return Keys;
    
    // otherwise re-calculate
    Keys.clear ();
    
    std::set<log_index_entry*, ltlie> tmp;

    log_index_entries::const_iterator e;
    std::map<u_int16, log_index_entries>::const_iterator i;
    std::set<log_index_entry*, ltlie>::const_iterator t;
    
    for (i = Entries.begin (); i != Entries.end (); i++)
    {
        // sort ...
        for (e = (*i).second.begin (); e != (*i).second.end (); e++)
            if (!(*e)->empty ()) tmp.insert (*e);

        for (t = tmp.begin (); t != tmp.end (); t++)
            Keys.push_back ((*t)->key ().c_str ());
            
        tmp.clear ();
    }
    
    Changed = false;
    
    return Keys;
}

// return entries indexed by given key
std::vector<const char*> log_index::refs (const std::string & key) const
{
    std::vector<const char*> refs;

    // do we have a valid key?
    if (key.length () > 0)
    {
        std::map<u_int16, log_index_entries>::const_iterator i;
        std::vector<std::string>::const_iterator k;
        log_index_entries::const_iterator j;
        log_index_entry entry (key);
                
        if ((i = Entries.find (toupper (key[0]))) != Entries.end ())
            if ((j = (*i).second.find (&entry)) != (*i).second.end ())
                for (k = (*j)->Uids.begin (); k != (*j)->Uids.end (); k++)
                    refs.push_back ((*k).c_str ());
    }
    
    return refs;
}
