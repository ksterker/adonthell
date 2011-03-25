/*
   Copyright (C) 2005 Tyler Nielsen <tyler.nielsen@gmail.com>
   Copyright (C) 2010 Josh Glover   <jmglov@jmglov.net>
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
 * @file   audio/sound.cc
 * @author Tyler Nielsen <tyler.nielsen@gmail.com>
 * @author Josh Glover   <jmglov@jmglov.net>
 *
 * @brief  The sound class for programs using audio.
 */

#include <string>

#include "base/base.h"
#include "audio/audio.h"
#include "audio/audio_manager.h"
#include "audio/sound.h"

using namespace audio;

void *(*sound::m_open)(const char *) = NULL;
void (*sound::m_close)(void *) = NULL;
int (*sound::m_play)(void *, int) = NULL;
int (*sound::m_fadein)(void *, double, int) = NULL;
void (*sound::m_halt)(int) = NULL;
void (*sound::m_fadeout)(int, double) = NULL;
bool (*sound::m_setpos)(int, int, double) = NULL;

std::vector<sound *> sound::m_channels;

sound::sound (const std::string &filename)
{
    LOG(INFO) << logging::indent() << "sound::sound(" << filename << ") called";
    logging::increment_log_indent_level();

    m_filename = "audio/" + filename;
    if (base::Paths().find_in_path (m_filename))
    {
        open_file();
    }
    else
    {
        m_channel = -1;
        m_sample = NULL;
    }
    
    logging::decrement_log_indent_level();
}

bool sound::play (int loops)
{
    if(m_channel != -1)
        halt();
    if(m_sample)
        m_channel = m_play (m_sample, loops);
    return handle_channel_create();
}

bool sound::fadein (double sec, int loops)
{
    if(m_channel != -1)
        halt();
    if(m_sample)
        m_channel = m_fadein (m_sample, sec, loops);
    return handle_channel_create();
}

bool sound::setposition (int angle, double distance)
{
    if(m_channel == -1)
        return false;
    return m_setpos (m_channel, angle, distance);
}

void sound::fadeout (double sec)
{
    if(m_channel != -1) {
        m_forcedhalt = true;
        m_fadeout (m_channel, sec);
    }
}

void sound::halt (void)
{
    if(m_channel != -1) {
        m_forcedhalt = true;
        m_halt (m_channel);
    }
}

void sound::handle_channel_stop (int channel)
{
    //FIXME  We can get called twice if there is a fadeout that ends after the end of the sound.
    if(m_channels[channel] == NULL)
    {
        return;
    }

    m_channels[channel]->m_channel = -1;
    //Only issue callbacks for sounds that end on their own.
    if(!m_channels[channel]->m_forcedhalt)
        complete(m_channels[channel]);
    m_channels[channel]->m_forcedhalt = false;
    m_channels[channel] = NULL;
}

sound::~sound ()
{
    if(m_sample)
        m_close (m_sample);
    m_sample = NULL;
}

bool sound::handle_channel_create (void)
{
    if(m_channel>=0)
    {
        //The call was successful, add to m_channels list;
        if(m_channels.size() <= (unsigned int) m_channel)
            m_channels.resize(m_channel + 1);
        m_channels[m_channel] = this;
        return true;
    }
    return false;
}

void sound::put_state (base::flat & file) const
{
    LOG(INFO) << logging::indent() << "sound::put_state() called";
    logging::increment_log_indent_level();

    file.put_string("filename", m_filename);

    log_state();

    logging::decrement_log_indent_level();
}

bool sound::get_state (base::flat & file)
{
    LOG(INFO) << logging::indent() << "sound::get_state() called";
    logging::increment_log_indent_level();

    m_filename = file.get_string("filename");

    open_file();
    log_state();

    logging::decrement_log_indent_level();

    return file.success ();
}

void sound::open_file (void)
{
    LOG(INFO) << logging::indent() << "sound::open_file() called";
    logging::increment_log_indent_level();

    LOG(INFO) << logging::indent() << "Opening sound file: '" << m_filename << "'...";
    m_sample     = m_open (m_filename.c_str());
    m_channel    = -1;
    m_forcedhalt = false;
    LOG(INFO) << logging::indent() << "Done!";

    logging::increment_log_indent_level();
}

void sound::log_state (void) const
{
    LOG(INFO) << logging::indent() << "m_filename: "   << m_filename;
    LOG(INFO) << logging::indent() << "m_sample: "     << m_sample;
    LOG(INFO) << logging::indent() << "m_channel: "    << m_channel;
    LOG(INFO) << logging::indent() << "m_forcedhalt: " << m_forcedhalt;
}
