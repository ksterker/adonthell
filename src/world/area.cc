/*
 $Id: area.cc,v 1.15 2008/09/14 14:25:14 ksterker Exp $
 
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

// convenience method for adding object at a know index 
bool area::put_entity (const u_int32 & index, coordinates & pos)
{
    if (index < Entities.size())
    {
        chunk::add(Entities[index]->get_object(), pos);
        return true;
    }
    
    return false;
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
    
    // create non-unique entity
    world::named_entity *ety = new world::named_entity (object, id, false);
    
    // store in area
    Entities.push_back (ety);
    NamedEntities[id] = ety;
    
    return object;    
}
