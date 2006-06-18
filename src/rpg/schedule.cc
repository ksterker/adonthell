/*
 $Id: schedule.cc,v 1.1 2006/06/18 19:26:48 ksterker Exp $
 
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
 * @file rpg/schedule.cc
 *
 * @author Kai Sterker
 * @brief Implements the character schedule class.
 */
 
#include "rpg/schedule.h"
#include "event/date.h"
#include "event/time_event.h"

using rpg::schedule;

// standart constructor
schedule::schedule ()
{
    Active = false;
    Running = false;
    QueuedSchedule = NULL;
}

// destructor
schedule::~schedule ()
{
    delete QueuedSchedule;
}

// execute the schedule
void schedule::update ()
{
    // no schedule active
    if (!Active) return;
    
    // no schedule running --> assign a new one
    if (!Running) 
    {
        // if schedule queued, try to activate it 
        if (QueuedSchedule)
        {
            QueuedSchedule->activate (this);
            delete QueuedSchedule;
            QueuedSchedule = NULL;            
        }

        // no queued schedule or initialization failed
        if (!Running)
        {
            // clearing the schedule before the manager runs
            // allows the manager to access the most recent data
            Schedule.clear ();
            Manager.call_method ("run");
        }
    }
        
    // finally run schedule
    Schedule.call_method ("run");
}

// assign a (new) schedule
bool schedule::set_schedule (const string & file, PyObject *args)
{
    if (Running)
    {
        fprintf (stderr, "*** schedule::set_schedule: stop current schedule first!\n");
        return false;
    }
    
    // no need to clear anything, as py_object takes care of that
    if (Schedule.create_instance (SCHEDULE_DIR + file, file, args))
    {    
        // 'run' schedule
        Running = true;
    
        // cancel alarm
        Factory.clear ();
    
        return true;
    }
    
    return false;
}

// queue a schedule
void schedule::queue_schedule (const string & file, PyObject *args)
{
    if (QueuedSchedule) delete QueuedSchedule;

    QueuedSchedule = new schedule_data (file, args);
}

// assign a (new) manager script
bool schedule::set_manager (const string & file, PyObject *args)
{
    return Manager.create_instance (SCHEDULE_DIR + file, file, args);
}

// set the alarm
void schedule::set_alarm (const string & time, const bool & absolute)
{
    string tm (time);
    
    // get rid of the current alarm
    Factory.clear ();
    
    // absolute hour of the day
    if (absolute)
    {
        char day[16];

        // if that hour has already passed today, assume the
        // same hour tomorrow
        if (events::date::parse_time (time) > (u_int32) events::date::hour () * 60)
            sprintf (day, "%id", events::date::day ());
        else
            sprintf (day, "%id", events::date::day () + 1);

        tm += day;
    }

    // create and register the new alarm
    events::time_event *ev = new events::time_event (tm, absolute);
    events::listener *li = Factory.add (ev, events::LISTENER_CXX);
    li->connect_callback (base::make_functor (*this, &schedule::on_alarm));
    li->set_id ("alarm");
}

// set alarm for a queued schedule
void schedule::queue_alarm (const string & time, const bool & absolute)
{
    if (!QueuedSchedule)
    {
        fprintf (stderr, "*** schedule::queue_alarm: queue a schedule first!\n");
        return;
    }
    
    QueuedSchedule->set_alarm (time, absolute);
}

// save state to disk
void schedule::put_state (base::flat & out) const
{
    out.put_bool ("sca", Active);
    out.put_bool ("scr", Running);
    
    // current manager script
    Manager.put_state (out);
    
    // current schedule script
    Schedule.put_state (out);
    
    // save alarm, if any
    Factory.put_state (out);
    
    // save queue, if any
    if (!QueuedSchedule)
    {
        out.put_bool ("scq", false);
    }
    else
    {
        out.put_bool ("scq", true);
        QueuedSchedule->put_state (out);
    }
}

// load state from disk
bool schedule::get_state (base::flat & in)
{
    Active = in.get_bool ("sca");
    Running = in.get_bool ("scr");
    
    // restore manager script
    if (!Manager.get_state (in))
    {
        fprintf (stderr, "*** schedule::get_state: failed loading manager script\n");
        return false;
    }
    
    // restore schedule script
    if (!Schedule.get_state (in))
    {
        fprintf (stderr, "*** schedule::get_state: failed loading schedule script\n");
        return false;
    }
    
    // restore alarm
    if (!Factory.get_state (in))
    {
        fprintf (stderr, "*** schedule::get_state: failed loading alarm\n");
        return false;
    }
    
    // restore listener callback (which cannot be saved)
    events::listener *li = Factory.get_listener ("alarm");
    if (li != NULL)
    {
        li->connect_callback (base::make_functor (*this, &schedule::on_alarm));
    }
    
    // restore queue
    if (in.get_bool ("scq"))
    {
        QueuedSchedule = new schedule_data;
        QueuedSchedule->get_state (in);
    }
    
    return in.success ();
}
