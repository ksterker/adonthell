/*
   $Id: timer.h,v 1.5 2004/11/15 08:54:33 ksterker Exp $

   Copyright (C) 2003/2004 Alexandre Courbot.
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
 * @file base/timer.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * @brief A timer implementation.
 */ 

#include <iostream>
#include <sys/time.h>
#include <time.h>

#include "base/types.h"

namespace base
{
    /**
     * This class has two purposes. For one, it can be used to measure
     * intervals of time. But it is also used to ensure a constant speed
     * of the game, independent from CPU speed and CPU load, given a fast
     * enough machine. It can detect slowdowns, that might be compensated
     * by skipping frames or reducing details. 
     *
     * For that second purpose, a global timer instance exists that should
     * be used: base::Timer. For simple time measuring, new objects should
     * be instanciated.
     */
    class timer
    {
    public:
        /**
         * Create a new timer with default values.
         */
        timer ();
       
        /**
         * Return length of a game cycle in milliseconds.
         * @return length of a game cycle.
         */
        u_int32 slice () const { return Slice; }
        
        /**
         * Return real time passed since creation of this timer 
         * in milli seconds. This will differ from uptime() if
         * update() hasn't been called regularly.
         * @return time passed since the timer was created.
         */
        u_int32 current_time () const;
        
        /**
         * Return the number of cycles that have been skipped
         * since the last call to timer::update(). On fast
         * hardware, this will usually be zero.
         * @return number of cycles skipped.
         */
        u_int32 frames_missed () const { return FramesMissed; }
        
        /**
         * Return time elapsed since creation of the counter.
         * This will differ from current_time() if update() 
         * hasn't been called regularly.
         * @return elapsed time in milliseconds.
         */
        u_int32 uptime () const { return Lasttime; }

        /**
         * Set the length of a single game cycle in ms. This is used
         * to syncronize the speed of the game. If the cycle can be
         * completed faster, the engine sleeps the time remaining.
         * If it takes longer to complete a cycle, frames should be
         * dropped to speed up the game.
         */
        void set_slice (u_int32 sl);

        /**
         * Call this after a cycle of the game has been completed.
         * It will either delay until the slice is completely done
         * or calculate the number of cycles that have been skipped
         * since the last call.
         */
        void update ();

    private:
        /**
         * suspend program for a certain number of milliseconds.
         * @param msecs amount of time to wait.
         */
        void sleep (u_int32 msecs) const;

        /**
         * Calculate difference between given time and creation
         * time of the timer in milliseconds.
         * @param tv a timestamp
         * @return difference between timestamp and creation time.
         */
        u_int32 convert_timeval (const struct timeval & tv) const
        { return ((tv.tv_sec - InitialTime.tv_sec) * 1000 + (tv.tv_usec - InitialTime.tv_usec) / 1000); }

        /// creation time of the %timer
        struct timeval InitialTime;
        /// length of a game cycle in milliseconds
        u_int32 Slice;
        /// amount of time this timer is running (in milliseconds)
        u_int32 Lasttime;
        /// number of cycles that had to be skipped
        u_int32 FramesMissed;
    };
}
