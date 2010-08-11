/*
 Copyright (C) 2008 Rian Shelley
 Copyright (C) 2010 Kai Sterker <kai.sterker@linuxgames.com>
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
 * @file   gui/text_cache.h
 * @author Rian Shelley
 * @author Kai Sterker
 * @brief  Defines a class for rendering text.
 */

#ifndef GUI_TEXTCACHE_H
#define GUI_TEXTCACHE_H

#include "gfx/surface.h"

namespace gui
{
class label;
class text_cache
{
public:
    text_cache (); 
    ~text_cache () { delete Cached; }
    
    const gfx::surface *render (const label *lbl, gfx::surface *target);
    
    void clear () 
    {
        delete Cached;
        Cached = NULL;
        Valid = false;
    }
    
    void invalidate ()
    {
        Valid = false;
    }

    void scroll (const u_int32 & s) { ScrollOffset = s; }
    
    void set_offset (const u_int32 & x, const u_int32 & y) { Ox = x; Oy = y; }
    
    u_int32 ox () const { return Ox; }
    u_int32 oy () const { return Oy; }
    
    bool blink();
    
private:
    void render_text (const u_int32 & rx, const u_int32 & ry, const label *lbl);
    
    /// prerendered image of the text
    gfx::surface *Cached;
    /// whether the cache is valid
    bool Valid;
    /// amount to scroll text if it does not fit into the line
    u_int32 ScrollOffset;
    /// timestamp for a blinking cursor
    u_int32 LastBlink;
    /// draw offset x
    u_int32 Ox;
    /// draw offset y
    u_int32 Oy;
};

}

#endif
