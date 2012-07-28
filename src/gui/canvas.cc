/*
 Copyright (C) 2010 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file gui/canvas.cc
 *
 * @author Kai Sterker
 * @brief Implements the canvas widget.
 */

#include "canvas.h"

using gui::canvas_decoration;

// draw background and canvas element
void canvas_decoration::draw_background (const s_int16 & x, const s_int16 & y, const gfx::drawing_area * da_opt, gfx::surface * target, const bool & highlight)
{
    // fill canvas with background ...
    gui::std_decoration::draw_background (x, y, da_opt, target, highlight);

    if (*Element != NULL)
    {
        s_int16 ox = (Length - (*Element)->length()) / 2;
        s_int16 oy = (Height - (*Element)->height()) / 2;

        gfx::drawing_area da (x, y, Length, Height);
        da.assign_drawing_area(da_opt);

        if (highlight)
        {
            gfx::surface *tmp = gfx::create_surface();
            tmp->resize ((*Element)->length(), (*Element)->height());
            (*Element)->draw (0, 0, NULL, tmp);
            tmp->set_brightness (Highlight);
            tmp->draw (x + ox, y + oy, &da, tmp);

            delete tmp;
        }
        else
        {
            (*Element)->draw (x + ox, y + oy, &da, target);
        }
    }
}

