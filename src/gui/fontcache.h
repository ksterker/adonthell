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
 * @file gui/fontcache.h
 *
 * @author Kai Sterker
 * @brief Handles caching of glyphs.
 */


#ifndef GUI_FONTCACHE_H
#define GUI_FONTCACHE_H

#include <adonthell/base/hash_map.h>
#include <adonthell/gfx/surface.h>

namespace gui
{

class font;
class glyph_info;

/**
 *
 */
class font_cache
{
public:
    font_cache();
    ~font_cache();

    /**
     * Get given glyph from the given font.
     * Return the glyph from the cache, if present.
     * Otherwise create tbe glyph and add it to the cache.
     *
     * @param glyph the glyph to return in UTF-32 format.
     * @param font the font used to render the glyph.
     *
     * @return the graphical representation of the glyph.
     */
    const glyph_info* get (const u_int32 & glyph, gui::font *f);

private:
    /// the font cache
    std::hash_map<std::string, glyph_info*> Cache;
};

}

#endif /* GUI_FONTCACHE_H */
