/*
 $Id: schedule.h,v 1.2 2009/04/19 16:46:11 ksterker Exp $
 
 Copyright (C) 2004/2005/2006/2009 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file world/schedule.h
 *
 * @author Kai Sterker
 * @brief Declares the character schedule class.
 */

#ifndef WORLD_SCHEDULE_H
#define WORLD_SCHEDULE_H

#include "python/script.h"
#include "event/factory.h"
#include "world/schedule_data.h"

/**
 * Path to the character schedule scripts.
 */
#define SCHEDULE_DIR "schedules.char."

namespace world
{
    class area;
    
    /**
     * This class manages %character schedules. Each %character has a
     * manager script that defines what it does all day long. It has to
     * provide a "run" method that will be executed whenever it is time
     * to choose a different occupation.
     * 
     * The character's actual activity is controlled by the actual 
     * %schedule scripts that get selected by the manager script, depending 
     * on various factors. These schedules must be event driven and will
     * normally not be called from the engine, except on the following
     * occasions:
     *
     * - When starting a new schedule, it's "start" method is called.
     * - When setting a character inactive, the "pause" method is called.
     * - When resuming character activity, the "resume" method is called.
     * - When changing to a new schedule, its "stop" method is called.
     *
     * There are also calls to the constructor (__init__) and destructor
     * (__del__) at the start respectively end of the schedules lifetime.
     *
     * It is possible to bypass the manager schedule and select a new
     * schedule by queuing it prior to calling set_running(false).
     */
    class schedule
    {
    public:

        /**
         * Standart constructor. Initialize everything to NULL/false.
         */        
        schedule ();

        /**
         * Destructor.
         */        
        ~schedule ();
        
        /**
         * Executes the mapcharacter's %schedule script in case it is active
         * and running. If it stopped running, the manager script will be
         * launched to determine a new %schedule. If the %schedule isn't active,
         * nothing will happen.
         */
        void update ();

        /**
         * @name Map of Schedule.
         */
        //@{
        /**
         * Set the map the schedule operates on.
         * @param map pointer to the map instance.
         */
        void set_map (area * map)
        {
            Map = map;
        }
        
        /**
         * Get the map this schedule is attached to.
         * @return map this schedule is attached to.
         */
        area * get_map () const
        {
            return Map;
        }
        //@}
        
        /**
         * @name Manager Schedule / Schedule Control
         */
        //@{
        /** 
         * Returns whether the %schedule is active or not. An inactive
         * %schedule should cause the %character to freeze.
         * 
         * @return \c true if the %schedule is active, \c false otherwise.
         */
        bool is_active () const     { return Paused != 0; }
        
        /** 
         * Sets whether the %schedule is active or not. Setting the
         * %schedule inactive will result in a call to the schedule's 
         * "pause" method. Setting the schedule active will result in
         * a call to the "resume" method. Being inactive should cause 
         * the %character to freeze.
         * 
         * @param a \c true if the %schedule should be activated, \c false
         * otherwise.
         */
        void set_active (const bool & a);
        
        /** 
         * Returns whether the current %schedule is running or not. Once
         * it stops running, the manager script will be executed to
         * determine a new schedule.
         * 
         * @return \c true if the schedule is running, \c false otherwise.
         */
        bool is_running () const     { return Running; }
        
        /** 
         * Sets whether the %schedule is running or not. Once
         * it stops running, the manager script will be executed to
         * determine a new %schedule.
         *
         * @param a \c false if the %schedule should be stopped, \c true otherwise.
         */
        void set_running (const bool & r)    { Running = r; }
                
        /**
         * Assign a (new) manager script. This script is responsible for
         * the overall %character behaviour.
         *
         * @param file Filename of the script to load.
         * @param args Addional arguments to pass to the script constructor.
         * @return \e true on success, \e false otherwise
         */
        bool set_manager (const std::string & file, PyObject * args = NULL);
        //@}

        /**
         * Activity Schedule, Future Activity
         */
        //@{
        /**
         * Assign a (new) %schedule script, which is responsible for the
         * character's current activity. This method should only be used
         * from the manager %schedule. From within a %schedule script, use
         * queue_schedule () instead.
         *
         * @param file Filename of the script to load.
         * @param args Addional arguments to pass to the script constructor.
         *
         * @return \e true on success, \e false otherwise
         * @sa queue_schedule ()
         */
        bool set_schedule (const std::string & file, PyObject * args = NULL);
        
        /**
         * Call this to explicitly clear the attached %schedule. This will
         * also run the destructor (__del__ method) of the script, if it
         * exists.
         */
        void clear_schedule ()
        {
            Schedule.clear ();
        }
                
        /**
         * Set %schedule to use once the current one stops. Overrides
         * the %schedule otherwise chosen by the manager script. This
         * method allows a %schedule to decide upon the character's
         * next activity.
         *
         * @param file Filename of the script to load.
         * @param args Addional arguments to pass to the script constructor.
         * 
         * @sa set_schedule ()
         */
        void queue_schedule (const std::string & file, PyObject * args = NULL);
        
        /**
         * Specify a period after which the manager script is run,
         * whether the active %schedule is finished or not. This function
         * makes use of the %event system and is much more efficient than
         * querying the current time within the %schedule itself. It
         * therefore is the preferred method of letting schedules run a 
         * fixed amount of gametime.
         *
         * Note that setting a new %schedule script will cancel the alarm.
         * Setting a new alarm will also override the existing one. That
         * means, only one alarm can be active at any given time.
         *
         * @param time The amount of time the %schedule should be active,
         *      in the format used by time_event::time_event ().
         * @param absolute If <b>true</b>, the current day will be added
         *      to the time. For example, set_alarm ("14h", true) means
         *      "today at 14:00", whereas set_alarm ("14h", false) means
         *      "14 hours from now on".
         */
        void set_alarm (const std::string & time, const bool & absolute = false);
        
        /**
         * Set an alarm for a script set with queue_schedule (). This 
         * needs to happen after the call to %queue_schedule, but before
         * calling set_running (false). If a %schedule is already running,
         * use set_alarm () instead.
         * 
         * @param time The amount of time the %schedule should be active.
         * @param absolute whether the given time is relative to the current
         *      time or an absolute hour of the current (or next) day.
         *
         * @sa set_alarm ()
         */
        void queue_alarm (const std::string & time, const bool & absolute = false);
        //@}
        
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
    
    #ifndef SWIG
        /// allow schedule to be passed through SWIG
        GET_TYPE_NAME(world::schedule);
    #endif // SWIG
        
    private:
    #ifndef SWIG
        /// forbid copy construction
        schedule (const schedule & s);
        
        /**
         * Add this schedule as first argument to the given tuple.
         * @param args tuple passed as Python method argument.
         * @return a new tuple with the schedule at first position.
         */
        PyObject *add_schedule (PyObject *args) const;
        
        /// callback for alarm event
        void on_alarm () { set_running (false); }

        /// whether the schedule should be executed 
        u_int16 Paused;

        /// whether the manager should be executed
        bool Running;
        
        /// the script describing a character's general behaviour.
        python::script Manager;
        
        /// the script describing a specific activity in detail.
        python::script Schedule;
        
        /// store the alarm event to execute the manager script at a certain time
        events::factory Factory;
        
        /// schedule to set instead of running manager script
        schedule_data *QueuedSchedule;
        
        /// area for which this schedule is used
        area *Map;
    #endif // SWIG
    };

} // namespace world

#endif // WORLD_SCHEDULE_H
