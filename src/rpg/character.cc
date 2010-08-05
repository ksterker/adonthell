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

#include "base/base.h"
#include "rpg/character.h"

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
            fprintf (stderr, "*** character: '%s' is already the player character!\n", PlayerCharacterId.c_str());
        }
    }

    // remember character for later access
    std::hash_map<std::string, character*>::iterator i = Characters.find (id);
    if (i != Characters.end ())
    {
        fprintf (stderr, "*** character: '%s' already present in storage!\n", id.c_str());
    }
    else
    {
        Characters[id] = this;
    }

    // set specie to which this character belongs
    set_specie(specie);
}


// dtor
character::~character ()
{
    Characters.erase (Id);
    python::script::clear();
}

std::vector<faction *>::const_iterator character::begin() const
{
    return Factions.begin();
}

std::vector<faction *>::const_iterator character::end() const
{
    return Factions.end();    
}

faction * character::get_faction(const std::string & name) const
{
    faction * tmp = faction::get_faction(name);
    
    return tmp;
}

bool character::add_faction(const std::string & name)
{
    faction * tmp = get_faction(name);
    
    if (tmp != NULL)
    {
        // Verify if this character conforms to the faction requirements
        if (tmp->verify_requirements() == true)
        {
            Factions.push_back(tmp);
            
            Pathfinding_Costs.update_costs(Factions, Specie);
            
            return true;
        }
    }
    
    return false;
}

bool character::remove_faction(const std::string & name)
{
    faction * tmp = get_faction(name);
    
    std::vector<faction *>::iterator i = find(Factions.begin(), Factions.end(), tmp);
    
    if (*i != NULL)
    {
        Factions.erase(i);
        
        Pathfinding_Costs.update_costs(Factions, Specie);
        return true;
    } else return false;
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

// update speed base on terrain
void character::update_speed(const std::string & terrain)
{
	float actual_speed = base_speed();

	// take the specie into consideration
	s_int32 specie_effects = specie()->estimate_speed(terrain);
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

    fprintf (stderr, "*** character::get_character: no character with id '%s' found!\n", id.c_str());
    return NULL;
}

// load characters from file
bool character::load ()
{
    // cleanup first
    /*
    std::hash_map<std::string, character*>::iterator i;
    while (!Characters.empty())
    {
        i = Characters.begin();
        character *c = i->second;
        delete c; // this removes c from Characters and invalidates i
    }
    */
    Characters.clear();
    
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

        fprintf (stderr, "*** character::load: loading character %s failed.\n", id);
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
            fprintf (stderr, "*** character::save: saving character '%s' failed!\n", i->first.c_str ());
            return false;
        }
        file.put_flat (i->first, record);
    }

    // write character to disk
    return file.put_record (path + "/" + CHARACTER_DATA);
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
    file.put_float ("cspd", Base_Speed);

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

    // get attributes
    Type = (char_type) file.get_uint8 ("ctyp");
    Name = file.get_string ("cnam");
    Id = file.get_string ("cid");
    Dialogue = file.get_string ("cdlg");
    Color = file.get_uint32 ("ccol");
    Base_Speed = file.get_float ("cspd");
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
