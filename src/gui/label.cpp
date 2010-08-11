#include "gui/label.h"
#include "gfx/gfx.h"

namespace gui
{
    void label::draw (const s_int16 & x, const s_int16 & y, const gfx::drawing_area *da, gfx::surface *target) const
	{
		widget::draw(x, y, da, target);
        const gfx::surface *txt = Cache->render (this, target);
        if (txt) txt->draw (x + Cache->ox(), y + Cache->oy(), da, target);
	}
    
	/*
	 * This function will change the height of the object based on the text
	 * It sets multiline to true
	 */
	void label::reheight()
	{
		vector<textsize> ts;
		u_int16 newwidth = 0, newheight = 0;
		Font->getMultilineSize (txt, length(), ts, newwidth, newheight);
        set_multiline (ts.size() > 1);
		set_size (length(), newheight);
	}
};
