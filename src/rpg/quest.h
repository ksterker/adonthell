/*
   $Id: quest.h,v 1.3 2004/10/18 07:40:23 ksterker Exp $
   
   Copyright (C) 2004 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file   rpg/quest.h
 * @author Kai Sterker <kaisterker@linuxgames.com> 
 * 
 * @brief  Keep track of progress in the game.
 */

#ifndef RPG_QUEST_H
#define RPG_QUEST_H

#include <map>
#include <vector>
#include "rpg/log_entry.h"

namespace rpg
{
#ifndef SWIG
    /**
     * Location in which the quests are kept
     */
    #define QUEST_DATA "/data/quest.data"
    
    /**
     * An internal class to represent the structure of a quest. Depending on its
     * content, it can represent a whole quest, a single step or a part (a group
     * of steps and/or parts).
     *
     * Each quest, part or step has three different states: started, in progress
     * and completed. A quest is started if at least one step is completed. It
     * is in progress as long as it has not been completely finished. To be
     * completely finished, either all steps must be completed, or a custom
     * completion rule must be satisfied.
     */
    class quest_part
    {
        public:
            /**
             * Create a new quest part. The id of the part must be unique for
             * quests. Parts can have the same id, as long as they belong to
             * different quests. The same is true for steps.
             * @param id name of the quest part.
             */
            quest_part (const std::string & id);
            
            /**
             * Delete quest part.
             */
            virtual ~quest_part ();
            
            /**
             * @name Query methods
             */
            //@{
            /**
             * Check whether a quest or part has been started.
             * @return \b true if has been started, \b false otherwise.
             */
            bool is_started () const { return Started; }
            /**
             * Check whether a quest, part or step has been completed.
             * @return \b true if that is the case, \b false otherwise.
             */
            bool is_completed () const { return Completed; }
            /**
             * Check whether a quest or part is in progress.
             * @return \b true if has been started but not completed, \b false otherwise.
             */
            bool in_progress () const { return is_started () && !is_completed (); }
            //@}
            
            /**
             * Set this quest parts to completed. This is only possible for parts that
             * do not have any children. The state of parent parts is updated accordingly. 
             */
            bool set_completed ();
            
            /**
             * Retrieve a %quest part with the given id.
             * @param id identifier of the part to return.
             * @return part on success, \b NULL otherwise.
             */
            const quest_part *child (const std::string & id) const;
            
            /**
             * Retrieve id of this %quest part.
             * @return id of the %quest part.
             */
            std::string id () const { return Id; }
            
            /**
             * @name Loading/Saving
             */
            //@{
            /**
             * Load %quest part (and its contents) from stream. 
             * @param in stream to load %quest part from.
             * @return \b true if loading successful, \b false otherwise.
             */
            bool get_state (base::flat & in);
        
            /**
             * Save %quest part (and contents) to a stream.
             * @param out stream to save %quest part to.
             * @return \b true if saving successful, \b false otherwise.
             */
            void put_state (base::flat & out) const;
            //@}
                
        protected:
            /**
             * Recalculate the state of completion. This method is called, when
             * the state of a child has changed, in order to check whether this
             * affects the state of its parent.
             */
            void update ();
            
            /**
             * Evaluate the completion rule in context of the children's state to
             * determine whether this quest part is completed or not.
             * @return \b true if it is completed, \b false otherwise.
             */
            bool evaluate ();
        
            /// Child quests
            std::map<std::string, quest_part*> Children;
            /// log entry if the quest is completed
            log_entry *Entry;
            /// Code to calculate completion
            std::string Code;
            /// Parent of that quest part
            quest_part *Parent;
            
        private:
            /// id of the quest
            std::string Id;
            /// Wether the quest has been started yet
            bool Started;
            /// Whether the quest has been finished
            bool Completed;
    };
#endif // SWIG

    /**
     * The quest class is used to keep track of the player's progress in the game.
     * For that, it stores a list of all the quests that make up the main plot line
     * and the various sub plots. Each such quest is organized as a tree. The root
     * of the tree is the quest itself as a whole. The leaves are all the individual
     * steps of the quest. Individual steps can be grouped together into parts. A
     * line of Python code can be assigned to each part to determine the state of
     * this part (completed or not) from the state of its steps or sub-parts. That
     * means that not all steps will be neccessary to complete a quest. The completion
     * of certain steps might even make the quest as a whole incompletable.
     *
     * As the game progresses, event or dialogue scripts can set quest steps to 
     * completed or query the state of quests, parts or steps. Whenever a quest,
     * part or step is completed, the attached log entry is copied into the log
     * book.
     *
     * To address a certain quest, part or step, its 'position' in the quest tree
     * must be kmown. The position is determined by the objects name and that of
     * its predecessors. In general, an address has the form 
     *
     * 		<quest>.<part>. ... .<part>.<step>
     */
    class quest
    {
        public:
            /**
             * Delete all quests.
             */
            static void cleanup ();
            
            /**
             * @name Query methods
             */
            ///@{
            /**
             * Check whether a quest or part has been started. This is the case 
             * if at least one of its steps has been completed.
             * @param id address of the quest or part.
             * @return \b true if has been started, \b false otherwise.
             */
            static bool is_started (const std::string & id);

            /**
             * Check whether a quest, part or step has been completed. Steps
             * have to be set to completed manually. Whether a whole quest or
             * part of the quest is completed will be determined by an optional
             * piece of python code. If that is missing, all children of the
             * object must be completed in order for the queried object to be
             * complete.
             * @param id address of the quest, part or step.
             * @return \b true if has been completed, \b false otherwise.
             */
            static bool is_completed (const std::string & id);

            /**
             * Check whether a quest or part is in progress. This is the
             * case if it has been started, but is not completed yet.
             * @param id address of the quest or part.
             * @return \b true if it is in progress, \b false otherwise.
             */
            static bool in_progress (const std::string & id);
            ///@}
            
            /**
             * Set a part of a quest to completed. Only quest steps can be set to
             * completed with this method. The status of quest parts or whole quests
             * will be determined automatically.
             *
             * Upon completion, the log entry attached to the step (or any parent
             * nodes) will be entered into the log book.
             *
             * @param id has to be address of a %quest step.
             */
            static void set_completed (const std::string & id);

            /**
             * @name Loading/Saving
             */
            //@{
            /**
             * Load all quests from stream. 
             * @param in stream to load quests from.
             * @return \b true if loading successful, \b false otherwise.
             */
            static bool get_state (base::flat & in);
            /**
             * Load all quests from %quest file. The file to load is
             * determined through the search path defined in base::Paths.
             * @return \b true if loading successful, \b false otherwise.
             */
            static bool get_state ();
            /**
             * Save all quests to stream.
             * @param out stream to save quests to.
             */
            static void put_state (base::flat & out);
            /**
             * Save all quests to %quest file in given directory. This
             * will usually be a saved game directory.
             * @param path directory to save quest to. 
             */
            static void put_state (const std::string & path);
            //@}

#ifndef SWIG
            /**
             * Add a %quest to list of quests.
             * @param part %quest to be added. 
             */
            static void add (quest_part *part);

        private:
            /**
             * Return the %quest part identified by the given path
             * @param path address of the %quest or part.
             * @return %quest_part if found, \b NULL otherwise
             */
            static const quest_part* get_part (const std::string & path);

            /**
             * Split the given path into its parts and return them as a list.
             * @param path path to a quest part or step
             * @return vector of the individual parts.
             */
            static std::vector<std::string> split (const std::string & path);
            
            /// all the quests available in the game
            static std::map<std::string, quest_part*> Quests;
#endif // SWIG
    };
}

#endif // RPG_QUEST_H
