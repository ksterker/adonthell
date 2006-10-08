/*
   $Id: audio_sdl.cc,v 1.4 2006/10/08 12:38:08 gnurou Exp $

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

#ifdef USE_LIBTOOL
/* exported names for libltdl */
#define audio_init sdl_LTX_audio_init
#define audio_cleanup sdl_LTX_audio_cleanup
#define audio_open sdl_LTX_audio_open
#define audio_close sdl_LTX_audio_close
#define audio_play sdl_LTX_audio_play
#endif

#include <iostream>
#include <map>

#include "SDL.h"
#include "SDL_mixer.h"

#include "audio/sound.h"
#include "audio/audio.h"

//TODO These values should be gotten from a config file or something
int audio_rate = 44100;
Uint16 audio_format = AUDIO_S16;
int audio_channels = 2;
int audio_buffers = 4096;
int audio_mixchannels = 16;

//This is the structure that we return back.  This wraps the sound to help with caching
struct Audio_Chunk
{
    Mix_Chunk *sample;
    int refcount;

    //Constructor
    Audio_Chunk (Mix_Chunk *sound) : sample(sound), refcount(0) { }
};

//typedef for the cache of sounds
//TODO:  This is a case sensitive map of filename to sample.
//       Under windows the filename is not case sensitive, so we may need to
//       change how this works.
typedef std::map<std::string, Audio_Chunk *> cachemap_t;

extern "C"
{
    bool audio_init();                                      //Initialize library
    void audio_cleanup();                                   //Close library

    void *audio_open(const char *filename);                 //Open a sound... if it is already open, return a referance
    void audio_close(void *sound);                          //Close a sound... if it is the last sound, free the memory
    int audio_play(void *sample, int loops);                //Play a sound... return the channel that the sound is playing on
    int audio_fadein(void *param, double sec, int loops);   //Fade a sound in... return the channel that the sound is playing on
    void audio_halt(int channel);                           //Halt a channel
    void audio_fadeout(int channel, double sec);            //Fade a channel out... over ms milliseconds
    bool audio_setposition(int channel, int angle, double distance); //Set position for a channel.
}

//This is the cache of the samples
static cachemap_t cachemap;

void channel_stopped(int channel)
{
    audio::sound::handle_channel_stop(channel);
}

bool audio_init()
{
    if (SDL_InitSubSystem (SDL_INIT_AUDIO) < 0)
    {
        std::cerr << "Couldn't init audio: " << SDL_GetError () << std::endl;
        return false;
    }
    if (Mix_OpenAudio (audio_rate, audio_format, audio_channels, audio_buffers) < 0) {
        std::cerr << "Couldn't init audio: " << Mix_GetError () << std::endl;
        return false;
    }
    Mix_ChannelFinished(channel_stopped);
    Mix_AllocateChannels(audio_mixchannels);
    return true;
}

void audio_cleanup()
{
    Mix_CloseAudio();
    SDL_QuitSubSystem (SDL_INIT_AUDIO);
}

//filename is clip to open
//returns audio_clip
void *audio_open(const char *filename)
{
    cachemap_t::iterator idx = cachemap.find(filename);

    if(idx == cachemap.end())
    {
        //We did not find the sample in the cache
        Mix_Chunk *sample = Mix_LoadWAV(filename);
        if (sample == NULL)
        {
            std::cerr << "Couldn't open file '" << filename << "': " << Mix_GetError () << std::endl;
            return NULL;
        }
        Audio_Chunk *audio = new Audio_Chunk(sample);
        cachemap[filename] = audio;
        idx = cachemap.find(filename);
    }

    idx->second->refcount++;

    return idx->second;
}

//param is audio_clip returned from audio_open
void audio_close(void *param)
{
    Audio_Chunk *audio = (Audio_Chunk *)param;
    audio->refcount--;
    if(audio->refcount <=0 )
    {
        //No more referances to this sample.. Free the memory
        //TODO:  Need to make sure sound is not playing anywhere
        Mix_FreeChunk(audio->sample);
        cachemap_t::iterator idx;
        for(idx = cachemap.begin(); idx != cachemap.end(); idx++)
        {
            if(idx->second == audio) {
                //Found the correct map entry
                cachemap.erase(idx);
                delete audio;
                return;
            }
        }
        std::cerr << "Couldn't find audio_chunk: " << param << " in the map" << std::endl;
    }
}

//param is audio_clip returned from audio_open
//loops is number of additional times to play (-1 = forever)
//returns channel sound is playing on or -1 for error
int audio_play(void *param, int loops)
{
    Audio_Chunk *audio = (Audio_Chunk *)param;
    int retval = Mix_PlayChannel(-1, audio->sample, loops);
    if(retval == -1)
    {
        // may be critical error, or maybe just no channels were free.
        // you could allocated another channel in that case...
        std::cerr << "Mix_PlayChannel: " << Mix_GetError() << std::endl;
    }
    return retval;
}

//param is audio_clip returned from audio_open
//ms is number of milliseconds to fade in over
//loops is number of additional times to play (-1 = forever)
//returns channel sound is playing on or -1 for error
int audio_fadein(void *param, double sec, int loops)
{
    Audio_Chunk *audio = (Audio_Chunk *)param;
    int retval = Mix_FadeInChannel(-1, audio->sample, loops, int(sec*1000));
    if(retval == -1)
    {
        // may be critical error, or maybe just no channels were free.
        // you could allocated another channel in that case...
        std::cerr << "Mix_FadeInChannel: " << Mix_GetError() << std::endl;
    }
    return retval;
}

//stop a channel
void audio_halt(int channel)
{
    Mix_HaltChannel(channel);
}

//fade out a channel over sec seconds
void audio_fadeout(int channel, double sec)
{
    Mix_FadeOutChannel(channel, int(sec*1000));
}

//set position on a channel
//angle is angle in degrees 0=forward, 90=right, 180=behind, 270=left
//distance is how far 0=close 1=far
bool audio_setposition(int channel, int angle, double distance)
{
    int dist = int(distance*255);
    int retval = Mix_SetPosition(channel, angle, dist);
    if(retval == 0)
    {
        std::cerr << "Mix_SetPosition: " << Mix_GetError() << std::endl;
        return false;
    }
    return true;
}
