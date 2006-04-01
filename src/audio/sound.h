/*
   $Id: sound.h,v 1.2 2006/04/01 22:45:54 Mithander Exp $

   Copyright (C) 1999/2000/2001/2002   Alexandre Courbot <alexandrecourbot@linuxgames.com>
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
 * @file   audio/sound.h
 * @author Tyler Nielsen <tyler.nielsen@gmail.com>
 *
 * @brief  The sound class for programs using audio.
 */

#ifndef AUDIO_SOUND_H_
#define AUDIO_SOUND_H_

#include <string>
#include <vector>

namespace audio {

    class sound
    {
    public:
        /**
         * Constuctor
         */
        sound (const std::string &filename);

        /**
         * Get the filename associated with the sound
         *
         * @return \e filename
         */
        const std::string &getfilename(void) {return m_filename;}

        /**
         * Play this sound
         *
         * @param loops number of times to repeat.  -1 means repeat forever
         * @return \e true if sound was successfully played
         */
        bool play (int loops = 0);

        /**
         * Fade the sound in
         *
         * @param sec time in seconds to spread the fade in over
         * @param loops number of times to repeat.  -1 means repeat forever
         * @return \e true if sound was successfully played
         */
        bool fadein(double sec, int loops = 0);

        /**
         * Set the position for the channel
         *
         * @param angle angle to position the sound 0=in front, -90/270=to left, 90=to right, 180=behind
         * @param distance relative distance to sound 0=close 1=far
         * @return \e true if position was successfully set
         */
        bool setposition(int angle, double distance);

        /**
         * Fade the channel out
         *
         * @param sec time in seconds to spread the fade in over
         */
        void fadeout(double sec);

        /**
         * Halt the sound
         */
        void halt(void);

        /**
         * Destructor: free all memory
         */
        virtual ~sound ();

#ifndef SWIG
        /**
         * Called by the library whenever a channel stops playing
         *
         * @param channel channel that stopped
         */
        static void handle_channel_stop(int channel);
#endif
    protected:

        std::string m_filename; // the name of the file we loaded
        void * m_sample; // the sound data
        int m_channel; // the channel the sound it playing on
        bool m_forcedhalt; //Was this sound stoped by the user

        //Functions from the audio backend
        static void *(*m_open)(const char *);
        static void (*m_close)(void *);
        static int (*m_play)(void *, int);
        static int (*m_fadein)(void *, double, int);
        static void (*m_halt)(int);
        static void (*m_fadeout)(int, double);
        static bool (*m_setpos)(int, int, double);

        //This lets us know which channel is associated with which class.
        static std::vector<sound *> m_channels;

        bool handle_channel_create(void);

        friend bool audio::init(const std::string &);
    };
}

#endif
