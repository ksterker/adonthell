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
 * @file gui/canvas.h
 *
 * @author Kai Sterker
 * @brief Defines the canvas widget.
 */

#ifndef GUI_CANVAS_H
#define GUI_CANVAS_H

#include "gfx/drawable.h"
#include "gui/widget.h"

namespace gui
{

#ifndef SWIG

/**
 * Decoration for the canvas widget.
 */
class canvas_decoration : public std_decoration
{
public:
	/**
	 * Create canvas decoration.
	 * @param element the drawable pointer of the canvas.
	 */
	canvas_decoration (const gfx::drawable **element)
	{
		Element = element;
	}

	/**
	 * Cleanup.
	 */
	virtual ~canvas_decoration() {}

	/**
	 * Render the canvas background and element.
	 *
     * @param x offset in x direction.
     * @param y offset in y direction.
     * @param da clipping rectangle.
     * @param target target surface.
     * @param highlight whether to draw with highlight.
	 */
	virtual void draw_background (const s_int16 & x, const s_int16 & y, const gfx::drawing_area * da_opt, gfx::surface * target, const bool & highlight = false);

private:
	/// pointer to the drawable pointer of the canvas widget
	const gfx::drawable **Element;
};

/**
 * Decorator factory for the canvas widget. Used to pass
 * through the actual drawable that needs to be rendered
 * to the decoration.
 */
class canvas_factory : public decoration_factory
{
public:
	/**
	 * Create canvas decoration factory.
	 * @param element the drawable pointer of the canvas.
	 */
	canvas_factory (const gfx::drawable **element)
	{
		Element = element;
	}

	/**
	 * Create UI decoration for a given state.
	 * @param state the state for which to create the decoration.
	 */
	virtual canvas_decoration *create_decorator(const std::string & state)
	{
		return new canvas_decoration (Element);
	}

private:
	/// pointer to the drawable pointer of the canvas widget
	const gfx::drawable **Element;
};

#endif

/**
 * Class that can render any gfx::drawable with
 * optional border and background. The drawable
 * will be centered on the canvas, respectively
 * only the center part of it will be shown in
 * case it is larger than the widget.
 */
class canvas : public widget
{
public:
    /**
     * Create canvas of given size.
     * @param width the initial width.
     * @param height the inital height.
     */
	canvas (const u_int16 & width, const u_int16 & height)
    : widget(), Drawable(NULL)
    {
        Look = new decoration (new canvas_factory (&Drawable));
        set_size (width, height);
    }

    /**
     * Create canvas from the given data file.
     * Size will be set to the background size.
     * @param style filename of widget decoration.
     */
	canvas (const std::string & style)
    : widget(), Drawable(NULL)
    {
        u_int16 w = 0, h = 0;

        Look = new decoration (new canvas_factory (&Drawable));
        if (Look->init (style))
        {
            Look->get_size (w, h);
        }

        set_length (w);
        set_height (h);
    }

	/**
	 * Destroy canvas widget.
	 */
	virtual ~canvas()
	{ }

	/**
	 * Set the drawable to display in this widget.
	 * The canvas will render the drawable, but the
	 * caller remains responsible for its lifecycle
	 * and any updates to its internal state.
	 * @param drawable the drawable to display.
	 */
	void set_drawable (const gfx::drawable & drawable)
	{
		Drawable = &drawable;
	}

#ifndef SWIG
    GET_TYPE_NAME_VIRTUAL(gui::canvas);
#endif

private:
	/// the drawable to render onto the canvas
	const gfx::drawable *Drawable;
};

}

#endif /* GUI_CANVAS_H */
