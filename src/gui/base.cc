#include <iostream>

#include "gfx/screen.h"
#include "gfx/surface.h"
#include "gui/container.h"
#include "gui/base.h"


using namespace gui;


base::base (): drawing_area ()
{
  m_parent = NULL;

  m_x = m_y = m_padx = m_pady = 0;

  m_vertical_align = m_horizontal_align = base::ALIGN_NONE;

  m_visible = m_enable = true;
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

bool base::update ()
{
  return true;
}


bool base::draw ()
{
  if (m_visible)
    {
      return drawContents ();
    }
  return false;
}


bool base::drawContents ()
{
  gfx::surface * s = gfx::screen::get_surface();
  if (s == NULL) return false;

  //top
  s->draw_line (getRealX(), getRealY(), 
		getRealX () + getLength () - 1, getRealY (), 0x888899, this);
  //bottom
  s->draw_line (getRealX(), getRealY() + getHeight () - 1, 
		getRealX () + getLength () - 1, getRealY () + getHeight () - 1, 0x888899, this);
  //left
  s->draw_line (getRealX(), getRealY(), 
		getRealX (), getRealY () + getHeight () - 1, 0x888899, this);
  //right
  s->draw_line (getRealX() + getLength () - 1, getRealY(), 
		getRealX () + getLength () - 1, getRealY () + getHeight () - 1, 0x888899, this);
  return true;
}

void base::updateSize ()
{
}

void base::setParent (container * parent)
{ 
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



