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
 * @file   audio/audio_manager.cc
 * @author Josh Glover <jmglov@jmglov.net>
 * 
 * @brief  Defines the audio_manager class.
 * 
 * 
 */


#include "audio/audio_manager.h"

namespace audio
{
    // Static data must be initialised
    int     audio_manager::audio_buffers_;
    int     audio_manager::audio_channels_;
    u_int16 audio_manager::audio_format_;
    int     audio_manager::audio_mixchannels_;
    int     audio_manager::audio_rate_;


    bool audio_manager::set_audio_buffers(const int audio_buffers) {
        LOG(INFO) << "set_audio_buffers(" << audio_buffers << "); was: "
                  << audio_buffers_;

        audio_buffers_ = audio_buffers;

        return true;
    }


    bool audio_manager::set_audio_channels(const int audio_channels) {
        LOG(INFO) << "set_audio_channels(" << audio_channels << "); was: "
                  << audio_channels_;

        audio_channels_ = audio_channels;

        return true;
    }


    bool audio_manager::set_audio_format(const u_int16 audio_format) {
        LOG(INFO) << "set_audio_format(" << audio_format << "); was: "
                  << audio_format_;

        audio_format_ = audio_format;

        return true;
    }


    bool audio_manager::set_audio_mixchannels(const int audio_mixchannels) {
        LOG(INFO) << "set_audio_mixchannels(" << audio_mixchannels << "); was: "
                  << audio_mixchannels_;

        audio_mixchannels_ = audio_mixchannels;

        return true;
    }


    bool audio_manager::set_audio_rate(const int audio_rate) {
        LOG(INFO) << "set_audio_rate(" << audio_rate << "); was: "
                  << audio_rate_;

        audio_rate_ = audio_rate;

        return true;
    }

} // namespace{}
