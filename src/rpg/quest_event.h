/*
   $Id: quest_event.h,v 1.5 2009/04/08 19:36:03 ksterker Exp $

   Copyright (C) 2005 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file 	rpg/quest_event.h
 *
 * @author 	Kai Sterker
 * @brief 	Declares the quest_event class.
 */

#ifndef QUEST_EVENT_H
#define QUEST_EVENT_H

#include <vector>
#include "event/event.h"
#include "rpg/quest.h"

namespace rpg
{
    /**
     * The %quest %event executes the attached callback whenever a quest with
	 * matching pattern changes its state.
	 *
	 * This can for example be used to switch NPC behaviour based on the
	 * player's progress in the game and is preferred over changes made from
	 * within dialogues for instance. When there might be multiple places where
	 * a change of quest state can be triggered, the %quest %event will ensure
	 * that the required action is executed in any case.
     */
    class quest_event : public events::event
    {
    public:
        /**
         * @name Initialization
         */
        //@{
    
        /**
         * Create a new quest %event.
         *
         * @param pattern A pattern specifying the quest nodes that will trigger 
		 *  the event on changes. The pattern can either contain the path to a
		 *  single node, or it can contain the wildcard characters '*' and '>'.
		 *	The following table gives some examples for the usage:
		 *
		 *  <pre>
		 *  Pattern            Matches                   Doesn't match
		 *  --------------------------------------------------------------------
		 *  quest.part.step    quest.part.step           quest.part.another_step
		 *  
		 *  quest.*.step       quest.part.step           quest.part
		 *                     quest.another_part.step   quest.part.another_step
		 *
		 *  quest.>            quest.part                quest
		 *                     quest.part.step           another_quest.part.step
		 *  </pre>
		 *
		 *	'*' will match one level in the path to the quest node, while '>' 
		 *	will match all nodes with a common prefix.
		 * @param part @quest part that just changed its state.
		 */
        quest_event (const string & pattern, quest_part * part = NULL);
		
#ifndef SWIG
		/**
		 * Create empty quest event
		 */
		quest_event () { }
#endif // SWIG
		//@}
		
        /**
         * @name Event Handling
         */
        //@{
        /**
         * Compare two %quest events for equality.
         *
         * @param e The %quest %event to compare this to.
         * @return <b>True</b> if the two events equal, <b>false</b> otherwise.
         */
        bool equals (const events::event * e);

		/**
		 * Return %quest part that triggered this event.
		 * @return %quest part that triggered this event.
		 */
		quest_part *part ()
		{
			return Part;
		}
		
#ifndef SWIG
        /**
         * Get name of %event.
         * @return the string "quest_event";
         */
        const char* name () const
        {
            return "quest_event";
        }

		/**
		 * Get an iterator pointing to the first level of the pattern. 
		 * Used by quest event manager to store and compare quest events 
		 * more efficiently.
		 * @return iterator pointing to first level of pattern.
		 */
		std::vector<std::string>::const_iterator begin() const
		{
			return Pattern.begin();
		}

		/**
		 * Get an iterator pointing to the end of the pattern. 
		 * @return iterator pointing to end of pattern.
		 */
		std::vector<std::string>::const_iterator end() const
		{
			return Pattern.end();
		}
		
		/**
		 * Get the length of the %quest path represented by this %event
		 * @return number of levels of the event's %quest path
		 */
		const u_int32 size () const
		{
			return Pattern.size ();
		}
		
#endif // SWIG
        //@}
        
        /**
         * @name Loading / Saving
         */
        //@{
        
        /** 
         * Saves the %event %data to a stream.
         * 
         * @param file stream where to save the %event.
         */ 
        void put_state (base::flat& file) const;
        
        /** 
         * Loads the %event from a stream.
         * 
         * @param file stream to load the %event from.
         * @return \e true if the %event could be loaded, \e false otherwise
         */
        bool get_state (base::flat& file);
        //@}
    
#ifndef SWIG
        /**
         * Allow %event to be passed as python argument
         */
        GET_TYPE_NAME(rpg::quest_event)
#endif

    private:
		/**
		 * Set the pattern for the triggering events.
		 * @param pattern path of quest that will trigger events. It may
		 *  contain the wildcards '*' and '>'. 
		 */
		void set_pattern (const std::string & pattern);
	
        /// pattern that will trigger event
        std::vector<std::string> Pattern;
		/// %quest part that triggered the event
		quest_part *Part; 
    };
}

#endif // QUEST_EVENT_H
