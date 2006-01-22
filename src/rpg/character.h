/*
   $Id: character.h,v 1.3 2006/01/22 21:32:39 ksterker Exp $
   
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
 * @file   rpg/character.h
 * @author Kai Sterker <kaisterker@linuxgames.com> 
 * 
 * @brief  Base class for creatures, NPCs and Player characters.
 */

#ifndef RPG_CHARACTER_H
#define RPG_CHARACTER_H

#include "base/hash_map.h"
#include "python/script.h"

namespace rpg
{
#define MALE    0
#define FEMALE  1
    
    /**
     * The character base class, containing all rpg-related attributes of a character.
     */
    class character : public python::script
    {
        public:
            /**
             * Create a new character and add it to the global character storage.
             */
            character ();
            
            /**
             * Delete character and remove it from the global character storage.
             */
            ~character ();
        
            /**
             * @name Member access
             */
            //@{
            /**
             * Return name of this character
             * @return characters name.
             */
            std::string name () const
            {
                return Name;
            }

            /**
             * Return dialogue script of this character
             * @return dialogue script assigned to this character.
             */
            std::string dialogue () const
            {
                return Dialogue;
            }
            //@}
            
            /**
             * @name Global character access
             */
            //@{
            /**
             * Return a character from the list of characters by its unique id.
             * @param id unique id of character to return.
             * @return character with given id or NULL if no such character exists.
             */
            static character *get_character (const std::string & id);
            
            /**
             * Return the main character, usually controlled by the player.
             * @return the player character.
             */
            static character *get_player ();
            //@}
        
            /**
             * @name Loading/Saving
             */
            //@{
            /**
             * 
             * @return \b true if loading successful, \b false otherwise.
             */
            bool get_state (base::flat & file);
            
            /**
             *
             */
            void put_state (base::flat & file) const;
            //@}
            
#ifndef SWIG
            GET_TYPE_NAME(character);
#endif // SWIG
        private:
            /// Name of the character, not neccessarily unique
            std::string Name;
            
            /// Unique identifier for this character
            std::string Id;
            
            /// Dialogue script assigned to this character
            std::string Dialogue;
            
            /// list of all characters currently instanciated
            static std::hash_map<std::string, character*> Characters;
    };
}

#endif // RPG_CHARACTER_H
