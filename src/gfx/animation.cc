/*
   $Id: animation.cc,v 1.1 2006/06/03 04:20:27 Mithander Exp $

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

using namespace std;

namespace gfx
{
    //Static storage
    animation::animation_cache animation::m_allAnimations;

    bool animation::change_animation (const std::string & new_animation)
    {
        if(!m_valid) return false;

        animation_map::iterator anim = m_sprite->second.find(new_animation);
        if(anim == m_sprite->second.end())
            return false;

        m_animation = anim;
        m_surface = m_animation->second.begin();

        m_x_offset += (*m_surface)->x_offset;
        m_y_offset += (*m_surface)->y_offset;

        return true;
    }

    bool animation::update ()
    {
        if(!m_valid) return false;

        m_surface++;
        if(m_surface == m_animation->second.end())
            m_surface = m_animation->second.begin();

        m_x_offset += (*m_surface)->x_offset;
        m_y_offset += (*m_surface)->y_offset;

        return true;
    }

    bool animation::load_animation (const std::string & filename)
    {
        animation_map sprite;
        animation_list anim;
        animation_frame *cur;
        int idx;

        m_sprite = m_allAnimations.find(filename);
        if (m_sprite != m_allAnimations.end()) {
            m_valid = true;
            return m_valid;
        }


        // TODO Load xml file here. 
        const int movedist = 8;
        char Buffer[100];
        anim.clear();
        cur = new animation_frame(surface_cache("gfx/character/npc/naked_guy/east_mov1.png"));
        anim.push_back(cur);
        sprite["stand_east"] = anim;
        anim.clear();
        for(idx = 0; idx < 6; idx ++) {
            sprintf(Buffer, "gfx/character/npc/naked_guy/east_mov%d.png", idx+1);
            cur = new animation_frame(surface_cache(Buffer), 0, movedist, 0);
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
            cur = new animation_frame(surface_cache(Buffer, true, true), 0, -movedist, 0);
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
            cur = new animation_frame(surface_cache(Buffer), 0, 0, movedist);
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
            cur = new animation_frame(surface_cache(Buffer), 0, 0, -movedist);
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

}
