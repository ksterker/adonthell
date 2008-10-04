/*
   $Id: quest_event.cc,v 1.4 2008/10/04 16:52:30 ksterker Exp $

   Copyright (C) 2005 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file 	rpg/quest_event.cc
 *
 * @author 	Kai Sterker
 * @brief 	Declares the quest_event class.
 */
 
#include "rpg/quest_event.h"

using rpg::quest_event;

// constructor
quest_event::quest_event (const std::string & pattern, quest_part *part)
{
	set_pattern (pattern);
	Part = part;
}

// test two quest events for equality
bool quest_event::equals (const events::event * e)
{
	quest_event *qevt = (quest_event *) e;
	std::vector<std::string>::const_iterator i = qevt->begin();
	std::vector<std::string>::const_iterator j = Pattern.begin();
	
	for (; j != Pattern.end() && i != qevt->end(); i++, j++)
	{
		// '>' matches rest of pattern
		if ((*i)[0] == '>' || (*j)[0] == '>')
		{
			return true;
		}
		
		// '*' matches one level
		if ((*i)[0] == '*' || (*j)[0] == '*')
		{
			continue;
		}
		
		// otherwise, levels need to be exactly the same
		if ((*i) != (*j))
		{
			return false;
		}
	}
	
	// the patterns matched, but we must still make sure that
	// they have the same size
	return Pattern.size () == qevt->size ();
}

// save quest event
void quest_event::put_state (base::flat& out) const
{
    // save basic event data first
    event::put_state (out);

	string pattern = "";
	for (std::vector<std::string>::const_iterator i = Pattern.begin(); i != Pattern.end(); /* nothing */)
	{
		pattern += (*i); 
		if (++i != Pattern.end()) pattern += ".";
	}
	
    // save quest event data
    out.put_string ("qpt", pattern);
}

// load quest event
bool quest_event::get_state (base::flat& in)
{
    // get basic event data
    if (event::get_state (in))
    {   
        // get quest event data
        string pattern = in.get_string ("qpt");
		set_pattern (pattern);
    }
    return in.success ();
}

// split given path into its parts
void quest_event::set_pattern (const std::string & pattern)
{
	unsigned int idx, pos = 0;

    // split pattern into its levels
    while ((idx = pattern.find (".", pos)) != pattern.npos)
    {
        Pattern.push_back (pattern.substr (pos, idx - pos));
        pos = idx + 1;
    }
    
    // add last part
    Pattern.push_back (pattern.substr (pos));
}

