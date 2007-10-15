/*
 $Id: placeable_model.cc,v 1.6 2007/10/15 02:19:33 ksterker Exp $
 
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
    CurrentShape = Shapes.begin (); 
}

// get current shape
placeable_shape * placeable_model::current_shape ()
{
    if (CurrentShape != Shapes.end ())
        return &(CurrentShape->second);
    else return NULL; 
}

// get shape by name
placeable_shape * placeable_model::get_shape (const std::string & name) 
{
    std::map <std::string, placeable_shape>::iterator shape;
    shape = Shapes.find (name); 
    if (shape == Shapes.end())
        return NULL;
    else return &(shape->second);
}

// get name of current shape
const std::string placeable_model::current_shape_name() const
{
    if (CurrentShape != Shapes.end ())
        return CurrentShape->first;
    else return std::string (); 
}

// add new shape
placeable_shape * placeable_model::add_shape (const std::string & name) 
{
    return &((Shapes.insert(std::pair<const std::string, const placeable_shape> (name, placeable_shape()))).first->second);
}

// delete given shape
bool placeable_model::del_shape (const std::string & name)
{
    return Shapes.erase(name);
}

// set the current shape
void placeable_model::set_shape (const std::string & name) 
{    
    if (CurrentShape != Shapes.end() && CurrentShape->first == name)
        return;
        
    std::map <std::string, placeable_shape>::iterator prev_shape;
    prev_shape = CurrentShape;
    CurrentShape = Shapes.find (name); 
    if (CurrentShape == Shapes.end())
        CurrentShape = prev_shape;
    
    else StateChanged = true;
}

// save state to stream
bool placeable_model::put_state (base::flat & file) const
{
    base::flat record;
    
    record.put_string ("state", current_shape_name ());
    for (std::map <std::string, placeable_shape>::iterator i = Shapes.begin();
         i != Shapes.end(); i++)
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
    
    // load current shape
    std::string shape = record.get_string ("state");
    
    // load actual shapes
    while (record.next (&value, &size, &name) == base::flat::T_FLAT) 
    {
        base::flat area ((const char*) value, size);
        placeable_shape * mpa = add_shape (std::string (name));
        mpa->get_state (area);
    }
    
    // set current shape
    set_shape (shape);
    
    return record.success ();
}
