/*
   $Id: base.cc,v 1.4 2004/02/05 21:52:38 jol Exp $

   Copyright (C) 1999/2000/2001/2002   Alexandre Courbot <alexandrecourbot@linuxgames.com>
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

#include <iostream>
#include <assert.h>

#include "gfx/screen.h"
#include "gfx/surface.h"
#include "gui/container.h"
#include "gui/base.h"


using namespace gui;


base::base (): drawing_area () {
  m_parent = NULL;
  m_x = m_y = m_padx = m_pady = 0;
  m_vertical_align = m_horizontal_align = base::ALIGN_NONE;
  m_selectable = m_visible = m_enabled = true;
  m_focus = m_selected = false;
}


void base::setLocation (s_int16 nx, s_int16 ny)
{
  m_x = nx;
  m_y = ny;

  updatePosition ();
}

void base::setSize (u_int16 nl, u_int16 nh)
{
  gfx::drawing_area::resize (nl, nh);
  
  updateSize ();
}

void base::updatePosition ()
{
  if (m_parent) 
    gfx::drawing_area::move (m_parent->getRealX () + getX () + getPadX (),
			     m_parent->getRealY () + getY () + getPadY ());
  else gfx::drawing_area::move (getX () + getPadX (), getY () + getPadY ());
}

bool base::update() {
  return true;
}


bool base::draw(gfx::surface * sf, gfx::drawing_area * da ) {
  if (m_visible) {
    bool b;
    assignArea(da);
    if (sf) b = drawContents (sf);
    else b = drawContents (gfx::screen::get_surface());
    detachArea();
    return b;
  }
  return false;
}


bool base::drawContents(gfx::surface * sf) {
  assert (sf != NULL);
  
  //top
  sf->draw_line (getRealX(), getRealY(), 
		getRealX () + getLength () - 1, getRealY (), 0x888899, this);
  //bottom
  sf->draw_line (getRealX(), getRealY() + getHeight () - 1, 
		getRealX () + getLength () - 1, getRealY () + getHeight () - 1, 0x888899, this);
  //left
  sf->draw_line (getRealX(), getRealY(), 
		getRealX (), getRealY () + getHeight () - 1, 0x888899, this);
  //right
  sf->draw_line (getRealX() + getLength () - 1, getRealY(), 
		getRealX () + getLength () - 1, getRealY () + getHeight () - 1, 0x888899, this);
  return true;
}

void base::updateSize () {
}

void base::setParent (container * parent) { 
  m_parent = parent;
  updatePosition ();
}


gfx::drawing_area * base::getParentDrawingArea ()
{
  if (m_parent) return m_parent->getDrawingArea ();
  return getDrawingArea ();
}

void base::setVerticalAlign (u_int8 align)
{
  m_vertical_align = align;
  // TODO
  std::cout << "setVerticalAlign:: TODO\n";
}

void base::setHorizontalAlign (u_int8 align)
{
  m_horizontal_align = align;
  // TODO
  std::cout << "setHorizontalAlign:: TODO\n";
}


base::~base ()
{
  if (m_parent) m_parent->removeChild (this);
}
