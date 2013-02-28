/*
   Copyright (C) 2003/2004 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file   rpg/character.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 *
 * @brief  Base class for creatures, NPCs and Player characters.
 */

#include <algorithm>

#include <adonthell/base/base.h>
#include <adonthell/base/logging.h>
#include "character.h"

using rpg::character;
using rpg::faction;

/// global storage of all available character instances
std::hash_map<std::string, character*> character::Characters;

/// id of character currently controlled by the player
std::string character::PlayerCharacterId = "";

/// character data file name
#define CHARACTER_DATA "character.data"

// ctor
character::character (const std::string & name, const std::string & id, const rpg::char_type & type,
                      const std::string & specie) : python::script()
{
    Color = 0xFFFFFFFF;
    Dialogue = "";
    Name = name;
    Id = id;
    Type = type;
    Base_Speed = 2.5;
    Speed = Base_Speed;

    // remember player character
    if (type == PLAYER)
    {
        if (PlayerCharacterId == "")
        {
            PlayerCharacterId = id;
        }
        else
        {
            LOG(WARNING) << "character: '" << PlayerCharacterId << "' is already the player character!";
        }
    }

    // remember character for later access
    std::hash_map<std::string, character*>::iterator i = Characters.find (id);
    if (i != Characters.end ())
    {
        LOG(ERROR) << "character: '" << id << "' already present in storage!";
    }
    else
    {
        Characters[id] = this;
    }

    // set specie to which this character belongs
    create_instance (specie);
}

// dtor
character::~character ()
{
    python::script::clear();
}

bool character::add_faction(const std::string & name)
{
    faction *tmp = faction::get_faction(name);
    if (tmp != NULL)
    {
        // Verify if this character conforms to the faction requirements
        if (tmp->verify_requirements() == true)
        {
            Factions.push_back(tmp);
            return true;
        }
    }
    
    return false;
}

bool character::remove_faction(const std::string & name)
{
    faction *tmp = faction::get_faction(name);
    std::vector<faction *>::iterator i = std::find(Factions.begin(), Factions.end(), tmp);
    
    if (i != Factions.end())
    {
        Factions.erase(i);
        return true;
    }

    return false;
}

bool character::belongs_to_faction(const std::string & name)
{
    faction *tmp = faction::get_faction(name);
    std::vector<faction *>::iterator i = std::find(Factions.begin(), Factions.end(), tmp);
    return i != Factions.end();
}

s_int32 character::get_faction_estimate_speed(const std::string & name) const
{
    std::vector<faction *>::const_iterator i;
    s_int32 sum = 0;
    
    for (i = Factions.begin(); i != Factions.end(); i++)
    {
        sum += (*i)->estimate_speed(name);
    }
    
    return sum;
}

// update speed based on terrain
void character::update_speed(const std::string & terrain)
{
	float actual_speed = base_speed();

	// take the specie into consideration
	s_int32 specie_effects = 0; // todo
	actual_speed += ((specie_effects * 0.01) * base_speed());

	// take the various factions into consideration
	s_int32 faction_effects = get_faction_estimate_speed(terrain);
	actual_speed += ((faction_effects * 0.01) * base_speed());

	set_speed (actual_speed);
}

// return character with given id
character *character::get_character (const std::string & id)
{
    std::hash_map<std::string, character*>::iterator i = Characters.find (id);
    if (i != Characters.end ())
    {
        return (*i).second;
    }

    LOG(ERROR) << "character::get_character: no character with id '" << id << "' found!";
    return NULL;
}

// load characters from file
bool character::load ()
{
    base::diskio file;
    
    // try to load character
    if (!file.get_record (CHARACTER_DATA)) return false;
        
    u_int32 size;
    char *data;
    char *id;
    
    character *c;
    bool result = true;
    
    // iterate over all saved characters
    while (file.next ((void**) &data, &size, &id) == base::flat::T_FLAT)
    {
        c = new character ();
        base::flat record (data, size); 
        if (c->get_state(record))
        {
            // loading successful
            Characters[id] = c;
            continue;
        }

        LOG(ERROR) << "character::load: loading character '" << id << "' failed.";
        result = false;
        delete c;
    }
    
    return result;
}

// save characters to file
bool character::save (const string & path)
{
    // try to save characters
    base::diskio file;
    
    std::hash_map<std::string, character*>::iterator i;
    for (i = Characters.begin(); i != Characters.end(); i++)
    {
        base::flat record;
        if (!i->second->put_state (record))
        {
            LOG(ERROR) << "character::save: saving character '" << i->first << "' failed!";
            return false;
        }
        file.put_flat (i->first, record);
    }

    // write character to disk
    return file.put_record (path + "/" + CHARACTER_DATA);
}

void character::cleanup()
{
    // cleanup first
    std::hash_map<std::string, character*>::iterator i = Characters.begin();
    while (i != Characters.end())
    {
        delete i->second;
        ++i;
    }
    Characters.clear();
}

// save character to stream
bool character::put_state (base::flat & file) const
{
    // do we have a valid character?
    if (!Instance) return false;

    // save the attributes
    file.put_uint8 ("ctyp", Type);
    file.put_string ("cnam", Name);
    file.put_string ("cid", Id);
    file.put_string ("cdlg", Dialogue);
    file.put_uint32 ("ccol", Color);
    file.put_string ("cptr", Portrait);
    file.put_float ("cspd", Base_Speed);

    // save factions
    base::flat factions;
    for (std::vector<faction*>::const_iterator i = Factions.begin(); i != Factions.end(); i++)
    {
        factions.put_string("", (*i)->name());
    }
    file.put_flat("cfct", factions);

    // save the template this character uses
    file.put_string ("ccls", class_name ());

    // pass record
    PyObject *args = PyTuple_New (1);
    PyTuple_SetItem (args, 0, python::pass_instance ((base::flat*) &file));

    // save the actual character data
    call_method ("put_state", args);
    Py_DECREF (args);

    return true;
}

// load character from stream
bool character::get_state (base::flat & file)
{
    // clean up, if neccessary
    if (Instance) clear ();
    Factions.clear();

    // get attributes
    Type = (char_type) file.get_uint8 ("ctyp");
    Name = file.get_string ("cnam");
    Id = file.get_string ("cid");
    Dialogue = file.get_string ("cdlg");
    Color = file.get_uint32 ("ccol");
    Portrait = file.get_string ("cptr");
    Base_Speed = file.get_float ("cspd");

    void *data;
    base::flat factions = file.get_flat("cfct");
    // iterate over all saved factions
    while (factions.next ((void**) &data, NULL, NULL) == base::flat::T_STRING)
    {
        add_faction((const char *)data);
    }

    Speed = Base_Speed;

    if (Type == PLAYER) PlayerCharacterId = Id;
    
    // get template to use for character
    std::string tmpl = file.get_string ("ccls");

    // instanciate
    if (!create_instance (tmpl)) return false;

    // pass file
    PyObject *args = PyTuple_New (1);
    PyTuple_SetItem (args, 0, python::pass_instance (&file));

    // load actual character data
    call_method ("get_state", args);
    Py_DECREF (args);

    return file.success ();
}
