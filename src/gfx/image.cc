/*
 $Id: image.cc,v 1.3 2007/06/16 23:19:35 ksterker Exp $
 
 Copyright (C) 1999/2000/2001/2002 Alexandre Courbot <alexandrecourbot@linuxgames.com>
 Copyright (C) 2007 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file   gfx/image.cc 
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Declares the image class.
 *  
 */

#include "gfx/image.h"

using gfx::image;
using gfx::surface;
using gfx::drawing_area;

// ctor
image::image (const u_int16 & length, const u_int16 & height) : drawable ()
{
    Image = gfx::create_surface();
    Image->resize (length, height);
    
    set_length (length);
    set_height (height);
}

image::~image ()
{
    delete Image;
}

// draw image
void image::draw (s_int16 x, s_int16 y, const drawing_area * da_opt, surface * target) const
{
    Image->draw (x, y, 0, 0, length(), height(), da_opt, target);
}
