/*
 Copyright (C) 2010 Kai Sterker <kai.sterker@gmail.com> 
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
 * @file gui/decoration.cc
 * @author Kai Sterker <kai.sterker@gmail.com>
 * @brief  Defines the gui decoration class.
 */

#include "gui/decoration.h"
#include "base/base.h"
#include "base/logging.h"
#include "base/diskio.h"
#include "gfx/gfx.h"
#include "gfx/surface_cacher.h"

using gui::decoration;
using gui::decoration_info;

#define DECORATION_DIR "gfx/gui/"

// dtor
decoration_info::~decoration_info ()
{
    for (std::vector<const gfx::surface*>::iterator i = Elements.begin(); i != Elements.end(); i++)
    {
        gfx::surfaces->free_surface (*i);
    }
}

// init from record
bool decoration_info::init (base::flat & record)
{
    // get background, which is mandatory
    std::string path = DECORATION_DIR + record.get_string ("bg");
    if (base::Paths.find_in_path (path))
    {
        const gfx::surface *s = gfx::surfaces->get_surface_only (path, false, false);
        Elements.push_back (s);
        
        Length = s->length();
        Height = s->height();
    }
    else return false;
    
    // get optional alpha of background
    Alpha = 255 - record.get_uint8 ("bg_alpha", true);
    
    // get border, if present
    bool result = record.success();
    if (record.get_bool ("has_border"))
    {
        // the order is important and must match the enum 
        result &= add_element (record, "border_tl");
        result &= add_element (record, "border_bl");
        result &= add_element (record, "border_tr");
        result &= add_element (record, "border_br");
        result &= add_element (record, "border_lft");
        result &= add_element (record, "border_rgt");
        result &= add_element (record, "border_top");
        result &= add_element (record, "border_bot");
    }
    
    return result;
}

// load gfx element
bool decoration_info::add_element (base::flat & record, const std::string &id)
{
    std::string path = DECORATION_DIR + record.get_string (id);
    if (base::Paths.find_in_path (path))
    {
        Elements.push_back (gfx::surfaces->get_surface_only (path, false, false));
        return true;
    }
    
    return false;
}

// resize the decoration
void decoration_info::set_size (const u_int16 & length, const u_int16 & height)
{
    if (Length != length || Height != height)
    {
        delete Cache;
        Cache = NULL;
        
        Length = length;
        Height = height;
    }
}

// get decoration surface
gfx::surface *decoration_info::get_surface ()
{
    if (Cache) return Cache;
    
    // recreate decoration
    Cache = gfx::create_surface ();
    Cache->set_alpha (255, true);
    Cache->resize (Length, Height);
    
    // the background
    if (Alpha != 255)
    {
        gfx::surface *tmp = gfx::create_surface ();
        tmp->resize (Elements[BACKGROUND]->length(), Elements[BACKGROUND]->height());
        Elements[BACKGROUND]->draw (0, 0, NULL, tmp);
        Cache->fillrect (0, 0, Length, Height, Cache->map_color (0, 0, 0, Alpha));
        Cache->tile (*tmp);
        delete tmp;
    }
    else
    {
        Cache->tile (*Elements[BACKGROUND]);
    }
    
    // the border, if present
    if (Elements.size() > 1)
    {
        const gfx::surface *s;
        gfx::drawing_area da;
        
        // the border
        da = gfx::drawing_area (Elements[BORDER_TOP_LEFT]->length(), 0, Length - Elements[BORDER_TOP_LEFT]->length() - Elements[BORDER_TOP_RIGHT]->length(), Elements[BORDER_TOP]->height());
        Cache->fillrect (0, 0, 0, 0, 0, &da);
        Cache->tile (*Elements[BORDER_TOP], &da);
        da = gfx::drawing_area (0, Elements[BORDER_TOP_LEFT]->height(), Elements[BORDER_LEFT]->length(), Height - Elements[BORDER_TOP_LEFT]->height() - Elements[BORDER_BOTTOM_LEFT]->height());
        Cache->fillrect (0, 0, 0, 0, 0, &da);
        Cache->tile (*Elements[BORDER_LEFT], &da);
        da = gfx::drawing_area (Elements[BORDER_BOTTOM_LEFT]->length(), Height - Elements[BORDER_BOTTOM]->height(), Length - Elements[BORDER_BOTTOM_LEFT]->length() - Elements[BORDER_BOTTOM_RIGHT]->length(), Height);
        Cache->fillrect (0, 0, 0, 0, 0, &da);
        Cache->tile (*Elements[BORDER_BOTTOM], &da);
        da = gfx::drawing_area (Length - Elements[BORDER_RIGHT]->length(), Elements[BORDER_TOP_RIGHT]->length(), Length, Height  - Elements[BORDER_TOP_RIGHT]->height() - Elements[BORDER_BOTTOM_RIGHT]->height());
        Cache->fillrect (0, 0, 0, 0, 0, &da);
        Cache->tile (*Elements[BORDER_RIGHT], &da);
        
        // the corners
        s = Elements[BORDER_TOP_LEFT]; 
        da = gfx::drawing_area (0, 0, s->length(), s->height());
        Cache->fillrect (0, 0, 0, 0, 0, &da);
        s->draw (0, 0, NULL, Cache);
        s = Elements[BORDER_TOP_RIGHT]; 
        da = gfx::drawing_area (Length - s->length(), 0, s->length(), s->height());
        Cache->fillrect (0, 0, 0, 0, 0, &da);
        s->draw (Length - s->length(), 0, NULL, Cache);
        s = Elements[BORDER_BOTTOM_LEFT]; 
        da = gfx::drawing_area (0, Height - s->height(), s->length(), s->height());
        Cache->fillrect (0, 0, 0, 0, 0, &da);
        s->draw (0, Height - s->height(), NULL, Cache);
        s = Elements[BORDER_BOTTOM_RIGHT]; 
        da = gfx::drawing_area (Length - s->length(), Height - s->height(), s->length(), s->height());
        Cache->fillrect (0, 0, 0, 0, 0, &da);
        s->draw (Length - s->length(), Height - s->height(), NULL, Cache);
    }
    
    return Cache;
}

// initialize decoration
bool decoration::init (const std::string & name)
{
    // reset in case we're called more than once
    cleanup ();
    
    base::diskio file;
    if (!file.get_record (DECORATION_DIR + name))
    {
        // error loading the file (file not found?)
        return false;
    }
    
    u_int32 size;
    void *value;
    char *id;
    
    while (file.next (&value, &size, &id) == base::flat::T_FLAT)
    {
        base::flat record = base::flat ((const char*) value, size);
        decoration_info *di = new decoration_info ();
        
        if (di->init (record))
        {
            Decoration[id] = di;
        }
        else
        {
            LOG(ERROR) << logging::indent() << "decoration::init: error loading state '" << id << "'.";
            delete di;
        }
    }
    
    CurrentState = Decoration.begin();
    return file.success();
}

// cleanup
void decoration::cleanup ()
{
    for (decoration_map::iterator i = Decoration.begin(); i != Decoration.end(); i++)
    {
        delete i->second;
    }
    Decoration.clear();
    
    // make sure iterators remain valid
    CurrentState = Decoration.end();
    FocusOverlay = Decoration.end();
}

// set decoration size
void decoration::set_size (const u_int16 & length, const u_int16 & height)
{
    for (decoration_map::iterator i = Decoration.begin(); i != Decoration.end(); i++)
    {
        i->second->set_size (length, height);
    }
}

// set decoration state
void decoration::set_state (const std::string & state)
{
    if (Decoration.size() > 0)
    {
        decoration_map::iterator NewState = Decoration.find (state);
        if (NewState != Decoration.end())
        {
            CurrentState = NewState;
        }
        else
        {
            LOG(WARNING) << logging::indent() << "decoration::set_state: unknown state '" << state << "'.";
        }
    }
}

// set focussed state
void decoration::set_focused (const bool & has_focus)
{
    if (Decoration.size() > 0)
    {
        if (has_focus)
        {
            FocusOverlay = Decoration.find("Focused");
            if (FocusOverlay == Decoration.end())
            {
                LOG(WARNING) << logging::indent() << "decoration::set_state: unknown state 'Focused'.";
            }
        }
        else
        {
            FocusOverlay = Decoration.end();
        }
    }
}

// draw decoration
void decoration::draw (const s_int16 & x, const s_int16 & y, const gfx::drawing_area * da, gfx::surface * target) const
{
    if (CurrentState != Decoration.end())
    {
        CurrentState->second->get_surface()->draw (x, y, da, target);
    }
    
    if (FocusOverlay != Decoration.end())
    {
        FocusOverlay->second->get_surface()->draw (x, y, da, target);        
    }
}
