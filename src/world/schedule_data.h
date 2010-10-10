/*
 $Id: schedule_data.h,v 1.1 2009/04/16 21:06:10 ksterker Exp $
 
 Copyright (C) 2004/2005/2006 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file world/schedule_data.h
 *
 * @author Kai Sterker
 * @brief Declares the schedule_data class.
 */

#ifndef WORLD_SCHEDULE_DATA_H
#define WORLD_SCHEDULE_DATA_H

#ifdef _POSIX_C_SOURCE
#undef _POSIX_C_SOURCE
#endif
#ifdef _XOPEN_SOURCE
#undef _XOPEN_SOURCE
#endif

#include <Python.h>
#include <string>

namespace world
{
    class schedule;
    
    /**
     * Data needed to queue a schedule. Only used by the schedule
     * class internally.
     */
    class schedule_data
    {
    public:
        /**
         * Default constructor.
         */
        schedule_data () 
        { 
            Time = "";
            Absolute = false;
        }

        /**
         * Create new schedule data.
         * @param file schedule file name.
         * @param args arguments for schedule constructor.
         */
        schedule_data (const std::string & file, PyObject *args)
        {
            Py_XINCREF (args);

            File = file;
            Args = args;
            Time = "";
            Absolute = false;
        }

        /**
         * Free arguments.
         */
        ~schedule_data ()
        {
            Py_XDECREF (Args);
        }

        /**
         * Set active schedule from the stored values.
         * @param s schedule to update from data.
         * @return true on success, false otherwise.
         */
        bool activate (schedule *s);
        
        /**
         * Specify a period after which the manager script is run,
         * whether the active %schedule is finished or not. This function
         * makes use of the %event system and is much more efficient than
         * querying the current time within the %schedule itself. It
         * therefore is the preferred method of letting schedules run a 
         * fixed amount of gametime.
         *
         * @param time The amount of time the %schedule should be active,
         *      in the format used by time_event::time_event ().
         * @param absolute If <b>true</b>, the current day will be added
         *      to the time. For example, set_alarm ("14h", true) means
         *      "today at 14:00", whereas set_alarm ("14h", false) means
         *      "14 hours from now on".
         */
        void set_alarm (const std::string & time, const bool & absolute = false)
        {
            Time = time;
            Absolute = absolute;
        }
        
        /**
         * @name Loading / Saving
         */
        //@{
        /** 
         * Save the %schedule to a stream.
         * @param file stream where to save the %schedule.
         */ 
        void put_state (base::flat& file) const;
    
        /** 
         * Loads the %schedule from a stream.
         * @param file stream to load the %schedule from.
         * @return \e true if the %schedule was loaded successfully, \e false otherwise.
         */
        bool get_state (base::flat& file);
        //@}    
    
    private:
        /// name of schedule script 
        std::string File;
        /// arguments for schedule script constructor
        PyObject *Args;
        /// alarm time for schedule script
        std::string Time;
        /// whether alarm time is relative of absolute
        bool Absolute;
    };
} // namespace world

#endif // WORLD_SCHEDULE_DATA_H
