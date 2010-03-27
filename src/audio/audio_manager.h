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
 * @file   audio/audio_manager.h
 * @author Josh Glover <jmglov@jmglov.net>
 *
 * @brief  Defines the audio_manager class.
 *
 *
 */

#ifndef AUDIO_MANAGER_H_
#define AUDIO_MANAGER_H_

#include <glog/logging.h>

#include "base/configuration.h"
#include "base/types.h"


/**
 * This module gives an interface to handle sound output
 *
 */
namespace audio
{
    /**
     * @name Audio formats
     */
    static const u_int16 AUDIO_S16 = 16;

    /**
     * @name Default values for audio manager
     */
    static const int     DEFAULT_AUDIO_BUFFERS     =      4096;
    static const int     DEFAULT_AUDIO_CHANNELS    =         2;
    static const u_int16 DEFAULT_AUDIO_FORMAT      = AUDIO_S16;
    static const int     DEFAULT_AUDIO_MIXCHANNELS =        16;
    static const int     DEFAULT_AUDIO_RATE        =     44100;

    /** Audio access is made through this class.
     *  This static class manages audio system configuration.
     */
    class audio_manager
    {
    public:
        /** Gets the audio buffer size
         *  @return audio_buffers  size of audio buffer, in bytes
         */ 
        static const int get_audio_buffers() { return audio_buffers_; }
        
        /** Gets the number of audio channels
         *  @return audio_channels  number of audio channels
         */ 
        static const int get_audio_channels() { return audio_channels_; }
        
        /** Gets the audio format
         *  @return audio_format  audio format; see
         *                        http://www.libsdl.org/cgi/docwiki.cgi/SDL_AudioSpec
         */ 
        static const u_int16 get_audio_format() { return audio_format_; }

        /** Gets the number of audio mixer channels
         *  @return audio_mixchannels  number of audio mixer channels
         */ 
        static const int get_audio_mixchannels() { return audio_mixchannels_; }

        /** Gets the audio rate (in Hz)
         *  @return audio_rate  audio rate in samples per second (Hz)
         */ 
        static const int get_audio_rate() { return audio_rate_; }

        /** Sets the audio buffer size
         *  @param audio_buffers  size of audio buffer, in bytes
         */ 
        static bool set_audio_buffers(const int audio_buffers = DEFAULT_AUDIO_BUFFERS);
        
        /** Sets the number of audio channels
         *  @param audio_channels  number of audio channels
         */ 
        static bool set_audio_channels(const int audio_channels = DEFAULT_AUDIO_CHANNELS);
        
        /** Sets the audio format
         *  @param audio_format  audio format; see
         *                       http://www.libsdl.org/cgi/docwiki.cgi/SDL_AudioSpec
         */ 
        static bool set_audio_format(const u_int16 audio_format = DEFAULT_AUDIO_FORMAT);

        /** Sets the number of audio mixer channels
         *  @param audio_mixchannels  number of audio mixer channels
         */ 
        static bool set_audio_mixchannels(const int audio_mixchannels = DEFAULT_AUDIO_MIXCHANNELS);

        /** Sets the audio rate (in Hz)
         *  @param audio_rate  audio rate in samples per second (Hz)
         */ 
        static bool set_audio_rate(const int audio_rate = DEFAULT_AUDIO_RATE);

    protected:
        static int     audio_rate_;
        static u_int16 audio_format_;
        static int     audio_channels_;
        static int     audio_buffers_;
        static int     audio_mixchannels_;

    }; // class{}

} // namespace{}

#endif
