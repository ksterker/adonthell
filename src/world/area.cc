/*
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

    // delete all the zones
    std::list<world::zone *>::const_iterator a;
    for (a = Zones.begin(); a != Zones.end(); a++)
    {
        delete *a;
    }

    Zones.clear();
}

// convenience method for adding object at a known index
bool area::put_entity (const u_int32 & index, coordinates & pos)
{
    if (index < Entities.size())
    {
        chunk::add(Entities[index], pos);
        return true;
    }

    fprintf (stderr, "*** area::put_entity: not entity at index %i!\n", index);
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

// get entity at given index
placeable * area::get_entity (const s_int32 & index) const
{
    if (index < Entities.size())
    {
        return Entities[index]->get_object();
    }

    return NULL;
}

// get named entity
placeable * area::get_entity (const std::string & id) const
{
    std::hash_map<std::string, world::named_entity*>::const_iterator entity = NamedEntities.find (id);
    if (entity == NamedEntities.end())
    {
        fprintf (stderr, "*** area::get_entity: entity '%s' doesn't exist!\n", id.c_str());
        for (entity = NamedEntities.begin(); entity != NamedEntities.end(); entity++)
        {
            fprintf (stderr, "  - '%s'\n", entity->first.c_str());
        }
        return NULL;
    }

    return entity->second->get_object();
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

        // this list contains a copy of all entities, unique or not.
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

const std::string * area::get_entity_name (const placeable * object) const
{
    std::hash_map<std::string, world::named_entity *>::const_iterator ei = NamedEntities.begin();
    while (ei != NamedEntities.end())
    {
        if (ei->second->get_object() == object)
            return &ei->first;
        ++ei;
    }

    return NULL;
}

// add zone
bool area::add_zone (world::zone * zone)
{
    std::list<world::zone *>::const_iterator i;

    for (i = Zones.begin(); i != Zones.end(); i++)
    {
        if ((*i)->name() == zone->name())
            return false;
    }

    Zones.insert(Zones.begin(), zone);

    return true;
}

// retrieve zone with a certain name
world::zone * area::get_zone (std::string & name)
{
    std::list<world::zone *>::const_iterator i;

    for (i = Zones.begin(); i != Zones.end(); i++)
    {
        if ((*i)->name() == name)
            return (*i);
    }

    return NULL;
}

// save to stream
bool area::put_state (base::flat & file) const
{
    collector objects;
    chunk::put_state (objects);
    bool type_saved = false;

    for (collector::const_iterator i = objects.begin(); i != objects.end(); i++)
    {
        base::flat entity;
        const collector_data & data = i->second;
        std::vector<chunk_info*>::const_iterator j;

        // save anonymous objects
        if (!data.Anonym.empty())
        {
            // save object type
            entity.put_uint8 ("type", data.Anonym.back()->get_object()->type());
            type_saved = true;

            base::flat anonym;
            for (j = data.Anonym.begin(); j != data.Anonym.end(); j++)
            {
                (*j)->Min.put_state (anonym);
            }

            entity.put_flat ("anonym", anonym);
        }

        // save unique named objects
        if (!data.Unique.empty())
        {
            if (!type_saved)
            {
                // save object type
                entity.put_uint8 ("type", data.Unique.back()->get_object()->type());
                type_saved = true;
            }

            base::flat unique;
            for (j = data.Unique.begin(); j != data.Unique.end(); j++)
            {
                unique.put_string ("id", *((*j)->get_entity()->id()));
                (*j)->Min.put_state (unique);
            }

            entity.put_flat ("unique", unique);
        }

        // save other named objects
        if (!data.Shared.empty())
        {
            if (!type_saved)
            {
                // save object type
                entity.put_uint8 ("type", data.Shared.back()->get_object()->type());
            }

            base::flat shared;
            for (j = data.Shared.begin(); j != data.Shared.end(); j++)
            {
                shared.put_string ("id", *((*j)->get_entity()->id()));
                (*j)->Min.put_state (shared);
            }

            entity.put_flat ("shared", shared);
        }

        type_saved = false;
        file.put_flat (i->first, entity);
    }

    // Save the zones
    if (!Zones.empty())
    {
        std::list <world::zone *>::const_iterator zone_i = Zones.begin();
        base::flat masterZones;
        base::flat zones;

        while (zone_i != Zones.end())
        {
            zones.put_sint32("MinX", (*zone_i)->min().x());
            zones.put_sint32("MinY", (*zone_i)->min().y());
            zones.put_sint32("MinZ", (*zone_i)->min().z());
            zones.put_sint32("MaxX", (*zone_i)->max().x());
            zones.put_sint32("MaxY", (*zone_i)->max().y());
            zones.put_sint32("MaxZ", (*zone_i)->max().z());

            masterZones.put_flat ((*zone_i)->name(), zones);
            zones.clear ();

            ++zone_i;
        }

        std::string temp ("Zones");
        file.put_flat (temp, masterZones);
    }

    return true;
}

// load from stream
bool area::get_state (base::flat & file)
{
    u_int32 size;
    u_int32 index = 0;
    coordinates pos;
    void *value;
    char *id;

    // iterate over map objects
    while (file.next (&value, &size, &id) == base::flat::T_FLAT)
    {
        base::flat entity = base::flat ((const char*) value, size);
        const std::string entity_file = id;

        if (strncmp(id, "Zones", 5) == 0) {

            while (entity.next (&value, &size, &id) == base::flat::T_FLAT)
            {
                base::flat zone_flat = base::flat ((const char*) value, size);

                world::vector3<s_int32> temp_min (zone_flat.get_sint32("MinX"), zone_flat.get_sint32("MinY"),
                    zone_flat.get_sint32("MinZ"));
                world::vector3<s_int32> temp_max (zone_flat.get_sint32("MaxX"), zone_flat.get_sint32("MaxY"),
                    zone_flat.get_sint32("MaxZ"));
                std::string temp_str = id;

                world::zone * temp_zone = new world::zone(temp_str, temp_min, temp_max);
                add_zone (temp_zone);

            }
        } else {

            const placeable_type type = (placeable_type) entity.get_uint8 ("type");

            // try loading anonymous objects
            base::flat record = entity.get_flat ("anonym", true);
            if (record.size() > 1)
            {
                // there are anonymous objects -> create instance
                placeable *object = add_entity (type);
                object->load (entity_file);

                // iterate over object positions
                while (record.next (&value, &size, &id) != base::flat::T_UNKNOWN)
                {
                    // get coordinate
                    pos.set_str (std::string ((const char*) value, size));

                    // place entity at current index at given coordinate
                    put_entity (index, pos);
                }

                // increase entity index
                index++;
            }

            // try loading unique objects
            record = entity.get_flat ("unique", true);
            if (record.size() > 1)
            {
                // iterate over object positions
                while (record.next (&value, &size, &id) != base::flat::T_UNKNOWN)
                {
                    // first get id of entity
                    std::string entity_name ((const char*) value);

                    // create a unique instance for each object
                    placeable *object = add_entity (type, entity_name);
                    object->load (entity_file);

                    // associate world object with its rpg representation
                    // TODO: maybe add this to add_entity() instead ...
                    switch (type)
                    {
                    	case world::CHARACTER:
                    	{
                            rpg::character *npc = rpg::character::get_character(entity_name);
                            ((world::character *) object)->set_mind (npc);
                            if (npc == NULL)
                            {
                            	fprintf (stderr, "*** area::get_state: cannot find rpg instance for '%s'.\n", entity_name.c_str());
                            }
                    	}
                    }
                    
                    // get coordinate
                    record.next (&value, &size, &id);
                    pos.set_str (std::string ((const char*) value, size));

                    // place entity at current index at given coordinate
                    put_entity (index, pos);

                    // increase entity index
                    index++;
                }
            }

            // try loading shared objects
            record = entity.get_flat ("shared", true);
            if (record.size() > 1)
            {
                // that's the instance that will be shared by all objects
                // it won't be placed on the actual map
                placeable *object = add_entity (type);
                object->load (entity_file);

                // increase entity index
                index++;

                // iterate over object positions
                while (record.next (&value, &size, &id) != base::flat::T_UNKNOWN)
                {
                    // first get id of entity
                    std::string entity_name ((const char*) value);

                    // create shared instance
                    add_entity (object, entity_name);

                    // get coordinate
                    record.next (&value, &size, &id);
                    pos.set_str (std::string ((const char*) value, size));

                    // place entity at current index at given coordinate
                    put_entity (index, pos);

                    // increase entity index
                    index++;
                }
            }
        }
    }

    return file.success ();
}

// save to file
bool area::save (const std::string & fname, const base::diskio::file_format & format) const
{
    // try to save character
    base::diskio record (format);
    if (!put_state (record))
    {
        fprintf (stderr, "*** area::save: saving '%s' failed!\n", fname.c_str ());
        return false;
    }

    // write item to disk
    return record.put_record (fname);
}

// load from file
bool area::load (const std::string & fname)
{
    // try to load area
    base::diskio record (base::diskio::BY_EXTENSION);

    if (record.get_record (fname) &&
        get_state (record))
    {
        Filename = fname;
        return true;
    }

    return false;
}
