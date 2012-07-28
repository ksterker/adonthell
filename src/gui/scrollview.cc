/*
 Copyright (C) 2010 Kai Sterker <kai.sterker@gmail.com>
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
 * @file   gui/scrollview.cc
 * @author Kai Sterker <kai.sterker@gmail.com>
 * @brief  Implements the scrollview class.
 */

#include "scrollview.h"

using gui::scrollview;

// render scrollview
void scrollview::draw(const s_int16 & x, const s_int16 & y, const gfx::drawing_area * da, gfx::surface * target) const
{
    if (!Visible) return;

    // draw background
    Look->draw (x, y, da, target, decoration::BACKGROUND);

    // draw scrolled widget
	if (!Children.empty())
	{
		// space occupied by scroll bars
		u_int16 xo = ((ScrollMode & SCROLL_Y) == SCROLL_Y) ? VScroll->length() : 0;
		u_int16 yo = ((ScrollMode & SCROLL_X) == SCROLL_X) ? HScroll->height() : 0;

		// space available for child
	    gfx::drawing_area client_area (x, y, length() -xo, height() - yo);
	    client_area.shrink (Look->border ());
	    client_area.assign_drawing_area (da);

	    // draw child
	    Children[0].Child->draw (client_area.x() - Ox, client_area.y() - Oy, &client_area, target);

	    // draw horizontal scroll bar
	    if ((ScrollMode & SCROLL_X) == SCROLL_X)
	    {
	    	HScroll->set_max(Children[0].Child->length());
   			HScroll->set_values(Ox, Ox + client_area.length());
	    	HScroll->set_size (client_area.length(), HScroll->height());
	    	HScroll->draw (client_area.x(), client_area.y() + client_area.height(), da, target);
	    }

	    // draw vertical scroll bar
		if ((ScrollMode & SCROLL_Y) == SCROLL_Y)
		{
			VScroll->set_max(Children[0].Child->height());
			VScroll->set_values(Oy, Oy + client_area.height());
			VScroll->set_size (VScroll->length(), client_area.height());
			VScroll->draw (client_area.x() + client_area.length(), client_area.y(), da, target);
		}
	}

    // draw border
    Look->draw (x, y, da, target, decoration::BORDER);
}

// set the container that should be scrolled
void scrollview::set_child (layout & c)
{
	bool refocus = focused;
	if (!Children.empty())
	{
		remove_child (*Children[0].Child);
	}

	add_child (c, 0, 0);
	if (refocus) focus();
}

// set the scroll mode
void scrollview::set_scroll_mode (const scroll_mode & m)
{
	ScrollMode = m;
	if (!Children.empty())
	{
		gui::layout *container = (gui::layout*) Children[0].Child;
		container->set_auto_grow ((resize_mode) m);
	}
}

// make sure the given widget is visible in the view
void scrollview::show (const gui::widget & c)
{
	gui::layout *container = (gui::layout*) Children[0].Child;
	gfx::drawing_area pos = container->get_location (c);

    // space occupied by scroll bars
    u_int16 xo = ((ScrollMode & SCROLL_Y) == SCROLL_Y) ? VScroll->length() : 0;
    u_int16 yo = ((ScrollMode & SCROLL_X) == SCROLL_X) ? HScroll->height() : 0;

    // space available for child
    gfx::drawing_area client_area (0, 0, length() -xo, height() - yo);
    client_area.shrink (Look->border ());

	if ((ScrollMode & SCROLL_X) == SCROLL_X)
	{
		if (pos.x() < Ox)
		{
			Ox = pos.x();
		}
		else if (pos.x() + pos.length() > Ox + client_area.length())
		{
			Ox = pos.x() + pos.length() - client_area.length();
		}
	}

	if ((ScrollMode & SCROLL_Y) == SCROLL_Y)
	{
		if (pos.y() < Oy)
		{
            Oy = pos.y();
		}
		else if (pos.y() + pos.height() > Oy + client_area.height())
		{
			Oy = pos.y() + pos.height() - client_area.height();
		}
	}
}

// center the given widget in the view
void scrollview::center (const gui::widget & c)
{
	gui::layout *container = (gui::layout*) Children[0].Child;
	gfx::drawing_area pos = container->get_location (c);

    // space occupied by scroll bars
    u_int16 xo = ((ScrollMode & SCROLL_Y) == SCROLL_Y) ? VScroll->length() : 0;
    u_int16 yo = ((ScrollMode & SCROLL_X) == SCROLL_X) ? HScroll->height() : 0;

    // space available for child
    gfx::drawing_area client_area (0, 0, length() -xo, height() - yo);
    client_area.shrink (Look->border ());

	if ((ScrollMode & SCROLL_X) == SCROLL_X)
	{
		Ox = pos.x() + (pos.length() - client_area.length()) / 2;
	}

	if ((ScrollMode & SCROLL_Y) == SCROLL_Y)
	{
		Oy = pos.y() + (pos.height() - client_area.height()) / 2;
	}
}

// update scroll offset
bool scrollview::keydown(input::keyboard_event & k)
{
	bool consumed = layout::keydown (k);
	if (consumed)
	{
		if (k.key() == input::keyboard_event::LEFT_KEY ||
			k.key() == input::keyboard_event::RIGHT_KEY ||
			k.key() == input::keyboard_event::TAB_KEY ||
			k.key() == input::keyboard_event::DOWN_KEY ||
			k.key() == input::keyboard_event::UP_KEY)
		{
			gui::layout *container = (gui::layout*) Children[0].Child;
			show (container->get_child (container->current()));
		}
	}

	return consumed;
}

