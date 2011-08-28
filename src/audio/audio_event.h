/*
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
 * @file    audio/audio_event.h
 * @author  Tyler Nielsen <tyler.nielsen@gmail.com>
 *
 * @brief   Declares the audio_event class.
 */

#ifndef AUDIO_EVENT_H
#define AUDIO_EVENT_H

#include "event/event.h"
#include "audio/sound.h"

namespace audio
{
    /**
     * The audio %event executes the attached script or callback at the end
     * of a sample.
     */
    class audio_event : public events::event
    {
    public:
        /**
         * @name Initialization
         */
        //@{

        /**
         * Create a new audio %event.
         *
         * @param sample The sample to trigger on
         */
        audio_event (sound *sample): event () 
		{ 
			Sample = sample; 
		}

#ifndef SWIG
        /**
         * Create empty audio event
         */
        audio_event () { }
#endif // SWIG

        //@}

        /**
         * @name Event Handling
         */
        //@{
        /**
         * Compare two audio events for equality.
         *
         * @param e The audio event to compare this to.
         * @return \e True if the two events equal, \e false otherwise.
         */
        bool equals (const events::event * e) const
        {
            return Sample == ((const audio_event *)e)->Sample;
        }

#ifndef SWIG
        /**
         * Get name of %event.
         * @return the string "audio_event";
         */
        const char* name () const
        {
            return "audio_event";
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
         * @param file stream where to save the %event.
         */
        void put_state (base::flat& file) const;

        /**
         * Loads the basic %event %date from a stream.
         *
         * @param file stream to load the %event from.
         * @return \e true if the %event could be loaded, \e false otherwise
         */
        bool get_state (base::flat& file);

        //@}

        /**
         * Get the event's sound sample
         *
         * @return the sound sample
         */
        const sound &sample () const
        {
            return *Sample;
        }

#ifndef SWIG
        /**
         * Allow %event to be passed as python argument
         */
        GET_TYPE_NAME(audio::audio_event)
#endif

    private:
        /// time when the event shall be triggered
        sound *Sample;
    };
}

#endif // TIME_EVENT_H
