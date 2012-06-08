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
#include "base/logging.h"
#include "base/diskio.h"
#include "gfx/gfx.h"
#include "gfx/surface_cacher.h"

using gui::decoration;
using gui::std_decoration;

#define DECORATION_DIR "gfx/gui/"

// dtor
std_decoration::~std_decoration ()
{
    for (std::vector<const gfx::surface*>::iterator i = Elements.begin(); i != Elements.end(); i++)
    {
        gfx::surfaces->free_surface (*i);
    }

    for (std::vector<const gfx::surface*>::iterator i = BrightElements.begin(); i != BrightElements.end(); i++)
    {
        if (*i != NULL) delete *i;
    }

    Elements.clear();
    BrightElements.clear();
}

// init from record
bool std_decoration::init (base::flat & record)
{
    // get optional background
    std::string path = DECORATION_DIR + record.get_string("bg", true);
    if (path != DECORATION_DIR)
    {
        if (base::Paths().find_in_path(path))
        {
            const gfx::surface *s = gfx::surfaces->get_surface_only(path, false, false);
            Elements.push_back(s);

            Length = s->length();
            Height = s->height();
        }
        else return false;
    }
    else Elements.push_back (NULL);

    // get optional alpha of background
    Alpha = 255 - record.get_uint8 ("bg_alpha", true);
    
    // get optional brightness adjustment for focus
    Highlight = record.get_sint8 ("highlight", true);
    if (Highlight == -1) Highlight = 0;
    
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

        // calculate border size, if any
        if (result)
        {
            Border.move (Elements[BORDER_LEFT]->length(), Elements[BORDER_TOP]->height());
            Border.resize (Elements[BORDER_RIGHT]->length(), Elements[BORDER_BOTTOM]->height());
        }
    }
    
    return result;
}

// load gfx element
bool std_decoration::add_element (base::flat & record, const std::string &id)
{
    std::string path = DECORATION_DIR + record.get_string (id);
    if (path != DECORATION_DIR && base::Paths().find_in_path (path))
    {
        Elements.push_back (gfx::surfaces->get_surface_only (path, true, false));
        return true;
    }
    
    return false;
}

// resize the decoration
void std_decoration::set_size (const u_int16 & length, const u_int16 & height)
{
    Length = length;
    Height = height;
}

// create a brighter version of the widget
void std_decoration::set_brightness (const s_int8 & level)
{
    // only do this once
    if (!BrightElements.empty()) return;

    for (std::vector<const gfx::surface*>::iterator i = Elements.begin(); i != Elements.end(); i++)
    {
        gfx::surface *brightElement = NULL;

        if (*i != NULL)
        {
            brightElement = gfx::create_surface();
            brightElement->copy(*(*i));
            brightElement->set_brightness (level);
        }

        BrightElements.push_back (brightElement);
    }

    Highlight = level;
}

// draw decoration background
void std_decoration::draw_background (const s_int16 & x, const s_int16 & y, const gfx::drawing_area * da_opt, gfx::surface * target, const bool & highlight)
{
    const std::vector<const gfx::surface*> & parts = highlight ? BrightElements : Elements;

    if (parts[BACKGROUND] != NULL)
    {
        gfx::drawing_area da (x, y, Length, Height);
        da.shrink(Border);
        da.assign_drawing_area(da_opt);

        if (Alpha != 255)
        {
            gfx::surface *tmp = gfx::create_surface ();
            tmp->copy(*parts[BACKGROUND]);
            tmp->set_alpha(Alpha);
            tmp->tile(&da, target);

            delete tmp;
        }
        else
        {
            parts[BACKGROUND]->tile(&da, target);
        }
    }
}

// draw decoration border
void std_decoration::draw_border (const s_int16 & x, const s_int16 & y, const gfx::drawing_area * da_opt, gfx::surface * target, const bool & highlight)
{
    const std::vector<const gfx::surface*> & parts = highlight ? BrightElements : Elements;

    if (parts.size() > 8)
    {
        gfx::drawing_area da;
        
        // the sides
        da = gfx::drawing_area (x + parts[BORDER_TOP_LEFT]->length(), y, Length - parts[BORDER_TOP_LEFT]->length() - parts[BORDER_TOP_RIGHT]->length(), parts[BORDER_TOP]->height());
        da.assign_drawing_area(da_opt);
        parts[BORDER_TOP]->tile (&da, target);
        da = gfx::drawing_area (x, y + parts[BORDER_TOP_LEFT]->height(), parts[BORDER_LEFT]->length(), Height - parts[BORDER_TOP_LEFT]->height() - parts[BORDER_BOTTOM_LEFT]->height());
        da.assign_drawing_area(da_opt);
        parts[BORDER_LEFT]->tile (&da, target);
        da = gfx::drawing_area (x + parts[BORDER_BOTTOM_LEFT]->length(), y + Height - parts[BORDER_BOTTOM]->height(), Length - parts[BORDER_BOTTOM_LEFT]->length() - parts[BORDER_BOTTOM_RIGHT]->length(), parts[BORDER_BOTTOM]->height());
        da.assign_drawing_area(da_opt);
        parts[BORDER_BOTTOM]->tile (&da, target);
        da = gfx::drawing_area (x + Length - parts[BORDER_RIGHT]->length(), y + parts[BORDER_TOP_RIGHT]->length(), parts[BORDER_RIGHT]->length(), Height  - parts[BORDER_TOP_RIGHT]->height() - parts[BORDER_BOTTOM_RIGHT]->height());
        da.assign_drawing_area(da_opt);
        parts[BORDER_RIGHT]->tile (&da, target);
        
        // the corners
        parts[BORDER_TOP_LEFT]->draw (x, y, da_opt, target);
        parts[BORDER_TOP_RIGHT]->draw (x + Length - parts[BORDER_TOP_RIGHT]->length(), y, da_opt, target);
        parts[BORDER_BOTTOM_LEFT]->draw (x , y + Height - parts[BORDER_TOP_RIGHT]->height(), da_opt, target);
        parts[BORDER_BOTTOM_RIGHT]->draw (x + Length - parts[BORDER_TOP_RIGHT]->length(), y + Height - parts[BORDER_TOP_RIGHT]->height(), da_opt, target);
    }
}

// default decoration factory
gui::decoration_factory gui::decoration_factory::DEFAULT;

// a border of size 0
gfx::drawing_area decoration::EMPTY_BORDER;

// initialize a default decoration
void decoration::init ()
{
    base::flat record;
    record.put_bool("has_border", false);

    decoration_factory *factory = Factory ? Factory : &decoration_factory::DEFAULT;
    std_decoration *di = factory->create_decorator("Default");
    if (di->init (record))
    {
        Decoration["Default"] = di;
        set_state ("Default");
    }
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
    
    decoration_factory *factory = Factory ? Factory : &decoration_factory::DEFAULT;

    u_int32 size;
    void *value;
    char *id;
    
    while (file.next (&value, &size, &id) == base::flat::T_FLAT)
    {
        base::flat record = base::flat ((const char*) value, size);
        std_decoration *di = factory->create_decorator(id);
        
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
    
    set_state ("Default");
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

// set focused state
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
void decoration::draw (const s_int16 & x, const s_int16 & y, const gfx::drawing_area * da, gfx::surface * target, const u_int32 & parts) const
{
    if (FocusOverlay != Decoration.end())
    {
        if (CurrentState != Decoration.end())
        {
            // center widget in overlay (in case overlay is bigger than widget)
    	    //s_int16 ox = (FocusOverlay->second->length() - CurrentState->second->length()) / 2;
    	    //s_int16 oy = (FocusOverlay->second->height() - CurrentState->second->height()) / 2;

            // apply highlight to widget, if required
            if (FocusOverlay->second->highlight())
            {
            	CurrentState->second->set_brightness (128 + FocusOverlay->second->highlight());
            }

            // draw widget
            if ((parts & BACKGROUND) == BACKGROUND) CurrentState->second->draw_background (x, y, da, target, FocusOverlay->second->highlight() != 0);
            if ((parts & BORDER) == BORDER) CurrentState->second->draw_border (x, y, da, target, FocusOverlay->second->highlight() != 0);
        }

        // draw actual overlay
        if ((parts & BACKGROUND) == BACKGROUND) FocusOverlay->second->draw_background (x, y, da, target);
        if ((parts & BORDER) == BORDER) FocusOverlay->second->draw_border (x, y, da, target);
    }
    else
    {
        // draw plain widget
        if (CurrentState != Decoration.end())
        {
            if ((parts & BACKGROUND) == BACKGROUND) CurrentState->second->draw_background (x, y, da, target);
            if ((parts & BORDER) == BORDER) CurrentState->second->draw_border (x, y, da, target);
        }
    }
}
