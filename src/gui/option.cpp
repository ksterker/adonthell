/*
 Copyright (C) 2008 Rian Shelley
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
 * @file   gui/option.cpp
 * @author Rian Shelley
 * @brief  Implements the option class.
 */

#include <adonthell/event/manager.h>
#include "option.h"
#include "ui_event.h"

namespace gui
{
	// key pressed
	bool option::keydown(input::keyboard_event& k)
	{
		if (!State && (k.key() == input::keyboard_event::SPACE_KEY || k.key() == input::keyboard_event::RETURN_KEY))
		{
			State = true;
			DownKey = k.key();
			return true;
		}

		return false;
	}

	// key released
	bool option::keyup(input::keyboard_event&k)
	{
		if (State && k.key() == DownKey)
		{
			State = false;
			activate();
			return true;
		}

		return false;
	}

	// switch selection state
	void option::activate()
	{
		Clicked = !Clicked;

        gui::ui_event evt (this, "activate");
        events::manager::raise_event (&evt);
	}

//no mouse support yet
/*
	bool option::mousedown(SDL_MouseButtonEvent & m)
	{
		if (m.button == SDL_BUTTON_LEFT)
		{
			State = true;
			DownKey = SDLK_0;
			return true;
		}
		return false;
	}
	bool option::mouseup(SDL_MouseButtonEvent & m)
	{
		if (m.button == SDL_BUTTON_LEFT && State)
		{
			State = false;
			if (m.x < w && m.y < h) //if within bounds, activate
				activate();
			return true;
		}
		return false;
	}
// */
}
