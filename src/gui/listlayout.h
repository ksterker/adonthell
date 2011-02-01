/*
 Copyright (C) 2011 Kai Sterker <kai.sterker@gmail.com>
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
 * @file   gui/list_layout.h
 * @author Kai Sterker
 * @brief  Declares the list layout class.
 */

#ifndef GUI_LISTLAYOUT_H
#define GUI_LISTLAYOUT_H

#include "gui/layout.h"

namespace gui
{

/**
 * A layout that arranges its elements automatically as either a
 * vertical (default) or horizontal list, depending on the resize
 * mode.
 */
class list_layout : public layout
{
public:
    /**
     * Create a new, empty list layout with the given size.
     * By default, items will be added below each other.
     * @param l layout length
     * @param h layout height
     */
    list_layout (const u_int16 & l, const u_int16 & h)
    : layout(l, h)
    {
        ResizeMode = GROW_Y;
    }

    /**
     * Create list layout widget with the given decoration.
     * Size will be set to that of the background image.
     * By default, items will be added below each other.
     * @param style filename of widget decoration.
     */
    list_layout (const std::string & style)
    : layout(style)
    {
        ResizeMode = GROW_Y;
    }

    /**
     * Adds a new child to the end of the layout,
     * growing the layout as necessary.
     * @param c the child to add.
     */
    void add_child (widget & c);

    /**
     * Inserts a new child at the given position,
     * moving subsequent children as necessary.
     * @param c the child to add.
     * @param pos the index at which to add the new element.
     */
    void insert_child (widget & c, const u_int32 & pos);

    /**
     * Resize the layout to accommodate for its children.
     * May grow or shrink the layout so that all children fit.
     * @param mode the growth mode.
     */
    virtual void resize (const gui::layout::resize_mode & mode = NONE);

protected:
    /**
     * Called when the user pressed the move right key.
     * @return true if a child became the newly focused element.
     */
    virtual bool moveright();
    /**
     * Called when the user pressed the move left key.
     * @return true if a child became the newly focused element.
     */
    virtual bool moveleft();
    /**
     * Called when the user pressed the move up key.
     * @return true if a child became the newly focused element.
     */
    virtual bool moveup();
    /**
     * Called when the user pressed the move down key.
     * @return true if a child became the newly focused element.
     */
    virtual bool movedown();

    /// hide from the public interface
    using layout::add_child;
};

}

#endif /* GUI_LISTLAYOUT_H */
