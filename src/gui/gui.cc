/*
   Copyright (C) 2013 Kai Sterker <kai.sterker@gmail.com>
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
 * @file   gui/gui.cc
 * @author Kai Sterker <kai.sterker@gmail.com>
 *
 * @brief  GUI module init and cleanup
 */

#include "gui.h"
#include "fontcache.h"
#include "ui_event_manager.h"

namespace gui
{
    /// the UI event manager instance
    static ui_event_manager *UiEventManager = NULL;

    // startup GUI
    bool init()
    {
        UiEventManager = new ui_event_manager;

        FontCache = new font_cache();
        return FontCache != NULL;
    }

    // configure GUI
    void setup(base::configuration & cfg)
    {
        // nothing to do for now
    }

    // shutdown GUI
    void cleanup()
    {
        delete FontCache;
        FontCache = NULL;

        delete UiEventManager;
        UiEventManager = NULL;
    }

    /// a single font cache
    font_cache *FontCache = NULL;
}
