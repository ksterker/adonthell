/*
 $Id: placeable_model.cc,v 1.4 2007/06/10 03:58:22 ksterker Exp $
 
 Copyright (C) 2002 Alexandre Courbot <alexandrecourbot@linuxgames.com>
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
 * @file   world/placeable_model.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the placeable_model class.
 * 
 * 
 */


#include "world/placeable_model.h"

using namespace world;

placeable_model::placeable_model()
{
    Current_state = States.begin (); 
}

placeable_area * placeable_model::current_state()
{
    if (Current_state != States.end ())
        return &(Current_state->second);
    else return NULL; 
}

placeable_area * placeable_model::get_model_state (const std::string & name) 
{
    std::map <std::string, placeable_area>::iterator State;
    State = States.find (name); 
    if (State == States.end())
        return NULL;
    else return &(State->second);
}

const std::string placeable_model::current_state_name() const
{
    if (Current_state != States.end ())
        return Current_state->first;
    else return std::string (); 
}

placeable_area * placeable_model::add_state (const std::string & name) 
{
    return &((States.insert(std::pair<const std::string, const placeable_area> (name, placeable_area()))).first->second);
}

bool placeable_model::del_state (const std::string & name)
{
    return States.erase(name);
}

void placeable_model::set_state (const std::string & name) 
{    
    if (Current_state != States.end() && Current_state->first == name)
        return;
        
    std::map <std::string, placeable_area>::iterator Previous_state;
    Previous_state = Current_state;
    Current_state = States.find (name); 
    if (Current_state == States.end())
        Current_state = Previous_state;
    else State_changed = true;
}

// save state to stream
bool placeable_model::put_state (base::flat & file) const
{
    base::flat record;
    
    record.put_string ("state", current_state_name ());
    for (std::map <std::string, placeable_area>::iterator i = States.begin();
         i != States.end(); i++)
    {
        base::flat area;
        
        // save each area with its own name
        i->second.put_state (area);
        record.put_flat (i->first, area);
    }
    
    file.put_flat ("model", record);
    return true;
}

// load state from stream
bool placeable_model::get_state (base::flat & file)
{
    base::flat record = file.get_flat ("model");
    if (!file.success ()) return false;

    char *name;
    void *value;
    u_int32 size;
    
    std::string state = record.get_string ("state");
    while (record.next (&value, &size, &name) == base::flat::T_FLAT) 
    {
        printf ("*** loading placeable_area %s ...", name);
        base::flat area ((const char*) value, size);
        placeable_area * mpa = add_state (std::string (name));
        mpa->get_state (area);
        printf (" done\n");
        fflush (stdout);
    }
    
    set_state (state);
    
    return record.success ();
}
