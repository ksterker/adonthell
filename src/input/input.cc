/*
   $Id: input.cc,v 1.2 2003/07/24 12:57:58 gnurou Exp $

   Copyright (C) 2003  Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

/**
 * @file   input.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the inpuy module init and cleanup functions.
 * 
 * 
 */

#include "input/input.h"

#include "ltdl.h"
#include <iostream>

#include "base/paths.h"

/**
 * The handler of our library file.
 * 
 */
static lt_dlhandle dlhandle = 0;

/**
 * Virtual backend init function.
 * 
 */
static bool (*inputinit)() = 0;

/**
 * Virtual backend cleanup function.
 * 
 */
static void (*inputcleanup)() = 0;

namespace input
{
    bool init(const std::string & backend_name)
    {
        if (lt_dlinit()) 
        { 
            std::cerr << lt_dlerror() << std::endl;
            std::cerr << "Error initializing liblt!\n"; 
            return false;
        }

        dlhandle = base::get_module(std::string("/input/") + backend_name);
    
        if (!dlhandle) goto bigerror;

        inputinit = (bool(*)()) lt_dlsym(dlhandle, "input_init");
        if (!inputinit)
        {
            std::cerr << lt_dlerror() << std::endl;
            goto bigerror;
        }
    
        inputcleanup = (void(*)()) lt_dlsym(dlhandle, "input_cleanup");
        if (!inputcleanup)
        {
            std::cerr << lt_dlerror() << std::endl;
            goto bigerror;
        }

        input::manager::update_p = (void(*)()) lt_dlsym(dlhandle, "input_manager_update");
        if (!input::manager::update_p)
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
        
        return inputinit();
    }

    void cleanup()
    {
        if (inputcleanup) inputcleanup();
        inputcleanup = NULL;

        if (dlhandle) lt_dlclose(dlhandle);
        lt_dlexit();
    }
}
