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
    
    // TODO: chunk::clear();
}

// convenience method for adding object at a known index
world::chunk_info *area::place_entity (const s_int32 & index, coordinates & pos)
{
    if (index >= 0 && index < Entities.size())
    {
        return chunk::add(Entities[index], pos);
    }

    fprintf (stderr, "*** area::place_entity: no entity at index %i!\n", index);
    return NULL;
}

// update state of map
void area::update()
{
    // FIXME: this might update objects several times 
    // if they are referred to by different entities
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

// add existing object as new entity
s_int32 area::add_entity (entity * ety)
{
    // handle named entities
    if (ety->id() != NULL)
    {
        std::string id = *ety->id();
        
        // check that entity is unique
        if (id.length() == 0 || NamedEntities.find (id) != NamedEntities.end ())
        {
            fprintf (stderr, "*** area::add_entity: entity '%s' already exists!\n", id.c_str());
            return -1;
        }
        
        // named entities are stored for later access
        NamedEntities[id] = dynamic_cast<world::named_entity*> (ety);
    }
    
    // this list contains a copy of all entities, named or not.
    Entities.push_back (ety);
    
    // return index of newly added entity
    return Entities.size() - 1;
}

// get name of entity associated to given object
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

// remove a specific zone
void area::remove_zone (world::zone * zone)
{
    Zones.remove (zone);
}

// retrieve zone with a certain name
world::zone * area::get_zone (std::string & name)
{
    std::list<world::zone *>::const_iterator i;

    for (i = Zones.begin(); i != Zones.end(); i++)
    {
        if (((*i)->type() & world::zone::TYPE_META) == world::zone::TYPE_META)
            if ((*i)->name() == name)
                return (*i);
    }

    return NULL;
}

// find zones of matching type at given position
std::vector<world::zone*> area::find_zones (const world::vector3<s_int32> & point, const u_int32 & type)
{
    std::vector<world::zone*> result;
    std::list<world::zone *>::const_iterator i;
    
    for (i = Zones.begin(); i != Zones.end(); i++)
    {
        if (((*i)->type() & type) == type)
        {
            if ((*i)->min().x() > point.x() || (*i)->max().x() < point.x()) continue; 
            if ((*i)->min().y() > point.y() || (*i)->max().y() < point.y()) continue; 
            if ((*i)->min().z() > point.z() || (*i)->max().z() < point.z()) continue;
            
            result.push_back (*i);
        }
    }
    
    return result;
}

// save to stream
bool area::put_state (base::flat & file) const
{
    u_int32 index = 0, act_idx = 0;
    char buffer[128];
    base::flat record;
    
    // list of map(interactions)
    base::flat action_list;
    
    // gather all different placeables and their locations on the map
    collector objects;
    chunk::put_state (objects);

    // first pass: save placeable models
    for (collector::const_iterator i = objects.begin(); i != objects.end(); i++)
    {
        base::flat entity;
        const world::placeable * data = i->first;
        data->save_model (entity);
        
        sprintf (buffer, "%d", index++);
        record.put_flat (buffer, entity);
    }
    file.put_flat ("objects", record);

    // reset
    index = 0;
    record.clear();

    // second pass: save entities and their positions and actions
    for (collector::const_iterator i = objects.begin(); i != objects.end(); i++)
    {
        base::flat entity;
        const collector_data & data = i->second;
        std::vector<chunk_info*>::const_iterator j;

        // save anonymous objects
        if (!data.Anonymous.empty())
        {
            base::flat anonym;
            for (j = data.Anonymous.begin(); j != data.Anonymous.end(); j++)
            {
                // save location action
                if ((*j)->has_action ())
                {
                    sprintf (buffer, "%d", act_idx++);
                    
                    base::flat actn_data;
                    (*j)->get_action()->put_state (actn_data);
                    action_list.put_flat (buffer, actn_data);
                    anonym.put_string ("action", buffer);
                }
                ((*j)->Min - (*j)->get_object()->entire_min()).put_state (anonym);
            }

            entity.put_flat ("anonym", anonym);
        }

        // save named objects
        if (!data.Named.empty())
        {
            base::flat named;
            for (j = data.Named.begin(); j != data.Named.end(); j++)
            {
                named.put_string ("id", *((*j)->get_entity()->id()));
                ((*j)->Min - (*j)->get_object()->entire_min()).put_state (named);
            }

            entity.put_flat ("named", named);
        }

        sprintf (buffer, "%d", index++);
        record.put_flat (buffer, entity);
    }
    file.put_flat ("actions", action_list);
    file.put_flat ("entities", record);

    // reset
    index = 0;
    record.clear();
    
    // third pass: save placeable states
    for (collector::const_iterator i = objects.begin(); i != objects.end(); i++)
    {
        base::flat entity;
        const world::placeable * data = i->first;
        data->put_state (entity);
        
        sprintf (buffer, "%d", index++);
        record.put_flat (buffer, entity);
    }
    
    file.put_flat ("states", record);

    // save the zones
    std::list <world::zone *>::const_iterator zone_i = Zones.begin();
    base::flat zone_list;

    while (zone_i != Zones.end())
    {
        (*zone_i)->put_state (zone_list);
        ++zone_i;
    }

    file.put_flat ("zones", zone_list);

    return true;
}

// load from stream
bool area::get_state (base::flat & file)
{
    u_int32 size;
    u_int32 index = 0;
    coordinates pos;
    placeable *object = NULL;
    void *value;
    char *id;

    // map (inter)actions
    std::string actn_id = "";
    
    // load placeable models
    std::vector<placeable*> tmp_objects;
    base::flat record = file.get_flat ("objects");
    
    // iterate over map objects
    while (record.next (&value, &size, &id) == base::flat::T_FLAT)
    {
        base::flat entity ((const char*) value, size);
        s_int8 type = entity.get_sint8("type");

        // TODO: maybe generalize the event factory code (events::types) and use here as well
        object = NULL;
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
                fprintf (stderr, "*** area::get_state: unknown object type %i\n", type);
                break;
            }
        }
        
        // entity has been created successfully
        if (object != NULL)
        {
            // load its actual data
            std::string modelfile = entity.get_string("model");
            object->load_model (modelfile);
        }
        
        // also store invalid objects, to not mess up the indices
        tmp_objects.push_back (object);
    }

    // load actions, if any
    base::flat action_list = file.get_flat ("actions");

    // load entities
    record = file.get_flat ("entities");
    while (record.next (&value, &size, &id) == base::flat::T_FLAT)
    {
        object = tmp_objects[index];
        if (object == NULL) continue;
        
        s_int32 ety_idx = -1;
        base::flat entity ((const char*) value, size);
        
        // try loading anonymous entities
        base::flat entity_data = entity.get_flat ("anonym", true);

        // iterate over entity positions
        while (entity_data.next (&value, &size, &id) != base::flat::T_UNKNOWN)
        {
            // load action associated to location, if any
            if (strcmp ("action", id) == 0)
            {
                // get action id
                actn_id = *((const char*) value);
                
                // read next value
                entity_data.next (&value, &size, &id);
            }
            
            // get coordinate
            pos.set_str (std::string ((const char*) value, size));
            
            if (ety_idx == -1)
            {
                // create entity ...
                world::entity *ety = new world::entity (object);
                // ... and add it to the map
                ety_idx = add_entity (ety);
            }

            // place entity at current index at given coordinate
            chunk_info *ci = place_entity (ety_idx, pos);
            
            // location has an action assigned
            if (actn_id != "")
            {
                base::flat actn_data = action_list.get_flat (actn_id);
                world::action *actn = ci->set_action ();
                actn->get_state (actn_data);
                actn_id = "";
            }
        }

        // try loading named entities
        entity_data = entity.get_flat ("named", true);
        // iterate over object positions
        while (entity_data.next (&value, &size, &id) != base::flat::T_UNKNOWN)
        {
            // load action associated to location, if any
            if (strcmp ("action", id) == 0)
            {
                // get action id
                actn_id = *((const char*) value);
                
                // read next value
                entity_data.next (&value, &size, &id);
            }
            
            // first get id of entity
            std::string entity_name ((const char*) value);

            // then get coordinate
            entity_data.next (&value, &size, &id);
            pos.set_str (std::string ((const char*) value, size));
            
            // create a named instance (that will be unique if it is the first) ...
            world::entity *ety = new world::named_entity (object, entity_name, ety_idx == -1);
            // ... and place it on the map
            chunk_info *ci = place_entity (add_entity (ety), pos);
            
            // location has an action assigned
            if (actn_id != "")
            {
                base::flat actn_data = action_list.get_flat (actn_id);
                world::action *actn = ci->set_action ();
                actn->get_state (actn_data);
                actn_id = "";
            }            
            
            // associate world object with its rpg representation
            switch (object->type())
            {
                case world::CHARACTER:
                {
                    rpg::character *npc = rpg::character::get_character(entity_name);
                    ((world::character *) object)->set_mind (npc);
                    if (npc == NULL)
                    {
                        fprintf (stderr, "*** area::get_state: cannot find rpg instance for '%s'.\n", entity_name.c_str());
                    }
                    break;
                }
                default:
                {
                    break;
                }
            }
        }
        
        // increase object index
        index++;
    }

    // reset
    index = 0;
    
    // load placeable states
    record = file.get_flat ("states");
    while (record.next (&value, &size, &id) == base::flat::T_FLAT)
    {
        object = tmp_objects[index];
        if (object == NULL) continue;
        
        base::flat entity ((const char*) value, size);
        object->get_state (entity);
        
        // increase object index
        index++;
    }
    
    // load zones
    record = file.get_flat ("zones");
    while (record.next (&value, &size, &id) == base::flat::T_FLAT)
    {
        base::flat zone = base::flat ((const char*) value, size);
        world::zone * temp_zone = new world::zone(id);
        temp_zone->get_state (zone);
        add_zone (temp_zone);
    }

    return file.success ();
}

// save to file
bool area::save (const std::string & fname, const base::diskio::file_format & format) const
{
    // try to save map
    base::diskio record (format);
    if (!put_state (record))
    {
        fprintf (stderr, "*** area::save: saving '%s' failed!\n", fname.c_str ());
        return false;
    }

    // write map to disk
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
