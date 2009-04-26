/*
   $Id: character.h,v 1.10 2009/04/26 18:52:59 ksterker Exp $
   
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

#include "base/diskio.h"
#include "base/hash_map.h"
#include "python/script.h"

namespace rpg
{
#ifndef SWIG
    /** The Python package containing %item templates. */
#   define CHARACTER_PACKAGE "characters."
#endif
    
    /** types of characters */
    enum char_type 
    { 
        PLAYER   = 0,
        NPC      = 1,
        CREATURE = 2
    };
    
#define MALE    0
#define FEMALE  1
    
    /**
     * The character base class, containing all rpg-related attributes of a character.
     */
    class character : public python::script
    {
        public:
            /**
             * Create a new character and add it to the global %character storage.
             * @param name the characters name
             * @param id unique id under which the character will be available in global %character storage.
             * @param type the characters type (PLAYER, NPC or CREATURE)
             */
            character (const std::string & name, const std::string & id, const rpg::char_type & type);
            
            /**
             * Delete character and remove it from the global character storage.
             */
            virtual ~character ();
        
            /**
             * Convenience method to instanciate a character template. Its usage is 
             * preferred over script::create_instance ().
             * @param templ name of the item template without .py extension
             * @param args argument tuple to pass to the template's constructor
             * @return \b true if instanciated successful, \b false otherwise. 
             */
            bool create_instance (const std::string & templ, PyObject * args = NULL)
            {
                if (python::script::create_instance (CHARACTER_PACKAGE + templ, templ, args))
                {
                    // add reference to character
                    set_attribute ("this", python::pass_instance (this));
                    return true;                    
                }
                
                return false;
            }
            
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
             * Set the color to use for speech produced by that 
             * character.
             * @param color the color associated with the character.
             */
            void set_color (const u_int32 & color)
            {
                Color = color;
            }

            /**
             * Get the color associated to that character.
             * @return the color to use for character speech.
             */
            u_int32 color () const
            {
                return Color;
            }
            
            /**
             * Return dialogue script of this character
             * @return dialogue script assigned to this character.
             */
            std::string dialogue () const
            {
                return Dialogue;
            }
            
            /**
             * Assign a new dialogue script to this character
             * @param dialogue the new dialogue.
             */
            void set_dialogue (const std::string & dialogue)
            {
                Dialogue = dialogue;
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
            static character *get_player ()
            {
                return get_character (PlayerCharacterId);
            }
            //@}
        
            /**
             * @name Loading/Saving
             */
            //@{
            /* Load character from named file. This will first load the %character 
             * template to instanciate the underlying Python item class. Then it will
             * restore the actual %character data. If an item is already instanciated,
             * it will be replaced.
             *
             * @param file name of the file to load %item from.
             * @return \b true if loading successful, \b false otherwise.
             */
             bool load (const string & file);
            
            /**
             * Save %character to named file. This will save both the item template
             * plus the actual data to the given file. The file will be replaced
             * if it already exists.
             *
             * @param file name of the file to save %character to.
             * @param format whether to save as XML or compressed binary.
             * @return \b true if saving successful, \b false otherwise.
             */
            bool save (const string & file, const base::diskio::file_format & format = base::diskio::BY_EXTENSION) const;
            
            /**
             * Loads the %character from a stream.
             * @param in stream to load the %character from.
             * @return \b true if loading successful, \b false otherwise.
             */
            bool get_state (base::flat & file);
            
            /**
             * Save the %character to a stream.
             * @param out stream where to save the %character.
             * @return \b true if loading successful, \b false otherwise.
             */
            bool put_state (base::flat & file) const;
            //@}
            
#ifndef SWIG
            GET_TYPE_NAME(rpg::character);
#endif // SWIG
        private:
            /// Name of the character, not neccessarily unique
            std::string Name;
            
            /// Unique identifier for this character
            std::string Id;
            
            /// Color used for the character in dialogues
            u_int32 Color;
            
            /// Dialogue script assigned to this character
            std::string Dialogue;
            
            /// list of all characters currently instanciated
            static std::hash_map<std::string, character*> Characters;
            
            /// id of the character currently controlled by the player
            static std::string PlayerCharacterId;
    };
}

#endif // RPG_CHARACTER_H
