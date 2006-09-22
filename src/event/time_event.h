/*
   $Id: time_event.h,v 1.7 2006/09/22 01:15:22 ksterker Exp $

   Copyright (C) 2002/2003/2004/2005 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file 	event/time_event.h
 *
 * @author 	Kai Sterker
 * @brief 	Declares the time_event class.
 */

#ifndef TIME_EVENT_H
#define TIME_EVENT_H

#include "event/event.h"

namespace events
{
    /**
     * The time %event executes the attached script or callback at a certain 
     * point in %game-time. This point can either be relative to the current
     * time, or absolute in time. In any case, this point should be in the
     * future. Time %event with an alarm time in the past will be triggered
     * at once.
     */
    class time_event : public event
    {
    public:
        /**
         * @name Initialization
         */
        //@{
    
        /**
         * Create a new time %event.
         *
         * @param time The time when the %event should be raised. The string
         *      specifies [w]eek, [d]ay, [h]our, [m]inute and [s]econds in the 
         * 		formatv"<number>w<number>d<number>h<number>m<number>s". If a 
         *      number is 0, it can be omitted.
         * @param absolute Decides whether the given time is relative from now
         *      on, or an absolute time
         */
        time_event (const string & time, bool absolute = false);
    
#ifndef SWIG
        /**
         * Standard constructor.
         */
        time_event () : event ()
        {
            Repeat = 1;
        }
        
        /**
         * Create a new time %event. This constructor is primarily used for
         * raising time events.
         *
         * @param time The "alarm" time in %gametime minutes.
         */
        time_event (const u_int32 & time) : event ()
        {
            Time = time;
            Repeat = 1;
        }
#endif // SWIG
        
        /**
         * Set whether the %event should be raised at fixed intervals.
         *
         * @param interval The time between two occurences of the %event.
         * @param count The number of times the %event shall be repeated.
         *      Specify -1 to repeat it an unlimited number of times.
         */
        void set_repeat (const string & interval, s_int32 count = -1);
        //@}
        
        /**
         * @name Event Handling
         */
        //@{
        /**
         * Compare two time events for equality.
         *
         * @param e The time event to compare this to.
         * @return <b>True</b> if the two events equal, <b>false</b> otherwise.
         */
        bool equals (const event * e)
        {
            return Time <= ((time_event *) e)->time ();
        }

#ifndef SWIG        
        /**
         * When time events repeat, their alarm time is increased by the
         * amount specified with time_event::set_repeat ().
         */
        void do_repeat ();

        /**
         * Get name of %event.
         * @return the string "time_event";
         */
        const char* name () const
        {
            return "time_event";
        }
#endif // SWIG
        //@}
        
        /**
         * @name Loading / Saving
         */
        //@{
        
        /** 
         * Saves the basic %event %data (such as the type or script data)
         * to a stream.
         * 
         * @param out stream where to save the %event.
         */ 
        void put_state (base::flat& out) const;
        
        /** 
         * Loads the basic %event %date from a stream.
         * 
         * @param in stream to load the %event from.
         * @return \e true if the %event could be loaded, \e false otherwise
         */
        bool get_state (base::flat& in);
    
        //@}
    
        /**
         * Get the event's "alarm" time, i.e. the time when it needs to be
         * executed.
         *
         * @return the "alarm" time in %gametime seconds.
         */
        u_int32 time () const
        {
            return Time;
        }

#ifndef SWIG
        /**
         * Allow %event to be passed as python argument
         */
        GET_TYPE_NAME(events::time_event)
#endif

    private:
        /// time when the event shall be triggered
        u_int32 Time;
    
        /// time that lies between two occurances of the event
        u_int32 Interval;
        
        /// whether the alarm time is relative or absolute
        bool Absolute;
    };
}

#endif // TIME_EVENT_H
