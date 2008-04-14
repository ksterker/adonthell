/*
   $Id: audio_event_manager.h,v 1.4 2008/04/14 11:06:40 ksterker Exp $

   Copyright (C) 2005 Tyler Nielsen <tyler.nielsen@gmail.com>
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
 * @file    audio/audio_event_manager.h
 * @author  Tyler Nielsen <tyler.nielsen@gmail.com>
 *
 * @brief   Declares the audio_event_manager class.
 */

#ifndef EVENT_AUDIO_EVENT_MANAGER_H
#define EVENT_AUDIO_EVENT_MANAGER_H

#include "event/manager_base.h"
#include <vector>

using events::manager_base;
using events::listener;
using events::event;

namespace audio
{
    /**
     * This class keeps track of audio events, i.e. events that are raised
     * at the end of a sound.
     */
    class audio_event_manager : public manager_base
    {
    public:
        /**
         * Create a new time_event_manager and register with the event handling
         * system.
         */
        audio_event_manager ();

        /**
         * Register a %listener with the %event manager. It is inserted
         * into the vector of registered listeners depending on its "alarm"
         * time. The %listener needs to be removed before it can be safely
         * deleted.
         *
         * @param li Pointer to the %listener to be registered.
         */
        void add (listener *li);

        /**
         * Removes the given %listener from the %event manager. Once it is
         * no longer needed, it can be freed.
         *
         * @param li Pointer to the %listener to be removed.
         */
        void remove (listener *li);

        /**
         * Raise one or more events in case the given time matches their
         * "sound" pointer.
         *
         * @param e An %event structure with the current audio clip
         */
        void raise_event (const event *e);

        /**
         * Register %time events with %event subsystem.
         */
        static void init ();

        /**
         * Remove %time events from %event subsystem.
         */
        static void cleanup ();

    private:
        /// storage for registered listeners.
        std::vector<listener*> Listeners;
    };
}

#endif // EVENT_TIME_EVENT_MANAGER_H
