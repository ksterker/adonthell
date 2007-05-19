/*
   $Id: animation.cc,v 1.4 2007/05/19 07:42:07 ksterker Exp $

   Copyright (C) 1999/2000/2001/2002/2003   Alexandre Courbot <alexandrecourbot@linuxgames.com>
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
    //Static storage
    animation::animation_cache animation::m_allAnimations;

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

    void animation::play ()
    {
        m_playing = true;
    }
    
    void animation::stop ()
    {
        m_playing = false;
    }
    
    void animation::rewind ()
    {
        m_surface = m_animation->second.begin();
    }
    
    bool animation::load_animation (const std::string & filename)
    {
        animation_map sprite;
        animation_list anim;
        animation_frame *cur;
        int idx;

        //Check if we have already loaded the file
        m_sprite = m_allAnimations.find(filename);
        if (m_sprite != m_allAnimations.end()) {
            m_valid = true;
            return m_valid;
        }


        // TODO Load xml file here. 

        //For now we hard code an animation until save/load is complete
        char Buffer[100];
        anim.clear();
        cur = new animation_frame(surface_cache("gfx/character/npc/naked_guy/east_mov1.png"));
        anim.push_back(cur);
        sprite["stand_east"] = anim;
        anim.clear();
        for(idx = 0; idx < 6; idx ++) {
            sprintf(Buffer, "gfx/character/npc/naked_guy/east_mov%d.png", idx+1);
            cur = new animation_frame(surface_cache(Buffer), 0);
            anim.push_back(cur);
        }
        sprite["walk_east"] = anim;

        anim.clear();
        cur = new animation_frame(surface_cache("gfx/character/npc/naked_guy/east_mov1.png", true, true));
        anim.push_back(cur);
        sprite["stand_west"] = anim;
        anim.clear();
        for(idx = 0; idx < 6; idx ++) {
            sprintf(Buffer, "gfx/character/npc/naked_guy/east_mov%d.png", idx+1);
            cur = new animation_frame(surface_cache(Buffer, true, true), 0);
            anim.push_back(cur);
        }
        sprite["walk_west"] = anim;

        anim.clear();
        cur = new animation_frame(surface_cache("gfx/character/npc/naked_guy/south_mov1.png"));
        anim.push_back(cur);
        sprite["stand_south"] = anim;
        anim.clear();
        for(idx = 0; idx < 6; idx ++) {
            sprintf(Buffer, "gfx/character/npc/naked_guy/south_mov%d.png", idx+1);
            cur = new animation_frame(surface_cache(Buffer), 0);
            anim.push_back(cur);
        }
        sprite["walk_south"] = anim;

        anim.clear();
        cur = new animation_frame(surface_cache("gfx/character/npc/naked_guy/north_mov1.png"));
        anim.push_back(cur);
        sprite["stand_north"] = anim;
        anim.clear();
        for(idx = 0; idx < 6; idx ++) {
            sprintf(Buffer, "gfx/character/npc/naked_guy/north_mov%d.png", idx+1);
            cur = new animation_frame(surface_cache(Buffer), 0);
            anim.push_back(cur);
        }
        sprite["walk_north"] = anim;

        m_allAnimations[filename] = sprite;
        m_sprite = m_allAnimations.find(filename);
        m_animation = m_sprite->second.find("stand_east");
        m_surface = m_animation->second.begin();

        set_length((*m_surface)->image->length());
        set_height((*m_surface)->image->height());

        m_valid = true;

        return m_valid;
    }

    bool animation::save_animation (const std::string & filename)
    {
        base::diskio animation (base::diskio::XML_FILE);
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
            animation.put_flat(ii->first, anim);
        }

        //Dump it out to a file
        animation.put_record(filename);
    }
}
