/*
 Copyright (C) 2011 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file gui/fontcache.cc
 *
 * @author Kai Sterker
 * @brief Handles caching of glyphs.
 */

#include <sstream>

#include "fontcache.h"
#include "font.h"

using gui::glyph_info;
using gui::font_cache;

font_cache::font_cache()
{
    // TODO Auto-generated constructor stub
}

font_cache::~font_cache()
{
    // TODO Auto-generated destructor stub
}

const glyph_info* font_cache::get (const u_int32 & glyph, gui::font *f)
{
    // create a unique name based on font attributes
    std::stringstream cache_name (std::ios::out);
    cache_name << std::hex << glyph << "_" << f->color() << "_" << f->size() << "_" << f->name();

    // check for glyph in cache
    std::hash_map<std::string, glyph_info*>::iterator idx = Cache.find(cache_name.str());
    if (idx != Cache.end())
    {
        return idx->second;
    }

    // not found, we need to render it
    glyph_info *gi = f->create_glyph(glyph);
    Cache[cache_name.str()] = gi;

    return gi;
}

