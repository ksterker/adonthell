/*
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
placeable::placeable(world::area & mymap) : SolidMaxSize(), Mymap(mymap)
{
    SolidMinPos.set (MI16, MI16, MI16);
    Type = UNKNOWN;
    Solid = true;
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
    // object becomes solid if there is at least one solid component
    Solid = false;

    // reset current size and position
    std::vector<world::placeable_model*>::iterator i = Model.begin();
    (*i)->set_shape (state);
    const placeable_shape *shape = (*i)->current_shape ();
    if (shape != NULL)
    {
        
        if (shape->is_solid())
        {
            SolidCurSize.set (shape->length(), shape->width(), shape->height());
            SolidCurPos.set (shape->x(), shape->y(), shape->z());
            
            Solid = true; // set this object as solid
        }
        
        EntireCurSize.set (shape->length(), shape->width(), shape->height());
        EntireCurPos.set (shape->x(), shape->y(), shape->z());
    }        
    
    // update shape and size of composite placeables
    for (i++; i != Model.end(); i++)
    {
        (*i)->set_shape (state);
        const placeable_shape *shape = (*i)->current_shape ();
        if (shape != NULL)
        {
            if (shape->is_solid())
            {
                SolidCurSize.set_x (std::max (SolidCurSize.x(), shape->length()));
                SolidCurSize.set_y (std::max (SolidCurSize.y(), shape->width()));
                SolidCurSize.set_z (std::max (SolidCurSize.z(), shape->height()));
                
                SolidCurPos.set_x (std::min (SolidCurPos.x(), shape->x()));
                SolidCurPos.set_y (std::min (SolidCurPos.y(), shape->y()));
                SolidCurPos.set_z (std::min (SolidCurPos.z(), shape->z()));
                
                Solid = true;
            }
            
            EntireCurSize.set_x (std::max (EntireCurSize.x(), shape->length()));
            EntireCurSize.set_y (std::max (EntireCurSize.y(), shape->width()));
            EntireCurSize.set_z (std::max (EntireCurSize.z(), shape->height()));
                
            EntireCurPos.set_x (std::min (EntireCurPos.x(), shape->x()));
            EntireCurPos.set_y (std::min (EntireCurPos.y(), shape->y()));
            EntireCurPos.set_z (std::min (EntireCurPos.z(), shape->z()));
        }
    }

    State = state;
}

// get z position of the object's surface
s_int32 placeable::get_surface_pos () const
{    
    return SolidCurSize.z() + SolidCurPos.z();
}

// get terrain type of placeable
const std::string* placeable::get_terrain () const
{
    std::string tmp;
    
    for (std::vector<world::placeable_model*>::const_iterator i = Model.begin(); i != Model.end(); i++)
    {
        tmp = (*i)->terrain();
        if (tmp != "None")
        {
            return &(*i)->terrain();
        }
    }
    
    return NULL;
}

// add model to placeable
void placeable::add_model (world::placeable_model * model)
{
    Model.push_back (model);
    
    // update bounding box, using the extend of the largest shape
    for (placeable_model::iterator shape = model->begin(); shape != model->end(); shape++)
    {
        // only accounts for solid shapes
        if ((*shape).second.is_solid()) {
            SolidMaxSize.set_x (std::max (SolidMaxSize.x(), shape->second.length()));
            SolidMaxSize.set_y (std::max (SolidMaxSize.y(), shape->second.width()));
            SolidMaxSize.set_z (std::max (SolidMaxSize.z(), shape->second.height()));
            
            SolidMinPos.set_x (std::min (SolidMinPos.x(), shape->second.x()));
            SolidMinPos.set_y (std::min (SolidMinPos.y(), shape->second.y()));
            SolidMinPos.set_z (std::min (SolidMinPos.z(), shape->second.z())); 
        
        }
        
        // add regardless of being solid or not
        EntireMaxSize.set_x (std::max (EntireMaxSize.x(), shape->second.length()));
        EntireMaxSize.set_y (std::max (EntireMaxSize.y(), shape->second.width()));
        EntireMaxSize.set_z (std::max (EntireMaxSize.z(), shape->second.height()));
        
        EntireMinPos.set_x (std::min (EntireMinPos.x(), shape->second.x()));
        EntireMinPos.set_y (std::min (EntireMinPos.y(), shape->second.y()));
        EntireMinPos.set_z (std::min (EntireMinPos.z(), shape->second.z())); 
        
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
