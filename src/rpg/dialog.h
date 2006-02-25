/*
 $Id: dialog.h,v 1.3 2006/02/25 18:21:03 ksterker Exp $
 
 Copyright (C) 2006 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file   rpg/dialog.h
 * @author Kai Sterker <kaisterker@linuxgames.com> 
 * 
 * @brief  Core of the dialogue system.
 */

#ifndef RPG_DIALOG_H
#define RPG_DIALOG_H

#include "python/script.h"
#include "rpg/character.h"
#include "rpg/dialog_line.h"

namespace rpg
{
    /**
     * The Python package containing %dialogue scripts templates.
     */
    #ifndef SWIG
    #   define DIALOGUE_PACKAGE "dialogues."
    #endif
        
    /**
     * The dialog class interpretes the Python dialogue scripts and provides
     * the result to the user interface.
     *
     */
    class dialog : public python::script
    {
    public:
        /**
         * Initiate dialogue with given NPC. The dialogue attribute of the character
         * determines which dialogue is loaded.
         */
        dialog (rpg::character & npc);
        
        /**
         * Delete dialogue and history.
         */
        virtual ~dialog ();
        
        /**
         * Reload a dialogue whose script has changed on disk. Mainly useful during 
         * development. Should be called immediately after constructor.
         */
        bool reload ();
        
        /**
         * Execute the next step of the dialogue. The first time this method is called 
         * (i.e. at the dialogue's beginning), -1 needs to be passed. 
         * @param answer index of answer selected by the player or -1 if there are no 
         *     player choices.
         * @return the next line of the dialogue, containing NPC speech and possible
         *     player responses.
         */
        const rpg::dialog_line *run (const s_int32 & answer = -1);
        
        /**
         * Return history of this dialogue, i.e. all lines in the order they
         * have occured in the dialogue so far.
         */
        const rpg::dialog_line *history () const
        {
            if (Current != NULL) return Current->first();
            return NULL;
        }
        
    private:
        /**
         * Copy frequently used objects from the Python dialogue script for
         * faster access.
         */
        bool init ();
           
        /**
         * Reset dialog to state after construction.
         */
        void cleanup ();
        
        /**
         * Search the given line for $name and $fm{/} macros and embedded python code.
         * Replace any occurances with the correct value and return the resulting string.
         * @param line line of dialogue to process
         * @return line of dialogue with macros replaced and code executed.
         */
        std::string scan_string (const char* line);

        /**
         * Get a substring of given line, starting from begin and finishing at end.
         * @param line string from which to retrieve a part.
         * @param begin begin of substring.
         * @param end end of substring.
         * @return new string, containg part of given string.
         */
        char* get_substr (const char* line, char* begin, char* end); 
        
        /// arguments passed to the python dialogue script constructor
        PyObject *Args;
        /// history of this dialogue (i.e. its first line)
        rpg::dialog_line* Current;
        /// this dialogues lines of text
        char** Lines;
        /// the lines of text that are allowed to loop
        std::vector<s_int32> Loop;
        /// the successors of the current line
        std::vector<s_int32> Successors;
        /// the lines of text already uttered during the conversation
        std::vector<s_int32> Used;
    };

} // namespace rpg
#endif // RPG_DIALOG_H
