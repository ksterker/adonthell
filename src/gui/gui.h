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
 * @file   gui/gui.h
 * @author Kai Sterker <kai.sterker@gmail.com>
 *
 * @brief  The GUI module main include file.
 */

#ifndef GUI_H
#define GUI_H

#include <adonthell/base/configuration.h>

namespace gui
{
    /**
     * Initialize the GUI module. This function \e must be called
     * before doing anything GUI-related.
     * @return \e true in case of success, \e false otherwise.
     */
    bool init();

    /**
     * Load GUI settings from configuration file.
     * @param cfg the game configuration
     */
    void setup (base::configuration & cfg);

    /**
     * Cleanup the GUI module - call this function once you've
     * finished using it.
     */
    void cleanup();
}

#endif
