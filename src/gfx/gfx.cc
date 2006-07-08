/*
   $Id: gfx.cc,v 1.7 2006/07/08 17:04:03 ksterker Exp $

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
 * @file   gfx.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 *
 * @brief  Defines the gfx module init and cleanup functions,
 *         as well as the surface factory.
 *
 *
 */

#include <map>
#include "base/paths.h"
#include "gfx/gfx.h"

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

using namespace std;

namespace gfx
{
    bool init(const string & backend_name)
    {
        if (lt_dlinit())
        {
            cerr << lt_dlerror() << endl;
            cerr << "Error initializing liblt!\n";
            return false;
        }

        //     dlhandle = lt_dlopenext(libname.c_str());
        dlhandle = base::get_module(string("/gfx/") + backend_name);

        if (!dlhandle) goto bigerror;

        gfxinit = (bool(*)()) lt_dlsym(dlhandle, "gfx_init");
        if (!gfxinit)
        {
            cerr << lt_dlerror() << endl;
            goto bigerror;
        }

        gfxcleanup = (void(*)()) lt_dlsym(dlhandle, "gfx_cleanup");
        if (!gfxcleanup)
        {
            cerr << lt_dlerror() << endl;
            goto bigerror;
        }

        screen::set_video_mode_p = (bool(*)(u_int16, u_int16, u_int8)) lt_dlsym(dlhandle, "gfx_screen_set_video_mode");
        if (!screen::set_video_mode_p)
        {
            cerr << lt_dlerror() << endl;
            goto bigerror;
        }

        screen::update_p = (void(*)()) lt_dlsym(dlhandle, "gfx_screen_update");
        if (!screen::update_p)
        {
            cerr << lt_dlerror() << endl;
            goto bigerror;
        }

        screen::trans_color_p = (u_int32(*)()) lt_dlsym(dlhandle, "gfx_screen_trans_color");
        if (!screen::trans_color_p)
        {
            cerr << lt_dlerror() << endl;
            goto bigerror;
        }

        screen::clear_p = (void(*)()) lt_dlsym(dlhandle, "gfx_screen_clear");
        if (!screen::clear_p)
        {
            cerr << lt_dlerror() << endl;
            goto bigerror;
        }

        screen::get_surface_p = (surface * (*)()) lt_dlsym(dlhandle, "gfx_screen_get_surface");
        if (!screen::get_surface_p)
        {
            cerr << lt_dlerror() << endl;
            goto bigerror;
        }

        create_surface_p = (surface *(*)()) lt_dlsym(dlhandle, "gfx_create_surface");
        if (!create_surface_p)
        {
            cerr << lt_dlerror() << endl;
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

    // setup from configuration
    void setup (base::configuration & cfg)
    {
        // TODO: read options from configuration and use during initialization
    }

    // shutdown gfx
    void cleanup()
    {
        if (gfxcleanup) gfxcleanup();
        gfxcleanup = NULL;

        if (dlhandle) lt_dlclose(dlhandle);
        lt_dlexit();
    }

    surface * create_surface()
    {
        return create_surface_p();
    }

    //TODO  cache needs some work right now you there is no way to clear it
    //      perhaps it should be moved into a class
    const surface * surface_cache (const string & file, bool set_mask, bool invert_x)
    {
        static map<string, surface *> _cache;

        //Create a unique name based on all parameters
        string cachename = file + "_" + (set_mask?"true":"false") + "_" + (invert_x?"true":"false");

        map<string, surface *>::iterator idx = _cache.find(cachename);

        if (idx != _cache.end())
        {
            //We already found it in the cache
            return idx->second;
        }

        //Cache miss, try to load the file
        surface *cur = create_surface();
        cur->load_png(file);
        cur->set_mask(set_mask);
        cur->mirror(invert_x, false);

        //Add it to the cache
        _cache[cachename] = cur;
        return cur;
    }
}
