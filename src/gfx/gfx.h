/*
   $Id: gfx.h,v 1.1 2003/07/18 15:16:09 gnurou Exp $

   Copyright (C) 2003  Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
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

#include "gfx/drawing_area.h"
#include "gfx/drawable.h"
#include "gfx/surface.h"
#include "gfx/screen.h"

/**
 * All the low-level graphical tasks are handled by this module.
 * 
 */
namespace gfx
{
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
    gfx::surface * create_surface();
}


#endif
