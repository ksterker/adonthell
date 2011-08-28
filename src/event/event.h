/*
   $Id: event.h,v 1.9 2009/04/08 21:52:09 ksterker Exp $

   Copyright (C) 2000/2001/2002/2003/2004/2005 Kai Sterker 
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
 * @file   event/event.h
 * @author Kai Sterker <kaisterker@linuxgames.com>
 *
 * @brief  Declares the %event class.
 *
 */

#ifndef EVENT_EVENT_H
#define EVENT_EVENT_H

#include "base/flat.h"

/**
 * Support for various events used to drive the game world and the plot.
 */
namespace events
{
#ifndef SWIG
    /**
     * Directory where %event scripts reside.
     */
    #define EVENTS_DIR "game_events."
#endif // SWIG

    /**
     * Base class for events. You can create your own %event types that can
     * be handled by the event_list and event_handler by inheriting from
     * this class.
     *
     * Events are used to notify when certain things happen during the game.
     * They may either execute the "run" method of an exclusive %python script
     * or a simple %python callback defined elsewhere.
     */
    class event
    {
    public:
#ifndef SWIG
        /**
         * Constructor. Needs to be called by any derived class!
         */
        event ();
#endif
        /**
         * Destructor.
         */
        virtual ~event () { }

        /**
         * @name Member access
         */
        //@{
        /**
         * Get the event's type.
         *
         * @return type of the %event
         */
        u_int8 type ();

#ifndef SWIG
        /**
         * Get name of %event.
         * @return name of %event.
         */
        virtual const char* name () const = 0;
#endif // SWIG

        /**
         * Return whether this event should be repeated.
         *
         * @return the number of times this event should be repeated or
         *      -1 in case it should be repeated unlimited times.
         */
        s_int32 repeat () const
        {
            return Repeat;
        }

        /**
         * Set whether this event should be repeated. A number greater than 0
         * will execute the event that many times, a number less than 0 will
         * repeat the event forever. A number equal to 0 won't repeat the event.
         *
         * @param count How often the event should be repeated.
         */
        void set_repeat (s_int32 count)
        {
            Repeat = count;
        }

#ifndef SWIG
        /**
         * This is called by the %event %listener when the %event has been
         * triggered to decrease the event's repeat count. If the repeat-count
         * reaches 0, the %event needs to be destroyed.
         */
        virtual void do_repeat ()
        {
            if (Repeat > 0) Repeat--;
        }
#endif // SWIG

        /**
         * Compare two events for equality.
         *
         * @param evnt pointer to the %event to compare with.
         * @return \e true if the events are equal, \e false otherwise.
         */
        virtual bool equals (const event* evnt) const = 0;

        /**
         * @name Loading / Saving
         */
        //@{

        /**
         * Saves the basic %event %data (such as the type or repeat data)
         * to a file. Call this method from the derived class.
         *
         * @param file stream where to save the %event.
         */
        virtual void put_state (base::flat& file) const;

        /**
         * Loads the basic %event %data from stream. Call this method from
         * the derived class.
         *
         * @param file flattener to load the %event from.
         * @return \e true if the %event could be loaded, \e false otherwise
         */
        virtual bool get_state (base::flat& file);
        //@}

#ifndef SWIG
        /**
         * Allow %event to be passed as python argument
         */
        GET_TYPE_NAME_VIRTUAL (events::event)
#endif

    protected:
        /**
         * @name Basic Event Data
         */
        //@{
        /**
         * Event type - see enum above.
         */
        u_int8 Type;

        /**
         * Defines how often the %event should be repeated. <b>0</b> means
         * never, <b>-1</b> means infinitely and <b>n</b> (n > 0) means
         * exactly n times.
         */
        s_int32 Repeat;
        //@}
    };
}

#endif // EVENT_EVENT_H
