/*
   $Id: gfx.cc,v 1.1 2003/07/18 15:16:09 gnurou Exp $

   Copyright (C) 2003  Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

/**
 * @file   gfx.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the gfx module init and cleanup functions,
 *         as well as the surface factory.
 * 
 * 
 */

#include "base/paths.h"
#include "gfx/gfx.h"

#include "ltdl.h"
#include <iostream>

/**
 * The handler of our library file.
 * 
 */
static lt_dlhandle dlhandle = 0;

/**
 * Virtual backend init function.
 * 
 */
static bool (*gfxinit)() = 0;

/**
 * Virtual backend cleanup function.
 * 
 */
static void (*gfxcleanup)() = 0;

/**
 * Virtual backend surface creation function.
 * 
 */
static gfx::surface * (*create_surface_p)() = 0;

bool gfx::init(const std::string & backend_name)
{
    if (lt_dlinit())
    {
        std::cerr << lt_dlerror() << std::endl;
        std::cerr << "Error initializing liblt!\n"; 
        return false;
    }

//     dlhandle = lt_dlopenext(libname.c_str());
    dlhandle = base::get_module(std::string("/gfx/") + backend_name);
    
    if (!dlhandle) goto bigerror;
        
    gfxinit = (bool(*)()) lt_dlsym(dlhandle, "gfx_init");
    if (!gfxinit)
    {
        std::cerr << lt_dlerror() << std::endl;
        goto bigerror;
    }
    
    gfxcleanup = (void(*)()) lt_dlsym(dlhandle, "gfx_cleanup");
    if (!gfxcleanup)
    {
        std::cerr << lt_dlerror() << std::endl;
        goto bigerror;
    }
    
    screen::set_video_mode_p = (bool(*)(u_int16, u_int16, u_int8)) lt_dlsym(dlhandle, "gfx_screen_set_video_mode");
    if (!screen::set_video_mode_p)
    {
        std::cerr << lt_dlerror() << std::endl;
        goto bigerror;
    }

    screen::update_p = (void(*)()) lt_dlsym(dlhandle, "gfx_screen_update");
    if (!screen::update_p)
    {
        std::cerr << lt_dlerror() << std::endl;
        goto bigerror;
    }

    screen::trans_color_p = (u_int32(*)()) lt_dlsym(dlhandle, "gfx_screen_trans_color");
    if (!screen::trans_color_p)
    {
        std::cerr << lt_dlerror() << std::endl;
        goto bigerror;
    }

    screen::clear_p = (void(*)()) lt_dlsym(dlhandle, "gfx_screen_clear");
    if (!screen::clear_p)
    {
        std::cerr << lt_dlerror() << std::endl;
        goto bigerror;
    }

    create_surface_p = (gfx::surface *(*)()) lt_dlsym(dlhandle, "gfx_create_surface");
    if (!create_surface_p)
    {
        std::cerr << lt_dlerror() << std::endl;
        goto bigerror;
    }

    goto success;
    
 bigerror:
    if (dlhandle) lt_dlclose(dlhandle);
    lt_dlexit();
    return false;

 success:        
    return gfxinit();
}

void gfx::cleanup()
{
    if (gfxcleanup) gfxcleanup();
    gfxcleanup = NULL;

    if (dlhandle) lt_dlclose(dlhandle);
    lt_dlexit();
}

gfx::surface * gfx::create_surface()
{
    return create_surface_p();
}
