/*
   $Id: date.cc,v 1.2 2003/12/29 10:01:59 uid66230 Exp $

   Copyright (C) 2002/2003 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file event/date.cc
 *
 * @author Kai Sterker
 * @brief Implements the date class.
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "event/date.h"
#include "base/timer.h"

using event::date;

// #include "time_event.h"
// #include "event_handler.h"

// gametime minutes spent in the gameworld so far
u_int32 date::Time = 0;

// number of game cycles since the last gametime minute passed
double date::Ticks = 0.0;

// Increase gametime 
void date::update ()
{
    static double tenth_minute = 0; // gametime::minute () / 10.0;
    
    // fts contains the number of cycles that passed since the last
    // call to date::update
    // Ticks += timer::frames_missed ();

    // check whether an in-game minute has passed
    while (Ticks >= tenth_minute)
    {
        Ticks -= tenth_minute;
        Time++;
        
        // raise time event
        // time_event evt (Time);
        // event_handler::raise_event (&evt);
    }
}

// load state from disk
bool date::get_state (base::igzstream &in)
{
    // read the current date as (gametime) minutes since start of the game
    Time << in;
    
    return true;
}

// save state to disk
void date::put_state (base::ogzstream &out)
{
    // write the time to disk
    Time >> out;
}

// calculate the current weekday
u_int16 date::weekday ()
{
    return day () % DAYS_PER_WEEK;
}

// calculate the current day
u_int16 date::day ()
{
    // how many minutes make one day
    static u_int16 day_in_minutes = 600 * HOURS_PER_DAY;

    return Time / day_in_minutes;
}

// calculate the hour of the current day
u_int16 date::hour ()
{
    return (Time / 600) % HOURS_PER_DAY;
}

// calculate minute of the hour
u_int16 date::minute ()
{
    return (Time / 10) % 60; 
}

// convert the time string to gametime minutes
u_int32 date::parse_time (const std::string & time)
{
    u_int32 t_minutes = 0, number = 0;
    char num[2] = "0";

    for (u_int32 i = 0; i < time.length (); i++)
    {
        // got a number
        if (isdigit (time[i]))
        {
            num[0] = time[i];
            number = 10 * number + atoi (num);
        }
        // got a letter
        else if (isalpha (time[i]))
        {
            switch (time[i])
            {
                // weeks
                case 'w':
                {
                    t_minutes += number * DAYS_PER_WEEK * HOURS_PER_DAY * 600;
                    break;
                }
                // days
                case 'd':
                {
                    t_minutes += number * HOURS_PER_DAY * 600;
                    break;
                }
                // hours
                case 'h':
                {
                    t_minutes += number * 600;
                    break;
                }
                // minutes
                case 'm':
                {
                    t_minutes += number * 10;
                    break;
                }
                // 1/10 minutes
                case 't':
                {
                    t_minutes += number;
                    break;
                }
                // error
                default:
                {
                    fprintf (stderr, "*** date::parse_time: Unknown time specifier '%c'\n", time[i]);
                    break;
                }
            }

            number = 0;
        }
    }

    return t_minutes;
}
