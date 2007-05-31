/*
   $Id: animation.cc,v 1.8 2007/05/31 05:54:33 ksterker Exp $

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
 * @file gfx/animation.cc
 *
 * @author Tyler Nielsen <tyler.nielsen@gmail.com>
 *
 * @brief  Defines the animation global interface.
 */

#include "animation.h"
#include "base/diskio.h"

using namespace std;

namespace gfx
{
    // static storage
    animation::animation_cache animation::m_allAnimations;

    // change animation being played
    bool animation::change_animation (const std::string & new_animation)
    {
        //Check if the animation is valid yet
        if(!m_valid) return false;

        //Look for an animation with ne name passed in
        animation_map::iterator anim = m_sprite->second.find(new_animation);
        if(anim == m_sprite->second.end())
            return false;

        //we found it so update our pointers
        m_animation = anim;
        m_surface = m_animation->second.begin();

        return true;
    }

    // update state of animation
    bool animation::update ()
    {
        //Check if the animation is valid yet
        if(!m_valid) return false;

        if (m_playing)
        {
            //TODO deal with delay here.

            //Update to the next surface.  If it wraps, then reset it to the beginning
            m_surface++;
            if(m_surface == m_animation->second.end())
                m_surface = m_animation->second.begin();
        }
        
        return true;
    }

    // start playing animation
    void animation::play ()
    {
        m_playing = true;
    }
    
    // pause playing animation
    void animation::stop ()
    {
        m_playing = false;
    }
    
    // reset to first frame
    void animation::rewind ()
    {
        m_delay = 0;
        m_surface = m_animation->second.begin();
    }
    
    // load from stream
    bool animation::get_state (base::flat & file)
    {
        s_int32 size;
        void *value;
        char *id;

        while (file.next (&value, &size, &id) == base::flat::T_FLAT)
        {
            base::flat anim = base::flat ((const char*) value, size);
            string animation_name = id;
            animation_list cur_animation;

            while (anim.next (&value, &size, &id) == base::flat::T_FLAT) 
            {
                base::flat frame = base::flat((const char*) value, size);

                cur_animation.push_back(new animation_frame(surface_cache(id, frame.get_bool("mask"), frame.get_bool("mirrored_x")), frame.get_uint32("delay")));
            }

            m_sprite->second[animation_name] = cur_animation;
        }

        return file.success ();
    }
    
    // load from XML file
    bool animation::load_animation (const std::string & filename)
    {
        //Check if we have already loaded the file
        m_sprite = m_allAnimations.find(filename);
        if (m_sprite != m_allAnimations.end()) {
            m_valid = true;
            return m_valid;
        }

        //We have not loaded it before so load it from the file
        base::diskio animation (base::diskio::XML_FILE);

        if (!animation.get_record (filename))
            //Error loading the file (file not found?)
            return false;

        //Sprite doesn't exist so create a blank one
        animation_map sprite;
        m_allAnimations[filename] = sprite;
        m_sprite = m_allAnimations.find(filename);

        //This will populate the sprite that we just created
        bool retval = get_state (animation);

        m_animation = m_sprite->second.find("stand_east");  //TODO This should be part of the XML File, not hardcoded here
        m_surface = m_animation->second.begin();

        set_length((*m_surface)->image->length());          //TODO This should be part of the XML File, not hardcoded here
        set_height((*m_surface)->image->height());          //TODO This should be part of the XML File, not hardcoded here

        m_valid = true;
        m_playing = true;                                   //TODO This should be part of the XML File, not hardcoded here

        return retval;
    }

    // save to stream
    bool animation::put_state (base::flat & file) const
    {
        animation_map::iterator ii;

        //Loop through all available animations
        for(ii = m_sprite->second.begin(); ii != m_sprite->second.end(); ii++) {
            base::flat anim;
            animation_list::iterator jj;

            //Loop through all frames in current animation
            for(jj = ii->second.begin(); jj != ii->second.end(); jj++) {
                base::flat frame;

                //Add information about the frame.
                frame.put_bool("mask", (*jj)->image->is_masked());
                frame.put_bool("mirrored_x", (*jj)->image->is_mirrored_x());
                frame.put_uint32("delay", (*jj)->delay);

                //Add the frame to the animation
                anim.put_flat((*jj)->image->filename(), frame);
            }

            //Add the current animation to the diskio
            file.put_flat(ii->first, anim);
        }

        return true;
    }

    // save to XML file
    bool animation::save_animation (const std::string & filename) const
    {
        // create container
        base::diskio animation (base::diskio::XML_FILE);

        // dump to container
        put_state (animation);
        
        // dump it out to a file
        return animation.put_record (filename);
    }
    
    // get filename of sprite
    std::string animation::filename() const
    {
    	if (m_valid)
    	{
    		return m_sprite->first;
    	}
    	return "";
    }
}
