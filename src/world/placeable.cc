/*
 $Id: placeable.cc,v 1.10 2009/04/26 18:52:59 ksterker Exp $
 
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
 * @file   world/placeable.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the placeable class.
 * 
 * 
 */

#include <algorithm>

#include "placeable.h"
#include "world/area.h"

using world::placeable;

#define MI16 32767

// ctor
placeable::placeable(world::area & mymap) : MaxSize(), Mymap(mymap)
{
    MinPos.set (MI16, MI16, MI16);
    Type = UNKNOWN;
    State = "";
}

// dtor
placeable::~placeable()
{
    for (std::vector<world::placeable_model*>::iterator i = Model.begin(); i != Model.end(); i++)
    {
        delete *i;
    }
    Model.clear();
}

// get unique id of placeable
const std::string & placeable::uid () const
{
    static std::string NO_ID ("");
    const std::string *uid = Mymap.get_entity_name (this);
    if (uid != NULL) return *uid;
    return NO_ID;
}

// change state of placeable
void placeable::set_state (const std::string & state)
{
    // reset current size and position
    CurSize.set (0, 0, 0);
    CurPos.set (MI16, MI16, MI16);
    
    // update shape and size
    std::vector<world::placeable_model*>::iterator i;
    for (i = Model.begin(); i != Model.end(); i++)
    {
        (*i)->set_shape (state);
        const placeable_shape *shape = (*i)->current_shape ();
        if (shape != NULL)
        {
            CurSize.set_x (std::max (CurSize.x(), shape->length()));
            CurSize.set_y (std::max (CurSize.y(), shape->width()));
            CurSize.set_z (std::max (CurSize.z(), shape->height()));
            
            CurPos.set_x (std::min (CurPos.x(), shape->x()));
            CurPos.set_y (std::min (CurPos.y(), shape->y()));
            CurPos.set_z (std::min (CurPos.z(), shape->z()));            
        }
    }
    
    State = state;
}

// add model to placeable
void placeable::add_model (world::placeable_model * model)
{
    Model.push_back (model);
    
    // update bounding box, using the extend of the largest shape
    for (placeable_model::iterator shape = model->begin(); shape != model->end(); shape++)
    {
        MaxSize.set_x (std::max (MaxSize.x(), shape->second.length()));
        MaxSize.set_y (std::max (MaxSize.y(), shape->second.width()));
        MaxSize.set_z (std::max (MaxSize.z(), shape->second.height()));
        
        MinPos.set_x (std::min (MinPos.x(), shape->second.x()));
        MinPos.set_y (std::min (MinPos.y(), shape->second.y()));
        MinPos.set_z (std::min (MinPos.z(), shape->second.z()));        
    }
}

// save to stream
bool placeable::put_state (base::flat & file) const
{
    base::flat entity;

    entity.put_string ("state", State);
    for (iterator i = Model.begin(); i != Model.end(); i++)
    {
        (*i)->put_state (entity);
    }
    file.put_flat ("entity", entity);

    return true;
}

// load from stream
bool placeable::get_state (base::flat & file)
{
    std::string state = file.get_string ("state");
    
    char *name;
    void *value;
    u_int32 size;
    
    // load actual models
    while (file.next (&value, &size, &name) == base::flat::T_FLAT) 
    {
        placeable_model * model = new placeable_model ();
        model->get_state (file);
        add_model (model);
    }    
    
    set_state (state);
    return file.success ();
}
