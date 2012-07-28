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
 * @file gui/ui_event.cc
 *
 * @author Kai Sterker
 * @brief Implements the ui_event class.
 */

#include "ui_event.h"

using gui::ui_event;

// create a new ui event
ui_event::ui_event (widget *source, const std::string & action, void *user_data) : event ()
{
    Repeat = -1;
    Source = source;
    Action = action;
    UserData = user_data;
}
