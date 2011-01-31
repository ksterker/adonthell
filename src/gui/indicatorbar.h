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
 * @file gui/indicatorbar.h
 *
 * @author Kai Sterker
 * @brief Progress and scroll indicator.
 */

#ifndef GUI_INDICATORBAR_H
#define GUI_INDICATORBAR_H

#include "gui/widget.h"

namespace gui
{

#ifndef SWIG

/**
 * Decoration for the indicator bar widget.
 */
class indicator_decoration : public std_decoration
{
public:
    /**
     * Initialize from the given record.
     */
    virtual bool init (base::flat & record);

    /**
     * Draw indicator.
     *
     * @param x offset in x direction.
     * @param y offset in y direction.
     * @param da clipping rectangle.
     * @param target target surface.
     * @param highlight whether to draw with highlight.
     */
    virtual void draw_background (const s_int16 & x, const s_int16 & y, const gfx::drawing_area * da_opt, gfx::surface * target, const bool & highlight = false);

private:
    /// the different elements as stored in the elements vector
    enum { V_START, V_MID, V_END, V_HANDLE, H_START, H_MID, H_END, H_HANDLE };
};

/**
 * Decorator factory for the indicator bar widget.
 */
class indicator_factory : public decoration_factory
{
public:
	virtual std_decoration *create_decorator(const std::string & state)
	{
		return new gui::indicator_decoration();
	}
};

#endif

/**
 * A widget that can display the ratio between
 * two ranges. With the proper style it can either
 * become a progress or scroll bar.
 */
class indicator_bar : public widget
{
public:
	typedef enum
	{
		VERTICAL,
		HORIZONTAL
	} orientation;

    /**
     * Create widget of given size.
     * @param width the initial width.
     * @param height the inital height.
     */
	indicator_bar(const u_int16 & width, const u_int16 & height)
	: widget (width, height), Min(0), Max(100), Lower(0), Upper(100)
	{
		Slider = new decoration (new indicator_factory ());
		Orientation = VERTICAL;
	}

    /**
     * Create widget from the given data file.
     * Size will be set to the background size.
     * @param style filename of widget decoration.
     */
	indicator_bar(const std::string & style)
	: widget (style), Min(0), Max(100), Lower(0), Upper(100)
	{
		Slider = new decoration (new indicator_factory ());
		Slider->init (style);
		Orientation = VERTICAL;
	}

	/**
	 * Free resources used by indicator.
	 */
	virtual ~indicator_bar()
	{
		delete Slider;
	}

	/**
	 * Set or change the style of the widget.
	 * @param style the name of the new style.
	 */
	virtual void set_style (const std::string & style)
	{
		widget::set_style (style);
		Slider->init (style);
	}

	/**
	 * Set whether the indicator should be oriented
	 * vertically or horizontally. This will influence
	 * the rendering of the fill level. Default orientation
	 * is VERTICAL.
	 * @param o the new orientation.
	 */
	void set_orientation (const orientation & o) { Orientation = o; }

	/**
	 * Value range.
	 */
	//@{
	/**
	 * Set the absolute minimum value of the indicator.
	 * @param min the minimum value.
	 */
	void set_min (const u_int32 & min) { Min = min; }

	/**
	 * Set the absolute maximum value of the indicator.
	 * @param max the maximum value.
	 */
	void set_max (const u_int32 & max) { Max = max; }
	//@}

	/**
	 * @name Current indicator values
	 */
	//@{
	/**
	 * Set a new indicator position, relative
	 * to its absolute min and max values.
	 * @param lower lower indicator value.
	 * @param upper upper indicator value.
	 */
	void set_values (const u_int32 & lower, const u_int32 & upper)
	{
		if (lower < Min) Lower = Min;
		else Lower = lower;

		if (upper > Max) Upper = Max;
		else Upper = upper;
	}

	/**
	 * Set the lower position of the indicator, relative
	 * to its absolute min and max values.
	 * @param lower lower indicator value.
	 */
	void set_lower_val (const u_int32 & lower)
	{
		if (lower < Min) Lower = Min;
		else if (lower > Upper) Lower = Upper;
		else Lower = lower;
	}

	/**
	 * Set the upper position of the indicator, relative
	 * to its absolute min and max values.
	 * @param upper upper indicator value.
	 */
	void set_upper_val (const u_int32 & upper)
	{
		if (upper < Lower) Upper = Lower;
		else if (upper > Max) Upper = Max;
		else Upper = upper;
	}
	//@}

    /**
     * Draw the object on the %screen.
     *
     * @param x X position where to draw.
     * @param y Y position where to draw.
     * @param da optional drawing_area to use during the drawing operation.
     * @param target pointer to the surface where to draw the drawable. If NULL,
     *               draw on the screen.
     */
    virtual void draw (const s_int16 & x, const s_int16 & y, const gfx::drawing_area * da = NULL, gfx::surface * target = NULL) const;

#ifndef SWIG
        GET_TYPE_NAME_VIRTUAL(gui::indicator_bar);
#endif

protected:
    /// the active part of the indicator bar
    decoration *Slider;
    /// the widgets orientation
    orientation Orientation;
	/// the minimum value for the indicator
	u_int32 Min;
	/// the maximum value for the indicator
	u_int32 Max;
	/// the lower value of the indicator
	u_int32 Lower;
	/// the upper value of the indicator
	u_int32 Upper;
};

}

#endif /* GUI_INDICATORBAR_H */
