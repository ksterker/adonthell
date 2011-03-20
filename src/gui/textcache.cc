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
 * @file   gui/text_cache.cc
 * @author Rian Shelley
 * @author Kai Sterker
 * @brief  Defines a class for rendering text.
 */

#include "base/base.h"
#include "gui/textcache.h"
#include "gui/label.h"

using gui::text_cache;

typedef union
{
	u_int8 c[4];
	u_int32 i;
} color;

// ctor
text_cache::text_cache () : Cached(NULL), Valid(false), ScrollOffset(0), Ox(0), Oy(0) 
{
    LastBlink = ::base::Timer.current_time();
}

// render text contained in label
const gfx::surface * text_cache::render (const gui::label *lbl, gfx::surface *target)
{
    if (!lbl->length() || !lbl->height()) return NULL;
    
    // do we need to regenerate the Cached image?
    if (!Valid)
    {
        const std::string txt = lbl->get_string();
        font *f = lbl->get_font();
        
        // render offset
        u_int32 rx, ry;
        
        // find the needed size of the cache
        u_int32 nw, nh;
        f->getSize(txt, nw, nh);
        
        // TODO: properly account for the case where both centering and 
        // multiline are enabled. 
        
        // clamp the size to the size of the widget
        if (nw > lbl->length()) nw = lbl->length();
        if (nh > lbl->height()) nh = lbl->height();

        // does the existing cache need to be recreated?
        if (!Cached)
        {
            Cached = gfx::create_surface();
            Cached->set_alpha(255, true);
            Cached->resize(lbl->length(), lbl->height());
        }

        // clear cache reset
        // Cached->fillrect(0, 0, lbl->length(), lbl->height(), Cached->map_color (0, 0, 0, 0));
        
        // compute where to render it
        if (lbl->multiline())
        {
            rx = 0;
        }
        else if (!ScrollOffset && lbl->center_x())
        {
            // if a manual offset has been set, centering doesnt matter
            rx = (lbl->length() - nw)/2;
        }
        else
        {
            rx = -ScrollOffset;
        }
        
        if (lbl->center_y())
        {
            ry = (lbl->height() + f->getSize())/2;
        }
        else
        {
            ry = f->getSize();
        }
        
        // this effect tries to do a gaussian blur of the text behind it
        // the idea is to make the text itself easier to read
        // try to get a reasonable background color
        color old, n;
        old.i = f->getColor();
        target->unmap_color(old.i, n.c[0], n.c[1], n.c[2], n.c[3]);
        
        // render in a background color, to get contrast
        float intensity = n.c[0]*.3 + n.c[1]*0.59 + n.c[2]*0.11;
        if (intensity < 128.0)
        {
            f->setColor (Cached->map_color (0xff, 0xff, 0xff, 0xff));
            Cached->fillrect(0, 0, lbl->length(), lbl->height(), Cached->map_color (0xff, 0xff, 0xff, 0));
        }
        else
        {
            f->setColor (Cached->map_color (0, 0, 0, 0xff));
            Cached->fillrect(0, 0, lbl->length(), lbl->height(), Cached->map_color (0, 0, 0, 0));
        }
        render_text (rx, ry, lbl);

        // apply a gaussian blur to it
        Cached->blur();

        // render on top of the blur, with the normal color
        f->setColor(n.i);
        render_text (rx, ry, lbl);
        f->setColor(old.i);

        // cache is valid now
        Valid = true;
    }
    
    return Cached;
}

/// the rate of blinking in ms
#define BLINKRATE 500

bool text_cache::blink(const bool & reset)
{
    int now = ::base::Timer.current_time();
    if (reset || LastBlink + BLINKRATE*2 < now)
    {
        // turn on cursor every second
        LastBlink = ::base::Timer.current_time();
    }
    
    // show cursor for 500ms
    return LastBlink + BLINKRATE > now;
}

void text_cache::render_text(const u_int32 & rx, const u_int32 & ry, const label *lbl)
{
    const std::string & txt = lbl->get_string();
    font * f = lbl->get_font();

    if (lbl->multiline())
    {
        int p = 0;
        int y = ry;
        u_int16 tw, th;
        std::vector<gui::textsize> ts;
        f->getMultilineSize(txt, Cached->length(), ts, tw, th);

        for (int i = 0; i < ts.size(); i++)
        {
            f->render(txt.substr(p, ts[i].cpos-p), rx, y, Cached);
            y += ts[i].h;
            p = ts[i].cpos + 1;
        }
    }
    else
    {
        f->render(txt, rx, ry, Cached);
    }
}
