/*
   $Id: gfx.h,v 1.5 2006/06/03 04:20:27 Mithander Exp $

   Copyright (C) 2003  Alexandre Courbot <alexandrecourbot@linuxgames.com>
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
 * @file   gfx/gfx.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 *
 * @brief  The gfx module main include file.
 *
 *
 */

#ifndef GFX_H_
#define GFX_H_

#include <string>
#include "gfx/screen.h"

/**
 * All the low-level graphical tasks are handled by this module.
 *
 */
namespace gfx
{
    class surface;
    class drawable;

    /**
     * Initialize the gfx module. This function \e must be called
     * before doing anything gfx-related.
     *
     * @param backend_name Name of the graphical backend to use.
     *
     * @return \e true in case of success, \e false otherwise.
     */
    bool init(const std::string & backend_name);

    /**
     * Cleanup the gfx module - call this function once you've
     * finished using it.
     *
     */
    void cleanup();

#ifdef SWIG
    /*
      SWIG directive - create_surface returns a new
      surface, not a pointer to an existing one.
    */
    %newobject create_surface;
#endif
    /**
     * Return a new surface. Surfaces can \e only be created
     * by this function, and you have the responsability
     * to free them when no longer needed.
     *
     * @return a new surface (NULL if failed) corresponding to
     * the current backend.
     */
    surface * create_surface();

    /**
     * Return a cached surface object, or load one if not already cached
     *
     * @param file which file to return a cached version of
     *
     * @return a pointer to an existing drawable object.  Do not delete it.
     */
    const surface * surface_cache(const std::string & file, bool set_mask = true, bool invert_x = false);
}


#endif
