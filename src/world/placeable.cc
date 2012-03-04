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

#include "world/placeable.h"
#include "world/placeable_shape.h"
#include "world/area.h"
#include "base/logging.h"

using world::placeable;

/// sort placeable_models according to the z-position of their surface
struct z_order : public std::binary_function<const world::placeable_model*, const world::placeable_model*, bool>
{
    bool operator() (const world::placeable_model* a, const world::placeable_model* b)
    {
        world::placeable_shape *as = a->current_shape();
        world::placeable_shape *bs = b->current_shape();

        //Here we want to sort by the top of each shape.  However, if they have the same top,
        //then we want the one with the highest bottom (which is the smallest height).  The reason
        //for this is we have tiles and walls with the same height, but we want to prefer to draw on
        //the tile if the player is standing on it.
        s_int32 atop = as->get_max().z();
        s_int32 btop = bs->get_max().z();
        return (atop > btop) || (atop == btop && as->z () < bs->z ());
    }
};

// ctor
placeable::placeable(world::area & mymap, const std::string & hash)
: Hash(hash), SolidMaxSize(), Mymap(mymap)
{
    Type = UNKNOWN;
    Solid = true;
    State = "";
    HaveSolid = false;
    HaveEntire = false;
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

    // resort models according to their z order
    std:sort (Model.begin(), Model.end(), z_order());
}

// get z position of the object's surface
s_int32 placeable::get_surface_pos () const
{    
    return SolidCurSize.z() + SolidCurPos.z();
}

// get z position at the given coordinate
s_int32 placeable::get_surface_pos(const s_int32 & x, const s_int32 & y) const
{
    for (std::vector<world::placeable_model*>::const_iterator i = Model.begin(); i != Model.end(); i++)
    {
        const placeable_shape *shape = (*i)->current_shape ();
        if (shape != NULL && shape->is_solid())
        {
            if (x >= shape->x() && x <= shape->length() + shape->x() &&
                y >= shape->y() && y <= shape->width() + shape->y())
            {
                return shape->height() + shape->z();
            }
        }
    }
    
    // Fallback. We might end up here if the coordinates fall
    // onto a non-solid part of the placeable. We should return
    // a value indicating failure in that case (e.g. MAX_INT)
    // and the caller would have to probe another placeable
    // in turn. But we can probably live with this minor
    // inaccuracy for now.
    return get_surface_pos();
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
            if(!HaveSolid) {
                HaveSolid = true;
                SolidMaxSize.set_x (shape->second.length());
                SolidMaxSize.set_y (shape->second.width());
                SolidMaxSize.set_z (shape->second.height());

                SolidMinPos.set_x (shape->second.x());
                SolidMinPos.set_y (shape->second.y());
                SolidMinPos.set_z (shape->second.z());
            }
            else {
                vector3<s_int16> solidMaxPos = SolidMinPos + SolidMaxSize;
                solidMaxPos.set_x (std::max (solidMaxPos.x(), (s_int16) (shape->second.length() + shape->second.x())));
                solidMaxPos.set_y (std::max (solidMaxPos.y(), (s_int16) (shape->second.width()  + shape->second.y())));
                solidMaxPos.set_z (std::max (solidMaxPos.z(), (s_int16) (shape->second.height() + shape->second.z())));

                SolidMinPos.set_x (std::min (SolidMinPos.x(), shape->second.x()));
                SolidMinPos.set_y (std::min (SolidMinPos.y(), shape->second.y()));
                SolidMinPos.set_z (std::min (SolidMinPos.z(), shape->second.z()));

                SolidMaxSize = solidMaxPos - SolidMinPos;
            }
        }

        // add regardless of being solid or not
        if(!HaveEntire) {
            HaveEntire = true;
            EntireMaxSize.set_x (shape->second.length());
            EntireMaxSize.set_y (shape->second.width());
            EntireMaxSize.set_z (shape->second.height());

            EntireMinPos.set_x (shape->second.x());
            EntireMinPos.set_y (shape->second.y());
            EntireMinPos.set_z (shape->second.z());
        }
        else {
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

// save static model data to file
bool placeable::save_model (const std::string & filename) const
{
    base::diskio static_model;

    for (std::vector<world::placeable_model*>::const_iterator i = Model.begin(); i != Model.end(); i++)
    {
        (*i)->put_state(static_model);
    }

    return static_model.put_record(filename);
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
    
    VLOG(1) << ModelFile << ": " << EntireMaxSize << std::endl;

    return model.success();
}
