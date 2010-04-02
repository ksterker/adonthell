/*
   Copyright (C) 2010 Josh Glover <jmglov@jmglov.net>
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
 * @file   test/audio/sound.h
 * @author Josh Glover <jmglov@jmglov.net>
 * 
 * @brief  Defines a sound_noop{} class for unit tests
 * 
 * 
 */


#include "audio/sound.h"

// No-op to allow constructor to succeed
void *audio_open(const char *filename) {
    return NULL;
}

namespace audio
{
    class sound_noop : public sound {
    public:
        static void init() {
            sound::m_open = audio_open;
        }

        sound_noop(const std::string &filename) : sound(filename) {}
        sound_noop() {}

        void * get_sample()     { return m_sample;     }
        int    get_channel()    { return m_channel;    }
        bool   get_forcedhalt() { return m_forcedhalt; }
    };
} // namespace{}
