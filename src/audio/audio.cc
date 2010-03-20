/*
   $Id: audio.cc,v 1.6 2007/05/14 02:00:04 ksterker Exp $

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
 * @file   audio/audio.cc
 * @author Tyler Nielsen <tyler.nielsen@gmail.com>
 *
 * @brief  Defines the audio module init and cleanup functions.
 *
 *
 */

#include "event/manager.h"
#include "audio/audio.h"
#include "audio/audio_manager.h"
#include "audio/sound.h"
#include "audio/audio_event.h"
#include "base/paths.h"

#include <iostream>
#include <deque>

/**
 * The handler of our library file.
 *
 */
static lt_dlhandle dlhandle = 0;

/**
 * Virtual backend init function.
 *
 */
static bool (*audioinit)() = 0;

/**
 * Virtual backend cleanup function.
 *
 */
static void (*audiocleanup)() = 0;

namespace audio
{
    /**
     * List of sounds that have completed after the last call to update
     */
    static std::deque<sound *> completed_sounds;

    // initialize audio module
    bool init(const std::string & backend_name)
    {
        if (lt_dlinit())
        {
            std::cerr << lt_dlerror() << std::endl;
            std::cerr << "Error initializing liblt!\n";
            return false;
        }

        dlhandle = base::get_module(std::string("/audio/_") + backend_name);

        if (!dlhandle) goto bigerror;

        audioinit = (bool(*)()) lt_dlsym(dlhandle, "audio_init");
        if (!audioinit)
        {
            std::cerr << lt_dlerror() << std::endl;
            goto bigerror;
        }

        audiocleanup = (void(*)()) lt_dlsym(dlhandle, "audio_cleanup");
        if (!audiocleanup)
        {
            std::cerr << lt_dlerror() << std::endl;
            goto bigerror;
        }

        audio::sound::m_open = (void *(*)(const char *)) lt_dlsym(dlhandle, "audio_open");
        if (!audio::sound::m_open)
        {
            std::cerr << lt_dlerror() << std::endl;
            goto bigerror;
        }

        audio::sound::m_close = (void(*)(void *)) lt_dlsym(dlhandle, "audio_close");
        if (!audio::sound::m_close)
        {
            std::cerr << lt_dlerror() << std::endl;
            goto bigerror;
        }

        audio::sound::m_play = (int(*)(void *, int)) lt_dlsym(dlhandle, "audio_play");
        if (!audio::sound::m_play)
        {
            std::cerr << lt_dlerror() << std::endl;
            goto bigerror;
        }

        audio::sound::m_fadein = (int(*)(void *, double, int)) lt_dlsym(dlhandle, "audio_fadein");
        if (!audio::sound::m_fadein)
        {
            std::cerr << lt_dlerror() << std::endl;
            goto bigerror;
        }

        audio::sound::m_halt = (void (*)(int)) lt_dlsym(dlhandle, "audio_halt");
        if (!audio::sound::m_halt)
        {
            std::cerr << lt_dlerror() << std::endl;
            goto bigerror;
        }

        audio::sound::m_fadeout = (void (*)(int, double)) lt_dlsym(dlhandle, "audio_fadeout");
        if (!audio::sound::m_fadeout)
        {
            std::cerr << lt_dlerror() << std::endl;
            goto bigerror;
        }

        audio::sound::m_setpos = (bool (*)(int, int, double)) lt_dlsym(dlhandle, "audio_setposition");
        if (!audio::sound::m_setpos)
        {
            std::cerr << lt_dlerror() << std::endl;
            goto bigerror;
        }

        goto success;

    bigerror:
        if (dlhandle) lt_dlclose(dlhandle);
        lt_dlexit();
        return false;

    success:
        return audioinit();
    }

    // setup from configuration
    void setup (base::configuration & cfg)
    {
        audio_manager::set_audio_buffers (
            cfg.get_int ("Audio", "BufferSize", DEFAULT_AUDIO_BUFFERS)
        );
        audio_manager::set_audio_channels (
            cfg.get_int ("Audio", "Channels", DEFAULT_AUDIO_CHANNELS)
        );
        audio_manager::set_audio_format (
            cfg.get_int ("Audio", "Format", DEFAULT_AUDIO_FORMAT)
        );
        audio_manager::set_audio_mixchannels (
            cfg.get_int ("Audio", "Mixchannels", DEFAULT_AUDIO_MIXCHANNELS)
        );
        audio_manager::set_audio_rate (
            cfg.get_int ("Audio", "Rate", DEFAULT_AUDIO_RATE)
        );
    }
    
    // shutdown audio
    void cleanup ()
    {
        if (audiocleanup) audiocleanup();
        audiocleanup = NULL;

        if (dlhandle) lt_dlclose(dlhandle);
        lt_dlexit();
    }

    void complete(sound *sample)
    {
        completed_sounds.push_back(sample);
    }

    void update(void)
    {
        while(completed_sounds.size() > 0)
        {
            audio_event evt (completed_sounds.front());
            events::manager::raise_event (&evt);
            completed_sounds.pop_front();
        }
    }
}
