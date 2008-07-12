/*
 $Id: area.cc,v 1.14 2008/07/12 11:12:37 ksterker Exp $
 
 Copyright (C) 2002 Alexandre Courbot <alexandrecourbot@linuxgames.com>
 Copyright (C) 2008 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file   world/area.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the area class.
 * 
 */

#include "world/area.h"
#include "world/character.h"
#include "world/object.h"

using world::coordinates;
using world::placeable;
using world::area;

// dtor
area::~area()
{
    clear();
}

// delete all entities
void area::clear()
{
    std::vector<entity*>::const_iterator i;
    for (i = Entities.begin(); i != Entities.end(); i++)
    {
        delete *i;
    }
    
    Entities.clear();
    NamedEntities.clear();
}

// set size of map grid
void area::resize (const u_int16 & nx, const u_int16 & ny) 
{
    Grid.resize (nx);
    for (std::vector <std::vector <square> >::iterator i = Grid.begin (); i != Grid.end (); i++)
        i->resize (ny); 
}

// get square of map grid
world::square * area::get (const u_int16 & x, const u_int16 & y)
{
    return (&Grid[x][y]);
}

// convenience method for adding object at a know index 
bool area::put_entity (const u_int32 & index, coordinates & pos)
{
    return put(Entities[index]->get_object(), pos);
}

// place object at given position on the grid
bool area::put (placeable * obj, coordinates & pos)
{
    const placeable_shape *shape = obj->current_shape ();
    if (!shape) return false;

    // calculate area of map squares occupied by the object's bounding box
    u_int16 end_x = (u_int16) ceil ((pos.ox () + shape->length ()) / (float) SQUARE_SIZE);
    u_int16 end_y = (u_int16) ceil ((pos.oy () + shape->width ()) / (float) SQUARE_SIZE); 

    // add to map
    return put (end_x, end_y, obj, pos, pos.z());
}

// add movable object to map
bool area::put (moving * obj) 
{
    const placeable_shape *shape = obj->current_shape ();
    if (!shape) return false;
    
    // calculate area of map squares occupied by the object's bounding box
    u_int16 end_x = (u_int16) ceil ((obj->ox () + shape->length ()/2.0f + shape->x()) / (float) SQUARE_SIZE);
    u_int16 end_y = (u_int16) ceil ((obj->oy () + shape->width ()/2.0f + shape->y()) / (float) SQUARE_SIZE); 
        
    return put (end_x, end_y, obj, *obj, obj->ground_pos());
}

// add to map
bool area::put (u_int16 & end_x, u_int16 & end_y, placeable *obj, coordinates & pos, const s_int32 & ground_z)
{
    chunk::add (obj, pos);
    
    u_int16 base_tile;

    u_int16 start_x = pos.x ();
    u_int16 start_y = pos.y (); 
    
    end_x += start_x;
    end_y += start_y;
    
    // make sure we do not exceed map size
    if (end_x > length()) end_x = length();
    if (end_y > height()) end_y = height();
    
    // calculate base tile
    const placeable_shape *shape = obj->current_shape ();
    if (shape->width() <= shape->height())
    {
        // bottom right tile will be used for rendering vertical objects 
        base_tile = end_x + end_y - 2;
    }
    else
    {
        // top left tile will be used for rendering flat objects
        base_tile = start_x + start_y;
    }
    
    square *sq; 
    
    // add object to all these squares
    for (u_int16 j = start_y; j < end_y; j++)
    {
        for (u_int16 i = start_x; i < end_x; i++) 
        {
            sq = get (i, j);
            sq->add (obj, pos, ground_z, i + j == base_tile); 
        }
    }
    
    return true;     
}

// remove static object from map
bool area::remove (placeable * obj, coordinates & pos) 
{
    chunk::remove (obj, pos);
    
    placeable_shape *shape = obj->current_shape ();
    if (!shape) return false;

    // calculate area of map squares occupied by the object's bounding box
    u_int16 start_x = pos.x (); 
    u_int16 start_y = pos.y ();    
    
    u_int16 end_x = start_x + (u_int16) ceil ((pos.ox () + shape->length () + shape->x()) / (float) SQUARE_SIZE);
    u_int16 end_y = start_y + (u_int16) ceil ((pos.oy () + shape->width () + shape->y()) / (float) SQUARE_SIZE); 
    
    // make sure we do not exceed map size
    if (end_x > length()) end_x = length();
    if (end_y > height()) end_y = height();
    
    square *sq; 
    
    for (u_int16 j = start_y; j < end_y; j++) 
    {
        for (u_int16 i = start_x; i < end_x; i++) 
        {
            sq = get (i, j);
            sq->remove (obj, pos); 
        }
    }
    
    return true; 
}

// remov moving object from map
bool area::remove (moving * obj) 
{
    return remove (obj, *obj); 
}

// update state of map
void area::update()
{
    std::vector<world::entity*>::const_iterator i;
    for (i = Entities.begin(); i != Entities.end(); i++)
    {
        (*i)->get_object()->update();
    }
}

// get named entity
placeable * area::get_entity (const std::string & id)
{
    std::hash_map<std::string, world::named_entity*>::iterator entity = NamedEntities.find (id);
    if (entity == NamedEntities.end())
    {
        fprintf (stderr, "*** area::get_entity: entity '%s' doesn't exist!\n", id.c_str());
        return NULL;
    }
    
    return (*entity).second->get_object();
}

// add anonymous entity
placeable * area::add_entity (const placeable_type & type)
{
    return add_entity (type, "");
}

// add named entity that can be retrieved by its id
placeable * area::add_entity (const placeable_type & type, const std::string & id)
{
    // check if entity is unique
    if (id.length() > 0 && NamedEntities.find (id) != NamedEntities.end ())
    {
        fprintf (stderr, "*** area::add_entity: entity '%s' already exists!\n", id.c_str());
        return NULL;
    }

    placeable *object = NULL;    

    // TODO: maybe generalize the event factory code (events::types) and use here as well
    switch (type)
    {
        case world::OBJECT:
        {
            object = new world::object (*this);
            break;
        }
        case world::CHARACTER:
        {
            object = new world::character (*this);
            break;
        }
        default:
        {
            fprintf (stderr, "*** area::add_entity: unknown type %i\n", type);
            break;
        }
    }

    // entity has been created successfully
    if (object != NULL)
    {
        world::entity *ety = NULL;
        
        if (id.length() == 0)
        {
            // handle anonymous entities
            ety = new world::entity (object);
        }
        else
        {
            // handle named entities
            ety = new world::named_entity (object, id);
            NamedEntities[id] = dynamic_cast<world::named_entity*> (ety);
        }
        
        // this list contains a copy to all entities, unique or not.
        Entities.push_back (ety);
    }
    
    // we do return the object here, as that's what is really interesting
    return object;
}

// add existing object as new entity
placeable * area::add_entity (placeable * object, const std::string & id)
{
    // check that entity is unique
    if (id.length() == 0 || NamedEntities.find (id) != NamedEntities.end ())
    {
        fprintf (stderr, "*** area::add_entity: entity '%s' already exists!\n", id.c_str());
        return NULL;
    }
    
    // no need to add to Entities, as the object is already contained.
    NamedEntities[id] = new world::named_entity (object, id);
    return object;    
}
