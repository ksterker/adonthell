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

#include "label.h"

namespace gui
{
	// render label
    void label::draw (const s_int16 & x, const s_int16 & y, const gfx::drawing_area *da, gfx::surface *target) const
	{
    	// draw label decoration
		widget::draw(x, y, da, target);

		/// draw label text
        if (!Text.empty()) draw_text (x + Ox, y + Oy, da, target);
	}
    
    void label::draw_text (const s_int16 & x, const s_int16 & y, const gfx::drawing_area *da, gfx::surface *target) const
    {
        if (!length() || !height()) return;

        // render offset
        s_int16 rx = x;
        s_int16 ry = y + Font->size();

        // compute where to render it
        if (multiline())
        {
            // find the needed size of the line of text
            u_int16 tw, th;
            std::vector<gui::textsize> ts;
            Font->get_text_size(Text, length(), ts, tw, th);

            // if required, center text vertically
            if (center_y())
            {
                ry += (height() - th)/2;
            }

            int p = 0;

            for (size_t i = 0; i < ts.size(); i++)
            {
                // if required, center text horizontally
                if (center_x())
                {
                    // center each line of multiline text
                    rx = x + (length() - ts[i].w)/2;
                }

                Font->draw_shadow(Text.substr(p, ts[i].cpos-p), rx, ry, da, target);
                Font->draw_text(Text.substr(p, ts[i].cpos-p), rx, ry, da, target);

                ry += ts[i].h;
                p = ts[i].cpos + 1;
            }
        }
        else
        {
            // find the needed size of the line of text
            u_int32 nw, nh;
            Font->get_text_size(Text, nw, nh);

            // if required, center text horizontally
            if (/*!ScrollOffset &&*/ center_x())
            {
                // if a manual offset has been set, centering doesn't matter
                rx = x + (length() - nw)/2;
            }
            // else
            // {
            //     rx = -ScrollOffset;
            // }

            // if required, center text vertically
            if (center_y())
            {
                ry += (height() - nh)/2;
            }

            Font->draw_shadow(Text, rx, ry, da, target);
            Font->draw_text(Text, rx, ry, da, target);
        }
    }

	// change the height of the object based on the text
	void label::reheight()
	{
		if (AutoHeight)
		{
			std::vector<textsize> ts;
			u_int16 newwidth = 0, newheight = 0;
			Font->get_text_size (Text, length(), ts, newwidth, newheight);
			set_size (length(), newheight);
		}
	}
}
