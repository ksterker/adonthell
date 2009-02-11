#include "option.h"

namespace gui
{
	bool option::keydown(input::keyboard_event& k)
	{
		if (!state && (k.key() == input::keyboard_event::SPACE_KEY || k.key() == input::keyboard_event::RETURN_KEY))
		{
			state = true;
			downkey = k.key();
			return true;
		}
		return false;
	}
	bool option::keyup(input::keyboard_event&k)
	{
		if (state && k.key() == downkey)
		{
			state = false;
			activate();
			return true;
		}
		return false;
	}
//no mouse support yet
/*
	bool option::mousedown(SDL_MouseButtonEvent & m)
	{
		if (m.button == SDL_BUTTON_LEFT)
		{
			state = true;
			downkey = SDLK_0;
			return true;
		}
		return false;
	}
	bool option::mouseup(SDL_MouseButtonEvent & m)
	{
		if (m.button == SDL_BUTTON_LEFT && state)
		{
			state = false;
			if (m.x < w && m.y < h) //if within bounds, activate
				activate();
			return true;
		}
		return false;
	}
// */
	void option::activate() 
	{
		clicked = !clicked;
		clickit(clicked, arg);
	}
};
