/*
   $Id: date.cc,v 1.11 2009/04/08 21:52:09 ksterker Exp $

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
 * @file event/date.cc
 *
 * @author Kai Sterker
 * @brief Implements the date class.
 */

#include "event/manager.h"
#include "event/date.h"
#include "event/time_event.h"
#include "base/base.h"
#include "base/diskio.h"

/// filename of time data file
#define TIME_DATA "time.data"

using events::date;

// gametime seconds spent in the gameworld so far
u_int32 date::Time = 0;

// how many game cycles make one second of game time
float date::Scale = 5.0;

// number of game time seconds before a time event will be raised
u_int16 date::Resolution = 1;

// number of game cycles since the last gametime second passed
double date::Ticks = 0.0;


// Increase gametime 
void date::update ()
{
    // frames_missed contains the number of cycles that have been
    // skipped during the last call to date::update
    Ticks += 1 + base::Timer.frames_missed ();

    // check whether to trigger time events
    while (Ticks >= Scale)
    {
        Ticks -= Scale;
        Time++;
        
        // raise time event
        if (Time % Resolution == 0) 
        {
            time_event evt (Time);
            manager::raise_event (&evt);
        }
    }
}

// load date from disk
bool date::load ()
{
    base::diskio file;

    // try to read file from disk
    if (!file.get_record (TIME_DATA)) return false;
    
    // read data from file
    return date::get_state (file);
}

// save date to disk
bool date::save (const std::string & path)
{
    base::diskio file;

    // save data
    date::put_state (file);
    
    // write file to disk
    return file.put_record (path + "/" + TIME_DATA);
}

// load state from stream
bool date::get_state (base::flat & file)
{
    // read the current date as (gametime) minutes since start of the game
    Time = file.get_uint32 ("time");
    Scale = file.get_float ("scale");
    Resolution = file.get_uint16 ("resolution");
    
    return file.success();
}

// save state to stream
void date::put_state (base::flat & file)
{
    // write the time to disk
    file.put_uint32 ("time", Time);
    file.put_float ("scale", Scale);
    file.put_uint16 ("resolution", Resolution);
}

// calculate the current weekday
u_int16 date::weekday (const u_int32 & time)
{
    return day (time) % DAYS_PER_WEEK;
}

// calculate the current day
u_int16 date::day (const u_int32 & time)
{
    // how many minutes make one day
    static u_int16 day_in_minutes = 600 * HOURS_PER_DAY;

    return time / day_in_minutes;
}

// calculate the hour of the current day
u_int16 date::hour (const u_int32 & time)
{
    return (time / 600) % HOURS_PER_DAY;
}

// calculate minute of the hour
u_int16 date::minute (const u_int32 & time)
{
    return (time / 10) % 60; 
}

// convert the time string to gametime minutes
u_int32 date::parse_time (const std::string & time)
{
    u_int32 secs = 0, number = 0;
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
                    secs += number * DAYS_PER_WEEK * HOURS_PER_DAY * 3600;
                    break;
                }
                // days
                case 'd':
                {
                    secs += number * HOURS_PER_DAY * 3600;
                    break;
                }
                // hours
                case 'h':
                {
                    secs += number * 3600;
                    break;
                }
                // minutes
                case 'm':
                {
                    secs += number * 60;
                    break;
                }
                // seconds
                case 's':
                {
                    secs += number;
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

    if (number != 0)
        fprintf (stderr, "*** date::parse_time: Time specifier missing at end of '%s'\n", time.c_str ());
        
    return secs;
}

// format timestamp according to given format string
std::string date::format_time (const std::string & format, const u_int32 & time)
{
	bool is_code = false;
	std::string result = "";
	
    for (u_int32 i = 0; i < format.length (); i++)
	{
		// last letter was '%'
		if (is_code)
		{
			char str[16];
				
			switch (format[i])
			{
				// day of week
				case 'w':
				{
					snprintf (str, 15, "%i", weekday (time));
					break;
				}
				// days since start of game
				case 'd':
				{
					snprintf (str, 15, "%i", day (time));
					break;
				}
				// hours of gametime
				case 'h':
				{
					snprintf (str, 15, "%i", hour (time));
					break;
				}
				// minutes of gametime
				case 'm':
				{
					snprintf (str, 15, "%i", minute (time));
					break;
				}
				// no valid format code
				default:
				{
					snprintf (str, 15, "%%%c", format[i]);
					break;
				}
			}

			if (format[i] != '%') is_code = false;
			result += str;
			continue;
		}
		
		// found format code indicator
		if (format[i] == '%')
		{
			is_code = true;
			continue;
		}
		
		// append all other characters of format string to result
		result += format[i];
	}
	
	return result;
}

// convert realtime to gametime
u_int32 date::convert_millis (const u_int32 & millis)
{
    return (u_int32) (millis / base::Timer.slice () / Scale);
}
