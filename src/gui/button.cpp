#include "button.h"

#include <iostream>
using namespace std;

namespace gui
{
	void button::draw(int x, int y, gfx::surface* s)
	{
		if (clicked) //have it drawn with the alternate image
		{
			const gfx::surface* old = bg;
			bg = imgdown;
			px = py = clickpush;
			label::draw(x, y, s);
			bg = old;
		}
		else
		{
			px = py = 0;
			label::draw(x, y, s);
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
		cout << "mouse down\n";
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
		cout << "mouse up\n";
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
