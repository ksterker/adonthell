/*
   $Id: log_entry.cc,v 1.4 2004/12/07 16:46:27 ksterker Exp $
   
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
 * @file   log_entry.cc
 * @author Kai Sterker <kaisterker@linuxgames.com> 
 * 
 * @brief  Entry .
 */

#include "rpg/log_entry.h"
#include "event/date.h"

using rpg::log_entry;
using std::string;

// create new log entry
log_entry::log_entry (const string & topic, const string & text, const string & uid)
{
    Timestamp = events::date::time ();
    Topic = topic;
    Text = text;
    Uid = uid;
}

// save log entry
void log_entry::put_state (base::flat &out) const
{
    base::flat record;
    
    record.put_uint32 ("let", Timestamp);
    record.put_string ("ltc", Topic);
    record.put_string ("ltx", Text);
    record.put_string ("lid", Uid);
    
    out.put_flat ("le", record);
}

// load log entry
bool log_entry::get_state (base::flat &in)
{
    base::flat record = in.get_flat ("le");
    if (!in.success ()) return false;
    
    Timestamp = record.get_uint32 ("let");
    Topic = record.get_string ("ltc");
    Text = record.get_string ("ltx");
    Uid = record.get_string ("lid");
    
    return record.success ();
}
