/*
   $Id: timer.cc,v 1.12 2008/04/14 11:03:09 ksterker Exp $

   Copyright (C) 2003/2004/2006 Alexandre Courbot <alexandrecourbot@linuxgames.com>
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
 * @file base/timer.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * @brief A timer implementation.
 */ 

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <errno.h>

#include "base/timer.h"

// some OS don't have nanosleep
#ifndef HAVE_NANOSLEEP
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

/**
 * POSIX timespec struct, for use by non-POSIX systems.
 *
 */
struct timespec 
{
    /// time in seconds
	time_t tv_sec;
    /// fraction of a second, in nanoseconds
	long tv_nsec;
};

int nanosleep (const struct timespec *req, struct timespec *rem) 
{
	Sleep (req->tv_sec * 1000 + req->tv_nsec / 1000000);
    rem->tv_nsec = 0;
    rem->tv_sec = 0;
    return 0;
}
#endif // HAVE_NANOSLEEP

namespace base
{
    // ctor
    timer::timer() : Slice(25), Lasttime(0), FramesMissed(0)
    {
        gettimeofday (&InitialTime, NULL);
    }

    // set duration of one game cycle in ms
    void timer::set_slice(u_int32 sl)
    {
        Slice = sl;
    }

    // return time passed since creation of timer 
    u_int32 timer::current_time() const
    {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return convert_timeval(tv);
    }

    // suspend application for given time
    void timer::sleep (u_int32 msecs)
    {
        unsigned char err;
        struct timespec req, rem;
        rem.tv_sec = msecs / 1000;
        rem.tv_nsec = (msecs % 1000) * 1000000;

        do
        {
            req = rem;
            err = nanosleep (&req, &rem);
        } while (err && (errno == EINTR));
    }

    // wait until current game cycle is over
    void timer::update ()
    {
        register u_int16 delay = current_time () - Lasttime;

        // wait if the current frame was calculated too fast
        if (Slice > delay) {
            sleep (Slice - delay);
            delay = current_time () - Lasttime;
            FramesMissed = 0;
        }
        // see whether calculating the current frame took too long
        else FramesMissed = delay / Slice;
        
        Lasttime += delay;
    }
}
