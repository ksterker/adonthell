/*
   $Id: dialog_line.h,v 1.1 2006/01/22 21:32:40 ksterker Exp $
   
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
 * @file   rpg/dialog_line.h
 * @author Kai Sterker <kaisterker@linuxgames.com> 
 * 
 * @brief  Class representig a line of a dialog to be displayed by the UI.
 */

#ifndef RPG_DIALOG_LINE_H
#define RPG_DIALOG_LINE_H

#include <string>
#include <vector>
#include "base/types.h"

namespace rpg
{
    
class dialog;
    
/**
 * This class represents a line of dialogue, containing its speaker, text and the players possible
 * answers. It is provided by the dialogue system, so that the user interface can display the
 * dialogue accordingly. As such, it represents an actual, linear conversation and its history, 
 * not the branched out dialogue that is the basis of this conversation.
 */
class dialog_line
{
/// dialog class may access private class members
friend class dialog;
    
public:
    /**
     * Create a new dialogue line and link it to its precedessors.
     * @param speaker NPC to which this line of text is assigned to 
     * @param text NPCs text
     * @param prev line of dialogue preceeding this one or NULL if there is no preceeding line.
     */
    dialog_line (const std::string & speaker, const std::string & text, dialog_line *prev);
    
    /**
     * Delete this line of dialog and all its successors.
     */
    ~dialog_line();
    
    /**
     * @name Access to dialogue objects
     */
    ///@{
    /**
     * Return name of NPC this line of text is associated with.
     * @return name of NPC or "Narrator" if the text is a line of narration.
     */
    const std::string & speaker () const
    {
        return Speaker;
    }
    
    /**
     * Return this dialogue lines text.
     * @return NPC or Narrator speech.
     */
    const std::string & text () const
    {
        return Text;
    }
    
    /**
     * Return number of answers the player may chose from.
     * @return the number of answers the player may chose from (0 if there aren't any).
     */
    int num_answers () const
    {
        return Answers.size();
    }
    
    /**
     * Return players answer at given index.
     * @param index index of answer to retrieve between 0 and num_answers() - 1
     * @return players answer at given index.
     */
    const std::string answer (const u_int32 & index) const
    {
        if (index < Answers.size())
        {
            return Answers[index];
        }
        else
        {
            fprintf (stderr, "*** dialog_line::answer: index %i > number of answers %i", index, (int) Answers.size());
            return "Index out of range";
        }
    }
    //@}
    
    /**
     * @name Access to dialogue history
     */
    //@{
    /**
     * Return first line of the dialoge
     * @return line that started the dialogue
     */
    dialog_line *first ()
    {
        dialog_line *first = this;
        while (first->Prev != NULL)
        {
            first = first->Prev;
        }
        return first;
    }
    
    /**
     * Return succeeding line of dialoge, if any.
     * @return next line of dialogue, or null if there is none.
     */
    dialog_line *next () const
    {
        return Next;
    }
    
    /**
     * Return whether more lines follow in this dialogue.
     * @return true if another line of dialogue follows, false otherwise.
     */
    bool has_next () const
    {
        return Next != NULL;
    }
    
    /**
     * Return answer previously selected by the player. It is only available when
     * traversing the history of the dialogue.
     * @return answer the player has chosen, if any.
     */
    std::string selected_answer () const
    {
        if (SelectedAnswer > 0)
        {
            return answer (SelectedAnswer);
        }
        else
        {
            fprintf (stderr, "*** dialog_line::selected_answer: no answer selected!");
            return "No answer selected yet";            
        }
    }
    //@}
    
private:
    /// speaker of this line of text
    std::string Speaker;
    
    /// line of text
    std::string Text;
    
    /// possible player answers, if any
    std::vector<std::string> Answers;

    /// answer the player actually selected
    s_int32 SelectedAnswer;
    
    /// next line of dialogue
    dialog_line *Next;
    
    // previous line of dialog
    dialog_line *Prev;
};

} // namespace rpg
#endif // RPG_DIALOG_LINE_H
