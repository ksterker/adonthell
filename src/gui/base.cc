/*
   $Id: base.cc,v 1.7 2004/02/08 22:25:07 ksterker Exp $

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
#include "input/manager.h"

#include "gui/container.h"
#include "gui/base.h"


//using namespace gui;


u_int32 gui::base::gui_objects = 0;

gui::base::base (): drawing_area () {

  m_id = gui_objects++;
#ifdef DEBUG
  std::cerr  << "GUI Allocated object: " << (u_int32) gui_objects << std::endl;
#endif

  m_listener = NULL;
  m_parent = NULL;
  m_x = m_y = m_padx = m_pady = 0;
  m_vertical_align = m_horizontal_align = base::ALIGN_NONE;
  m_selectable = m_visible = m_enabled = true;
  m_focus = m_selected = false;
}


void gui::base::setLocation (s_int16 nx, s_int16 ny) {
  m_x = nx;
  m_y = ny;

  updatePosition ();
}

void gui::base::setSize (u_int16 nl, u_int16 nh)
{
  gfx::drawing_area::resize (nl, nh);
  
  updateSize ();
}

void gui::base::updatePosition ()
{
  if (m_parent) 
    gfx::drawing_area::move (m_parent->getRealX () + getX () + getPadX (),
			     m_parent->getRealY () + getY () + getPadY ());
  else gfx::drawing_area::move (getX () + getPadX (), getY () + getPadY ());
}

bool gui::base::update() {
  return true;
}


bool gui::base::draw(gfx::surface * sf, gfx::drawing_area * da ) {
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


bool gui::base::drawContents(gfx::surface * sf) {
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

void gui::base::updateSize () {
}

void gui::base::setParent (container * parent) { 
  m_parent = parent;
  updatePosition ();
}


gfx::drawing_area * gui::base::getParentDrawingArea ()
{
  if (m_parent) return m_parent->getDrawingArea ();
  return getDrawingArea ();
}

void gui::base::setVerticalAlign (u_int8 align)
{
  m_vertical_align = align;
  // TODO
  std::cout << "setVerticalAlign:: TODO\n";
}

void gui::base::setHorizontalAlign (u_int8 align)
{
  m_horizontal_align = align;
  // TODO
  std::cout << "setHorizontalAlign:: TODO\n";
}

void gui::base::setListener(::input::listener * list, u_int8 device) {
  if(m_listener) {
    input::manager::remove (m_listener);
    delete m_listener;
  }
  m_listener = list;

  if (m_listener) {
    if (device & KEYBOARD_DEVICE) m_listener->connect_keyboard_function(::base::make_functor_ret(*this, &base::on_keyboard_event));
    else if (device & MOUSE_DEVICE) m_listener->connect_mouse_function(::base::make_functor_ret(*this, &base::on_mouse_event));
    else if (device & JOYSTICK_DEVICE) m_listener->connect_joystick_function(::base::make_functor_ret(*this, &base::on_joystick_event));
    else if (device & CONTROL_DEVICE) m_listener->connect_control_function(::base::make_functor_ret(*this, &base::on_control_event));
    input::manager::add(m_listener);
  }
}

::input::listener * gui::base::getListener () const {
  return m_listener;
}

bool gui::base::on_mouse_event (input::mouse_event * evt) {
  return false;
}

bool gui::base::on_keyboard_event (input::keyboard_event * evt) {
  return false;
}
      
bool gui::base::on_joystick_event (input::joystick_event * evt) {
  return false;
}
      
bool gui::base::on_control_event (input::control_event * evt) {
  return false;
}

gui::base::~base () {
  setListener (NULL);
  --gui_objects;

#ifdef DEBUG
  std::cerr  << "GUI Allocated object: " << (u_int32) gui_objects << std::endl;
#endif
}
