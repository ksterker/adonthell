/*
 $Id: schedule_data.cc,v 1.1 2009/04/16 21:06:10 ksterker Exp $
 
 Copyright (C) 2004/2005/2006 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file world/schedule_data.cc
 *
 * @author Kai Sterker
 * @brief Implements the %schedule data class.
 */

#include "schedule.h"
#include "schedule_data.h"

using world::schedule;
using world::schedule_data;

// activate queued schedule
bool schedule_data::activate (schedule *s)
{
    if (!s->set_schedule (File, Args))
    {
        fprintf (stderr, "*** schedule_data::activate: activation of queued schedule '%s' failed!\n", File.c_str ());
        return false;
    }
    
    if (Time != "") s->set_alarm (Time, Absolute);
    return true;
}

// save to stream
void schedule_data::put_state (base::flat& file) const
{
    file.put_string ("qsf", File);
    python::put_tuple (Args, file);
    file.put_string ("qst", Time);
    file.put_bool ("qsa", Absolute);
}

// load from stream
bool schedule_data::get_state (base::flat & file)
{
    File = file.get_string ("qsf");
    Args = python::get_tuple (file);
    Time = file.get_string ("qst");
    Absolute = file.get_bool ("qsa");
    
    return file.success();
}
