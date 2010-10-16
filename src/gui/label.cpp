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
 * @file   gui/label.cpp
 * @author Rian Shelley
 * @brief  Implements the label class.
 */

#include "gui/label.h"

namespace gui
{
	// render label
    void label::draw (const s_int16 & x, const s_int16 & y, const gfx::drawing_area *da, gfx::surface *target) const
	{
    	// draw label decoration
		widget::draw(x, y, da, target);

		/// draw label text
        const gfx::surface *txt = Cache->render (this, target);
        if (txt) txt->draw (x + Cache->ox(), y + Cache->oy(), da, target);
	}
    
	// change the height of the object based on the text
	void label::reheight()
	{
		if (AutoHeight)
		{
			std::vector<textsize> ts;
			u_int16 newwidth = 0, newheight = 0;
			Font->getMultilineSize (Text, length(), ts, newwidth, newheight);

			if (newheight != height())
			{
				set_size (length(), newheight);
			}
		}
	}
};
