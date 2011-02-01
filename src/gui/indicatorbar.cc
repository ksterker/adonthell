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
 * @file gui/indicatorbar.cc
 *
 * @author Kai Sterker
 * @brief Progress and scroll indicator.
 */

#include <algorithm>
#include "gui/indicatorbar.h"

using gui::indicator_bar;
using gui::indicator_decoration;

// initialize the decoration from given file
bool indicator_decoration::init (base::flat & record)
{
    // get optional brightness adjustment for focus
    Highlight = record.get_sint8 ("highlight", true);
    if (Highlight == -1) Highlight = 0;

    // get indicator parts
    bool result = record.success();

	// the order is important and must match the enum
	result &= add_element (record, "bar_v_start");
	result &= add_element (record, "bar_v_mid");
	result &= add_element (record, "bar_v_end");
	result &= add_element (record, "bar_v_handle");
	result &= add_element (record, "bar_h_start");
	result &= add_element (record, "bar_h_mid");
	result &= add_element (record, "bar_h_end");
	result &= add_element (record, "bar_h_handle");

	// calculate size
	if (result)
	{
		Length = Elements[V_MID]->length();
		Height = Elements[H_MID]->height();
	}

    return result;
}

// render the decoration
void indicator_decoration::draw_background (const s_int16 & x, const s_int16 & y, const gfx::drawing_area * da_opt, gfx::surface * target, const bool & highlight)
{
    const std::vector<const gfx::surface*> & parts = highlight ? BrightElements : Elements;

	// render vertical bar
	if (Length == 0)
	{
		Length = parts[V_MID]->length();
		Height = std::max (Height, u_int16 (parts[V_START]->height() + parts[V_END]->height() + parts[V_HANDLE]->height()));

	    // fill indicator bar
	    gfx::drawing_area da (x, y + parts[V_START]->height(), Length, Height - parts[V_START]->height() - parts[V_END]->height());
        da.assign_drawing_area (da_opt);
        parts[V_MID]->tile (&da, target);

	    // draw corners and handle
	    parts[V_START]->draw (x, y, da_opt, target);
	    parts[V_HANDLE]->draw (x, y + (Height - parts[V_HANDLE]->height())/2, da_opt, target);
	    parts[V_END]->draw (x, y + Height - parts[V_END]->height(), da_opt, target);
	}
	// render horizontal bar
	else // Height == 0
	{
		Length = std::max (Length, u_int16 (parts[H_START]->length() + parts[H_END]->length() + parts[H_HANDLE]->length()));
		Height = parts[H_MID]->height();

	    // fill indicator bar
	    gfx::drawing_area da (x + parts[H_START]->length(), y, Length - parts[H_START]->length() - parts[H_END]->length(), Height);
	    da.assign_drawing_area(da_opt);
	    parts[H_MID]->tile (&da, target);

	    // draw corners and handle
	    parts[H_START]->draw (x, y, da_opt, target);
	    parts[H_HANDLE]->draw (x + (Length - parts[H_HANDLE]->length())/2, y, da_opt, target);
	    parts[H_END]->draw (x + Length - parts[H_END]->length(), y, da_opt, target);
	}
}

// render the indicator
void indicator_bar::draw (const s_int16 & x, const s_int16 & y, const gfx::drawing_area * da, gfx::surface * target) const
{
	// draw background
	widget::draw (x, y, da, target);

    u_int16 l = 0, h = 0;
    Slider->get_size (l, h);

	// draw actual indicator on top
	if (Orientation == VERTICAL)
	{
		// map indicator to actual size of the bar
		double ratio = height() / double (Max - Min);
		u_int32 top = Lower * ratio;
		u_int32 bot = Upper * ratio;

		// center indicator on background
		s_int16 ox = (length() - l) / 2;

		Slider->set_size (0, bot - top);
		Slider->draw (x + ox, y + top, da, target, decoration::BACKGROUND);
	}
	else
	{
		// map indicator to actual size of the bar
		double ratio = length() / double (Max - Min);
		u_int32 top = Lower * ratio;
		u_int32 bot = Upper * ratio;

		// center indicator on background
		s_int16 oy = (height() - h) / 2;

		Slider->set_size (bot - top, 0);
		Slider->draw (x + top, y + oy, da, target, decoration::BACKGROUND);
	}
}
