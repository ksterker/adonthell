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
 * @file   gui/button.cpp
 * @author Rian Shelley
 * @brief  Implements the button class.
 */

#include <adonthell/base/logging.h>
#include <adonthell/event/manager.h>
#include "button.h"
#include "ui_event.h"

/// offset of text in Clicked button
#define CLICK_OFFSET 4

namespace gui
{
	// render button
    void button::draw(const s_int16 & x, const s_int16 & y, const gfx::drawing_area *da, gfx::surface *target) const
	{
		if (Clicked) //have it drawn with the alternate image
		{
			// Font->set_offset (CLICK_OFFSET, CLICK_OFFSET);
            Look->set_state("Activated");
			label::draw(x, y, da, target);
		}
		else
		{
			// Font->set_offset (0, 0);
            Look->set_state("Default");
			label::draw(x, y, da, target);
		}
	}

    // key pressed
	bool button::keydown(input::keyboard_event& k)
	{
		if (!Clicked && (k.key() == input::keyboard_event::SPACE_KEY || k.key() == input::keyboard_event::RETURN_KEY))
		{
			DownKey = k.key();
			Clicked = true;
			return true;
		}

		return false;
	}

	// key released
	bool button::keyup(input::keyboard_event&k)
	{
		if (Clicked && k.key() == DownKey)
		{
			activate();
			Clicked = false;
			return true;
		}

		return false;
	}

    gui::ui_event* button::get_activate_event (void* user_data)
    {
        // create listener for button click event
        return new ui_event(this, "activate", user_data);
    }

    void button::activate()
    {
        gui::ui_event evt (this, "activate");
        events::manager::raise_event (&evt);
    }

//for if we have mouse support later
/*
	bool button::mousedown(SDL_MouseButtonEvent & m)
	{
		LOG(INFO) << logging::indent() << "mouse down";
		if (m.button == SDL_BUTTON_LEFT)
		{
			Clicked = true;
			DownKey = SDLK_0;
			return true;
		}
		return false;
	}
	bool button::mouseup(SDL_MouseButtonEvent & m)
	{
		LOG(INFO) << logging::indent() << "mouse up";
		if (m.button == SDL_BUTTON_LEFT && Clicked)
		{
			Clicked = false;
			if (m.x < w && m.y < h) //if within bounds, activate
				activate();
			return true;
		}
		return false;
	}
// */
};
