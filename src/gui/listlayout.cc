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
 * @file   gui/list_layout.cc
 * @author Kai Sterker
 * @brief  Implements the list layout class.
 */

#include "listlayout.h"

using gui::list_layout;
using gui::layout;

// distance between two list elements
#define SPACING 2

// add element to end of list
void list_layout::add_child (widget & c)
{
    insert_child(c, num_children());
    layout::resize(ResizeMode);
}

// insert element in list
void list_layout::insert_child (widget & c, const u_int32 & pos)
{
    // insert new element
    gfx::drawing_area a (0, 0, c.length(), c.height());
    Children.insert (Children.begin() + pos, layoutchild(&c, a));

    // update list
    resize();
}

// update the list
void list_layout::resize (const gui::layout::resize_mode & mode)
{
    u_int32 x = SPACING + Look->border().x();
    u_int32 y = SPACING + Look->border().y();

    vector<layoutchild>::iterator i;
    for (i = Children.begin(); i != Children.end(); i++)
    {
        i->Pos.resize(i->Child->length(), i->Child->height());
        i->Pos.move(x, y);

        if (ResizeMode == GROW_Y)
        {
            y += i->Pos.height() + SPACING;
        }
        else
        {
            x += i->Pos.length() + SPACING;
        }
    }

    if ((ResizeMode & GROW_X) == 0) x = length();
    if ((ResizeMode & GROW_Y) == 0) y = height();

    // update widget size, if required
    if (x != length() || y != height()) set_size (x, y);
}

// select next child
bool list_layout::moveright()
{
    if (ResizeMode == GROW_X)
    {
        return layout::moveright();
    }

    return false;
}

// select next child
bool list_layout::movedown()
{
    if (ResizeMode == GROW_Y)
    {
        return layout::movedown();
    }

    return false;
}

// select previous child
bool list_layout::moveleft()
{
    if (ResizeMode == GROW_X)
    {
        return layout::moveleft();
    }

    return false;
}

// select previous child
bool list_layout::moveup()
{
    if (ResizeMode == GROW_Y)
    {
        return layout::moveup();
    }

    return false;
}
