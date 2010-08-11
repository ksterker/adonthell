#include "button.h"
#include "base/logging.h"


namespace gui
{
    void button::draw(const s_int16 & x, const s_int16 & y, const gfx::drawing_area *da, gfx::surface *target) const
	{
		if (clicked) //have it drawn with the alternate image
		{
			Cache->set_offset (clickpush, clickpush);
            Look->set_state("Activated");
			label::draw(x, y, da, target);
		}
		else
		{
			Cache->set_offset (0, 0);
            Look->set_state("Default");
			label::draw(x, y, da, target);
		}
	}
	bool button::keydown(input::keyboard_event& k)
	{
		if (!clicked && (k.key() == input::keyboard_event::SPACE_KEY || k.key() == input::keyboard_event::RETURN_KEY))
		{
			clicked = true;
			downkey = k.key();
			return true;
		}
		return false;
	}
	bool button::keyup(input::keyboard_event&k)
	{
		if (clicked && k.key() == downkey)
		{
			clicked = false;
			activate();
			return true;
		}
		return false;
	}
//for if we have mouse support later
/*
	bool button::mousedown(SDL_MouseButtonEvent & m)
	{
		LOG(INFO) << logging::indent() << "mouse down";
		if (m.button == SDL_BUTTON_LEFT)
		{
			clicked = true;
			downkey = SDLK_0;
			return true;
		}
		return false;
	}
	bool button::mouseup(SDL_MouseButtonEvent & m)
	{
		LOG(INFO) << logging::indent() << "mouse up";
		if (m.button == SDL_BUTTON_LEFT && clicked)
		{
			clicked = false;
			if (m.x < w && m.y < h) //if within bounds, activate
				activate();
			return true;
		}
		return false;
	}
// */
};
