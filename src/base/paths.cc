/*
   $Id: paths.cc,v 1.4 2004/08/02 07:35:28 ksterker Exp $

   Copyright (C) 2003/2004 Alexandre Courbot <alexandrecourbot@linuxgames.com>
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

using std::cerr;
using std::endl;
using base::paths;

namespace base
{
    // return dynamic module
    lt_dlhandle get_module (const std::string & modname)
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
        
        cerr << "Failed to load module " << modname << ": " << lt_dlerror() << endl;
        
        return NULL;
    }   
}

// initialize data search paths
void paths::init (const std::string & game, const std::string & userdatadir)
{
    // no save game directory unless we actually load a game
    SaveDataDir = "";
    
    // user data dir might be optional
    UserDataDir = userdatadir;
    if (UserDataDir != "") 
    {
        if (UserDataDir[UserDataDir.size () - 1] != '/') UserDataDir += "/";
        UserDataDir += game;
    }
    
    // builtin data directory
    GameDataDir = DATA_DIR;
    GameDataDir += "/games/";
    GameDataDir += game;
}