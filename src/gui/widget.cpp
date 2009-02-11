#include "widget.h"

namespace gui
{
	void widget::draw(int x, int y, gfx::surface* s)
	{
		if (bg)
		{
			bg->draw(x, y, NULL, s);
		}
	}
};

