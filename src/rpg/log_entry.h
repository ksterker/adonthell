/*
   $Id: log_entry.h,v 1.7 2009/04/08 19:36:02 ksterker Exp $
   
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
 * @file   rpg/log_entry.h
 * @author Kai Sterker <kaisterker@linuxgames.com> 
 * 
 * @brief  Log book entry.
 */

#ifndef RPG_LOGENTRY_H
#define RPG_LOGENTRY_H

#include <adonthell/base/flat.h>

namespace rpg
{
    /**
     * The %log_entry class encapsulates a single entry into one of the log books. Each entry
     * consists of a topic, the main text and a time stamp. It will also have a unique id to
     * allow cross-references and an index.
     */
    class log_entry
    {
        public:
            /**
             * Creates a new log entry.
             * @param topic 'Headline' of the entry.
             * @param text content of entry.
             * @param uid unique identifier for entry. Used by log index.
             */
            log_entry (const std::string & topic, const std::string & text, const std::string & uid);
            
            /**
             * @name Member Access
             */
            //@{
			/**
             * Return the topic of this log entry.
             * @return topic of the log entry.
             */
            std::string topic () const { return Topic; }
			
            /**
             * Return the text of this log entry.
             * @return text of the log entry.
             */
            std::string text () const { return Text; }
            
            /**
             * Return unique id of this log entry. This allows to find a certain log entry.
             * @return unique id of this log entry.
             */
            std::string uid () const { return Uid; }
            //@}
            
            /**
             * @name Loading/Saving
             */
            //@{
            /**
             * Load %log entry from stream. 
             * @param file stream to load entry from.
             * @return \b true if loading successful, \b false otherwise.
             */
            bool get_state (base::flat & file);
        
            /**
             * Save %log entry to a stream.
             * @param file stream to save entry to.
             * @return \b true if saving successful, \b false otherwise.
             */
            void put_state (base::flat & file) const;
            //@}

        private:
            /// creation time of log entry
            u_int32 Timestamp;
            /// main text of entry 
            std::string Text;
            /// topic of entry
            std::string Topic;
            /// unique id
            std::string Uid;
    };
}

#endif // RPG_LOGENTRY_H
