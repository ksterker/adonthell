/*
   $Id: character.cc,v 1.3 2006/02/15 21:30:42 ksterker Exp $
   
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
character::character (const std::string & name, const std::string & id, const rpg::char_type & type)
{
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

//
void character::put_state (base::flat & file) const
{
}

// 
bool character::get_state (base::flat & file)
{
    return file.success ();
}
