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
 * @file   gui/scrollview.h
 * @author Kai Sterker <kai.sterker@gmail.com>
 * @brief  Defines the scrollview class.
 */


#ifndef GUI_SCROLLVIEW_H
#define GUI_SCROLLVIEW_H

#include "layout.h"
#include "indicatorbar.h"

namespace gui
{

#ifndef SWIG
/// width of scrollbars in pixel
#define SCROLLBAR_WIDTH 14
#endif

/**
 * This container has a single element which
 * can be larger than the container itself. It
 * will ensure that the focused child of the
 * contained element will be visible.
 */
class scrollview : public layout
{
public:

	typedef enum
	{
		SCROLL_X = 1,
		SCROLL_Y = 2,
		SCROLL_BOTH = SCROLL_X | SCROLL_Y
	} scroll_mode;

    /**
     * Create a new, empty scroll view with the given size.
     * @param l layout length
     * @param h layout height
     */
	scrollview (const u_int16 & l, const u_int16 & h)
	: layout(l, h), Ox(0), Oy(0), HScroll (NULL), VScroll (NULL)
    {
		VScroll = new indicator_bar (SCROLLBAR_WIDTH, 0);
		HScroll = new indicator_bar (0, SCROLLBAR_WIDTH);
		HScroll->set_orientation (indicator_bar::HORIZONTAL);

		ScrollMode = SCROLL_Y;
        ResizeMode = NONE;
    }

    /**
     * Create scroll view widget from the given data file.
     * Size will be set to the background size.
     * @param style filename of widget decoration.
     */
	scrollview (const std::string & style)
	: layout(style), Ox(0), Oy(0), HScroll (NULL), VScroll (NULL)
	{
		VScroll = new indicator_bar (SCROLLBAR_WIDTH, 0);
		HScroll = new indicator_bar (0, SCROLLBAR_WIDTH);
		HScroll->set_orientation (indicator_bar::HORIZONTAL);

		ScrollMode = SCROLL_Y;
        ResizeMode = NONE;
	}

	/**
	 * Cleanup.
	 */
	virtual ~scrollview()
	{
		delete HScroll;
		delete VScroll;
	}

	/**
	 * Set the widget to scroll.
	 * @param c the widget to scroll.
	 */
	void set_child (layout & c);

	/**
	 * @name Scrolling
	 */
	//@{
	/**
	 * Resets the view to the origin.
	 */
	void reset ()
	{
		Ox = 0;
		Oy = 0;
	}

	/**
	 * Make sure the given widget is visible in the view.
	 * Will scroll just enough to bring the widget to the
	 * edge of the view that is nearest to its current
	 * position.
	 * @param c the widget to show.
	 */
	void show (const gui::widget & c);

	/**
	 * Center the view on the given widget.
	 * @param c the widget to center on.
	 */
	void center (const gui::widget & c);

	/**
	 * Sets the behaviour of the scroll view.
	 * @param m behaviour of the scroll view.
	 */
	void set_scroll_mode (const scroll_mode & m);

	/**
	 * Sets the look of the scroll bars.
	 * @param style filename of scroll bar decoration.
	 */
	void set_scroll_style (const std::string & style)
	{
		HScroll->set_style (style);
		VScroll->set_style (style);
	}
	//@}

    /**
     * Draw the object on the %screen.
     *
     * @param x X position where to draw.
     * @param y Y position where to draw.
     * @param da optional drawing_area to use during the drawing operation.
     * @param target pointer to the surface where to draw the drawable. If NULL,
     * draw on the screen.
     */
    virtual void draw(const s_int16 & x, const s_int16 & y, const gfx::drawing_area * da = NULL, gfx::surface * target = NULL) const;

#ifndef SWIG
    GET_TYPE_NAME_VIRTUAL(gui::scrollview);
#endif

protected:
    /**
     * Called when a key has been pressed by the user. Updates
     * the scroll offset.
     * @param k the keyboard event.
     * @return true if the event was consumed, false otherwise.
     */
    virtual bool keydown(input::keyboard_event & k);

private:
    /// horizontal scroll offset
    s_int32 Ox;
    /// vertical scroll offset
    s_int32 Oy;
    /// directions it is possible to scroll
    scroll_mode ScrollMode;

    /// horizontal scrollbar
    indicator_bar *HScroll;
    /// vertical scrollbar
    indicator_bar *VScroll;

    /// hide from the public interface
	using layout::add_child;
    /// hide from the public interface
	using layout::remove_child;
};

}

#endif /* GUI_SCROLLVIEW_H */
