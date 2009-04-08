/*
   $Id: character.cc,v 1.5 2009/04/08 19:36:02 ksterker Exp $
   
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

#include "rpg/character.h"

using rpg::character;

/// global storage of all available character instances
std::hash_map<std::string, character*> character::Characters;

/// id of character currently controlled by the player
std::string character::PlayerCharacterId = "";

/// 

// ctor
character::character (const std::string & name, const std::string & id, const rpg::char_type & type) : python::script()
{
    Color = 0xFFFFFFFF;
    Dialogue = "";
    Name = name;
    Id = id;
    
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
}

// dtor
character::~character ()
{
    Characters.erase (Id);
    python::script::clear();
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

// load character from file
bool character::load (const string & file)
{
    // try to load character
    base::diskio record (base::diskio::BY_EXTENSION);
    
    if (record.get_record (file)) 
        return get_state (record);
    
    return false;    
}

// save a single character to file
bool character::save (const string & file, const base::diskio::file_format & format) const
{
    // try to save character
    base::diskio record (format);
    if (!put_state (record))
    {
        fprintf (stderr, "*** character::save: saving '%s' failed!\n", file.c_str ());        
        return false;
    }
    
    // write character to disk
    return record.put_record (file);
}

// save character to stream
bool character::put_state (base::flat & file) const
{
    // name, id and type need to be saved outside the character
    // as they are already required when a character is created
    
    // do we have a valid character?
    if (!Instance) return false;
    
    base::flat record;
    
    // save the attributes
    record.put_string ("cdlg", Dialogue);
    record.put_uint32 ("ccol", Color);
    
    // save the schedule
    Schedule.put_state (record);
        
    // save the template this item uses
    record.put_string ("ccls", class_name ());
    
    // pass record
    PyObject *args = PyTuple_New (1);
    PyTuple_SetItem (args, 0, python::pass_instance ((base::flat*) &record));
    
    // save the actual character data
    call_method ("put_state", args);
    Py_DECREF (args);

    file.put_flat ("char", record);
    return true;
}

// load character from stream
bool character::get_state (base::flat & file)
{
    base::flat record = file.get_flat ("char");
    if (!file.success ()) return false;
    
    // clean up, if neccessary
    if (Instance) clear ();
    
    // get attributes
    Dialogue = record.get_string ("cdlg");
    Color = record.get_uint32 ("ccol");
    
    // load the schedule
    if (!Schedule.get_state (record)) return false;
    
    // get template to use for character
    std::string tmpl = record.get_string ("ccls");
    
    // instanciate
    if (!create_instance (tmpl)) return false;
    
    // pass file
    PyObject *args = PyTuple_New (1);
    PyTuple_SetItem (args, 0, python::pass_instance (&record));
    
    // load actual character data
    call_method ("get_state", args);
    Py_DECREF (args);
    
    // add reference to character
    set_attribute ("this", python::pass_instance (this));
    
    return file.success ();
}
