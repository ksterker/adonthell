/*
   Copyright (C) 1999/2000/2001/2002/2003 Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Copyright (C) 2006/2007 Tyler Nielsen <tyler.nielsen@gmail.com>
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
 * @file  gfx/sprite.cc
 *
 * @author Tyler Nielsen <tyler.nielsen@gmail.com>
 *
 * @brief  Defines the sprite global interface.
 */

#include "gfx/sprite.h"
#include "base/base.h"
#include "base/diskio.h"
#include "event/date.h"
#include "event/time_event.h"

using namespace std;

namespace gfx
{
    // ctor
    sprite::sprite () : m_valid(false), m_playing(false) 
    {
        m_listener = new events::listener_cxx (NULL, new events::time_event ());
        m_listener->connect_callback (base::make_functor (*this, &sprite::update));
        m_listener->set_id ("animation");
        m_listener->pause ();
    }
    
    // dtor
    sprite::~sprite ()
    {
        clear();
        delete m_listener;
    }
    
    // reset sprite
    void sprite::clear ()
    {
        for (animation_map::iterator i = m_states.begin(); i != m_states.end(); i++)
        {
            for (animation_list::const_iterator j = i->second.begin(); j != i->second.end(); j++) 
            {
                delete *j;
            }                
        }
        m_states.clear ();
        m_valid = false;
        m_playing = false;
        if (!m_listener->is_paused()) 
        {
            m_listener->pause();
        }
    }
    
    // change animation being played
    bool sprite::change_animation (const std::string & new_animation)
    {
        //Check if the animation is valid yet
        if(!m_valid) return false;

        // Look for an animation with the name passed in
        animation_map::iterator anim = m_states.find(new_animation);
        if(anim == m_states.end())
            return false;

        // stop playing current animation
        if (m_playing)
        {
            stop ();
        }
        
        // we found it so update our pointers
        m_animation = anim;
        m_surface = m_animation->second.begin();

        // update length and height of drawable
        set_length ((*m_surface)->image->s->length());
        set_height ((*m_surface)->image->s->height());

        // set delay until next frame
        events::time_event *evt = (events::time_event *) m_listener->get_event ();
        evt->set_time (events::date::convert_millis ((*m_surface)->delay) + events::date::time ());
        
        return true;
    }

    // update state of animation
    bool sprite::update (const events::event *evt)
    {
        // Check if the animation is valid yet
        if (!m_valid) return false;

        if (m_playing)
        {
            // stop animation?
            if ((*m_surface)->delay == 0)
            {
                m_playing = false;
                return true;
            }
            
            // Update to the next surface.  If it wraps, then reset it to the beginning
            m_surface++;
            if (m_surface == m_animation->second.end())
                m_surface = m_animation->second.begin();
        
            // set delay until next frame
            events::time_event *evt = (events::time_event *) m_listener->get_event ();
            evt->set_time (events::date::convert_millis ((*m_surface)->delay) + events::date::time ());
        }
        
        return true;
    }

    // start playing animation
    void sprite::play ()
    {
        m_playing = true;
        if (m_valid && m_animation->second.size() > 1)
            m_listener->resume ();
    }
    
    // pause playing animation
    void sprite::stop ()
    {
        m_playing = false;
        if (m_valid && m_animation->second.size() > 1 && !m_listener->is_paused ())
            m_listener->pause ();
    }
    
    // reset to first frame
    void sprite::rewind ()
    {
        if (m_valid)
            m_surface = m_animation->second.begin();
    }
    
    // load from stream
    bool sprite::get_state (base::flat & file)
    {
        u_int32 size;
        void *value;
        char *id;

        while (file.next (&value, &size, &id) == base::flat::T_FLAT)
        {
            base::flat anim = base::flat ((const char*) value, size);
            string animation_name = id;
            animation_list cur_animation;

            while (anim.next (&value, &size, &id) == base::flat::T_FLAT) 
            {
                base::flat frame ((const char*) value, size);
                cur_animation.push_back(new animation_frame(surfaces->get_surface(id, frame.get_bool("mask"), frame.get_bool("mirrored_x"), frame.get_bool("mirrored_y")), frame.get_uint32("delay")));
            }
            
            m_states[animation_name] = cur_animation;
        }

        return file.success ();
    }
    
    // load from XML file
    bool sprite::load (const std::string & filename)
    {
        const std::string & file = (filename.length() != 0) ? filename : m_filename;
        bool retval = false;
        
        // load raw png as one animation, one frame sprite
        if (file.find (".png", file.size() - 4) != std::string::npos)
        {
            std::string full_path (file);
            if (base::Paths().find_in_path (full_path))
            {
                animation_list cur_animation;
                cur_animation.push_back (new animation_frame (surfaces->get_surface (full_path, false, false), 0));
                m_states["default"] = cur_animation;
                retval = true;
            }
        }
        else
        {
            // load sprite from the file
            base::diskio animation (base::diskio::XML_FILE);
            if (!animation.get_record (file))
            {
                //Error loading the file (file not found?)
                return false;
            }
        
            // This will populate the sprite that we just created
            retval = get_state (animation);
        }
    
        if (retval)
        {
            m_animation = m_states.begin(); // TODO This should be part of the XML File, not hardcoded here
            m_surface = m_animation->second.begin();

            set_length((*m_surface)->image->s->length());          
            set_height((*m_surface)->image->s->height());          
            
            m_valid = true;
            m_filename = file;
        }
        
        return retval;
    }

    // save to stream
    bool sprite::put_state (base::flat & file) const
    {
        animation_map::const_iterator ii;

        //Loop through all available animations
        for(ii = m_states.begin(); ii != m_states.end(); ii++) {
            base::flat anim;
            animation_list::const_iterator jj;

            //Loop through all frames in current animation
            for(jj = ii->second.begin(); jj != ii->second.end(); jj++) {
                base::flat frame;

                //Add information about the frame.
                frame.put_bool("mask", (*jj)->image->s->is_masked());
                frame.put_bool("mirrored_x", (*jj)->image->s->is_mirrored_x());
                frame.put_uint32("delay", (*jj)->delay);

                //Add the frame to the animation
                anim.put_flat((*jj)->image->s->filename(), frame);
            }

            //Add the current animation to the diskio
            file.put_flat(ii->first, anim);
        }

        return true;
    }

    // save to XML file
    bool sprite::save (const std::string & filename) const
    {
        // no need to save simple "sprites"
        if (filename.find (".png", filename.size() - 4) == std::string::npos)
        {
            // create container
            base::diskio animation (base::diskio::XML_FILE);

            // dump to container
            put_state (animation);
            
            // dump it out to a file
            return animation.put_record (filename);
        }
        else
        {
            // simple sprite, skip successfully
            return true;
        }
    }
    
    // get filename of sprite
    std::string sprite::filename() const
    {
  		return m_filename;
    }
}
