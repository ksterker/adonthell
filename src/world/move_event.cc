/*
   Copyright (C) 2012 Kai Sterker <kai.sterker@gmail.com>
   Part of the Adonthell Project http://adonthell.nongnu.org

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
 * @file 	world/move_event.cc
 *
 * @author 	Kai Sterker
 * @brief 	Declares the move_event class.
 */
 
#include "base/logging.h"
#include "world/move_event.h"
#include "world/area_manager.h"

using world::move_event;

// constructor
move_event::move_event (const world::area *map, const std::string & actor, const std::string & check_enter, const std::string & check_leave)
{
    Actor = (world::moving *) map->get_entity(actor);
    CheckEnter = map->get_zone(check_enter);
    CheckLeave = map->get_zone(check_leave);

    if (check_leave != check_enter && check_leave != "")
    {
        LOG(WARNING) << "move_event: ignoring leave check (must be same as enter or \"\")";
        CheckLeave = NULL;
    }
}

// test two move events for equality
bool move_event::equals (const events::event * e) const
{
    const move_event *evt = (const move_event*) e;

    // actor && (enter || leave)
    if (Actor != evt->actor()) return false;

    bool equals = true;

    if (CheckEnter != NULL)
    {
        if (!CheckEnter->contains (evt->start()) && CheckEnter->contains(evt->end())) equals = true;
        else equals = false;
    }

    if (CheckLeave != NULL)
    {
        if (CheckLeave->contains (evt->start()) && !CheckLeave->contains(evt->end())) equals = true;
        else equals = false;
    }

    return equals;
}

// save move event
void move_event::put_state (base::flat& file) const
{
    // save basic event data first
    event::put_state (file);

    // save move event data
    file.put_string ("mva", Actor ? Actor->uid() : "");
    file.put_string ("mve", CheckEnter ? CheckEnter->name() : "");
    file.put_string ("mvl", CheckLeave ? CheckLeave->name() : "");
}

// load move event
bool move_event::get_state (base::flat& file)
{
    // get basic event data
    if (event::get_state (file))
    {   
        const world::area *map = world::area_manager::get_map();

        // get move event data
        std::string actor = file.get_string("mva");
        if (actor != "") Actor = (world::moving*) map->get_entity(actor);

        std::string check_enter = file.get_string("mve");
        if (check_enter != "") CheckEnter = map->get_zone(check_enter);

        std::string check_leave = file.get_string("mvl");
        if (check_leave != "") CheckLeave = map->get_zone(check_leave);
    }

    return file.success ();
}

