/*
 $Id: placeable_model.cc,v 1.8 2008/09/14 14:25:26 ksterker Exp $

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


#include "placeable_model.h"

using namespace world;

// ctor
placeable_model::placeable_model()
{
    CurrentShape = Shapes.begin ();
    Terrain = "None";
}

// dtor
placeable_model::~placeable_model()
{
}

// get current shape
placeable_shape * placeable_model::current_shape () const
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
std::string placeable_model::set_shape (const std::string & name)
{
    // shape is already set
    if (CurrentShape != Shapes.end() && CurrentShape->first == name)
        return name;

    // keep track of current shape in case we need to revert
    std::map <std::string, placeable_shape>::iterator prev_shape = CurrentShape;

    if (name == "")
    {
        // set default shape
        CurrentShape = Shapes.begin ();
    }
    else
    {
        // find new shape
        CurrentShape = Shapes.find (name);
    }
    
    if (CurrentShape == Shapes.end())
    {
        // shape not found
        CurrentShape = prev_shape;
    }
    else
    {
        // shape found, update sprite
        if (Sprite.change_animation (name))
        {
            Sprite.play ();
        }
    }
    
    return CurrentShape->first;
}

// set graphical representation of model
void placeable_model::set_sprite (const std::string & name)
{
    Sprite.clear ();
    Sprite.set_filename (name);
}

// get graphical representation of model
gfx::sprite *placeable_model::get_sprite ()
{
    // deferred loading
    if (!Sprite.is_valid())
    {
        Sprite.load ();

        if (CurrentShape != Shapes.end())
        {
            Sprite.change_animation (CurrentShape->first);
            Sprite.play ();
        }
    }

    return &Sprite;
}

// save state to stream
bool placeable_model::put_state (base::flat & file) const
{
    base::flat record;

    for (std::map <std::string, placeable_shape>::iterator i = Shapes.begin();
         i != Shapes.end(); i++)
    {
        base::flat shape;

        // save each shape with its own name
        i->second.put_state (shape);
        record.put_flat (i->first, shape);
    }

    record.put_string ("sprite", Sprite.filename ());
    if (Terrain != "None")
        record.put_string("terrain", Terrain);

    file.put_flat ("model", record);

    return true;
}

// load state from stream
bool placeable_model::get_state (base::flat & file)
{
    char *name;
    void *value;
    u_int32 size;

    // load actual shapes
    while (file.next (&value, &size, &name) == base::flat::T_FLAT)
    {
        base::flat shape ((const char*) value, size);
        placeable_shape * mpa = add_shape (std::string (name));
        mpa->get_state (shape);
    }

    // no shape selected yet
    CurrentShape = Shapes.end();

    // get associated sprite
    std::string sprite = file.get_string ("sprite");
    Sprite.set_filename (sprite);

    // get associated terrain
    std::string tmp_terr = file.get_string("terrain", true);

    if (!tmp_terr.empty())
    {
        Terrain = tmp_terr;
    }

    return file.success ();
}
