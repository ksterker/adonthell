/*
   $Id: paths.cc,v 1.1 2003/07/18 15:16:09 gnurou Exp $

   Copyright (C) 2003  Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

/**
 * @file   base/paths.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines some primitives to get essential game paths.
 * 
 * 
 */

#include <cstdlib>
#include <iostream>

#include "base/paths.h"

#define MODULES_ENV "ADONTHELL_MODULES_PATH"

lt_dlhandle base::get_module(const std::string & modname)
{
    lt_dlhandle ret;

    /* Try the MODULES_ENV variable first */
    const char * mod_env_str = getenv(MODULES_ENV);
    std::string mod_env = (mod_env_str ? mod_env_str : "");

    if (!mod_env.empty())
    {
        mod_env += modname;
        ret = lt_dlopenext(mod_env.c_str());
        if (ret) return ret;
    }

    /* Try the hard-coded path then */
    mod_env = PKGLIBDIR + modname;
    ret = lt_dlopenext(mod_env.c_str());
    if (ret) return ret;

    std::cerr << "Failed to load module " << modname << ": " << lt_dlerror() << std::endl;

    return NULL;
}
