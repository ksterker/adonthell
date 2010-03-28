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

    std::vector<world::placeable_model*>::iterator i = Model.begin();
    State = (*i)->set_shape (state);

    // reset current size and position
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
        (*i)->set_shape (State);
        const placeable_shape *shape = (*i)->current_shape ();
        if (shape != NULL)
        {
            if (shape->is_solid())
            {
                vector3<s_int16> solidMaxPos = SolidCurPos + SolidCurSize;
                solidMaxPos.set_x (std::max (solidMaxPos.x(), (s_int16) (shape->length() + shape->x())));
                solidMaxPos.set_y (std::max (solidMaxPos.y(), (s_int16) (shape->width()  + shape->y())));
                solidMaxPos.set_z (std::max (solidMaxPos.z(), (s_int16) (shape->height() + shape->z())));

                SolidCurPos.set_x (std::min (SolidCurPos.x(), shape->x()));
                SolidCurPos.set_y (std::min (SolidCurPos.y(), shape->y()));
                SolidCurPos.set_z (std::min (SolidCurPos.z(), shape->z()));

                SolidCurSize = solidMaxPos - SolidCurPos;

                Solid = true;
            }

            vector3<s_int16> entireMaxPos = EntireCurPos + EntireCurSize;
            entireMaxPos.set_x (std::max (entireMaxPos.x(), (s_int16) (shape->length() + shape->x())));
            entireMaxPos.set_y (std::max (entireMaxPos.y(), (s_int16) (shape->width()  + shape->y())));
            entireMaxPos.set_z (std::max (entireMaxPos.z(), (s_int16) (shape->height() + shape->z())));

            EntireCurPos.set_x (std::min (EntireCurPos.x(), shape->x()));
            EntireCurPos.set_y (std::min (EntireCurPos.y(), shape->y()));
            EntireCurPos.set_z (std::min (EntireCurPos.z(), shape->z()));

            EntireCurSize = entireMaxPos - EntireCurPos;
        }
    }
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
            vector3<s_int16> solidMaxPos = SolidMinPos + SolidMaxSize;
            solidMaxPos.set_x (std::max (solidMaxPos.x(), (s_int16) (shape->second.length() + shape->second.x())));
            solidMaxPos.set_y (std::max (solidMaxPos.y(), (s_int16) (shape->second.width()  + shape->second.y())));
            solidMaxPos.set_z (std::max (solidMaxPos.z(), (s_int16) (shape->second.height() + shape->second.z())));

            SolidMinPos.set_x (std::min (SolidMinPos.x(), shape->second.x()));
            SolidMinPos.set_y (std::min (SolidMinPos.y(), shape->second.y()));
            SolidMinPos.set_z (std::min (SolidMinPos.z(), shape->second.z()));

            SolidMaxSize = solidMaxPos - SolidMinPos;
        }

        // add regardless of being solid or not
        vector3<s_int16> entireMaxPos = EntireMinPos + EntireMaxSize;
        entireMaxPos.set_x (std::max (entireMaxPos.x(), (s_int16) (shape->second.length() + shape->second.x())));
        entireMaxPos.set_y (std::max (entireMaxPos.y(), (s_int16) (shape->second.width()  + shape->second.y())));
        entireMaxPos.set_z (std::max (entireMaxPos.z(), (s_int16) (shape->second.height() + shape->second.z())));

        EntireMinPos.set_x (std::min (EntireMinPos.x(), shape->second.x()));
        EntireMinPos.set_y (std::min (EntireMinPos.y(), shape->second.y()));
        EntireMinPos.set_z (std::min (EntireMinPos.z(), shape->second.z()));

        EntireMaxSize = entireMaxPos - EntireMinPos;
    }
}

// save to stream
bool placeable::put_state (base::flat & file) const
{
    file.put_string ("state", State);
    
    return true;
}

// load from stream
bool placeable::get_state (base::flat & file)
{    
    // load current state
    set_state (file.get_string ("state"));
    return file.success ();
}

// save placeable model name
bool placeable::save_model (base::flat & file) const
{
    file.put_sint8 ("type", Type);
    file.put_string ("model", ModelFile);
    return true;
}

// load placeable model from file name
bool placeable::load_model (const std::string & filename)
{
    // Type is loaded outside of this class
    ModelFile = filename;
    
    base::diskio static_model;
    if (!static_model.get_record (filename)) return false;
    return load_model(static_model);
}

// load placeable model
bool placeable::load_model (base::flat & model)
{
    char *name;
    void *value;
    u_int32 size;    
    
    // load shapes and sprites
    while (model.next (&value, &size, &name) == base::flat::T_FLAT) 
    {
        base::flat pm ((const char*) value, size);
        placeable_model * mdl = new placeable_model ();
        mdl->get_state (pm);
        add_model (mdl);
    }
    
    std::cout << this << " " << ModelFile << ": " << EntireMaxSize << std::endl;

    return model.success();
}
