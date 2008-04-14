/*
   $Id: paths.cc,v 1.13 2008/04/14 11:03:09 ksterker Exp $

   Copyright (C) 2003/2004 Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Copyright (C) 2005 Kai Sterker <kaisterker@linuxgames.com>
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
 * @author Kai Sterker <kaisterker@linuxgames.com>
 *
 * @brief  Defines some primitives to get essential game paths.
 *
 *
 */

#include <cstdlib>
#include <iostream>
#include <dirent.h>
#include <sys/stat.h>

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

        cerr << "Failed to load module " << mod_env << ": " << lt_dlerror() << endl;

        return NULL;
    }
}

// ctor
paths::paths ()
{
    init ("", ".");
}

// initialize data search paths
bool paths::init (const std::string & game, const std::string & userdatadir)
{
    Game = game;
    
    // no save game directory unless we actually load a game
    IncludeSaveDir = false;
    IncludeUserDir = false;

    // set OS specific directory containing configuration and saved games
#if defined(__APPLE__)		// OSX
    CfgDataDir = string (getenv ("HOME")) + "/Library/Application Support/Adonthell/";
#elif defined (WIN32)		// Windows
    CfgDataDir = "./";
#else						// Unix
    CfgDataDir = string (getenv ("HOME")) + "/.adonthell/";
#endif

#ifndef WIN32
    // make sure configuration directory exists, otherwise create it
    if (!exists (CfgDataDir)) mkdir (CfgDataDir.c_str (), 0700);
#endif

    // user data dir might be optional
    UserDataDir = userdatadir;
    if (UserDataDir != "")
    {
        if (UserDataDir[UserDataDir.size () - 1] != '/') UserDataDir += "/";
        if (game != "") UserDataDir += game + "/";

        // make sure the given user data dir is actually accessible
        if (exists (UserDataDir)) IncludeUserDir = true;
    }

    // builtin data directory
    GameDataDir = DATA_DIR;
    GameDataDir += "/games/";
    GameDataDir += game + "/";

    // make sure game data dir exists
    return exists (GameDataDir);
}

// set path to saved game (before loading the game from that directory)
void paths::set_save_dir (const std::string & dir)
{
    if (dir != "" && exists (CfgDataDir + dir))
    {
        SaveDataDir = CfgDataDir + dir;
        IncludeSaveDir = true;
    }
    else
    {
        IncludeSaveDir = false;
        SaveDataDir = "";
    }
}

// open the specified file
bool paths::open (igzstream & file, const std::string & path) const
{
    // check whether path is absolute in which case we just open the file
    if (path.length() > 1 && (path[0] == '~' || path[0] == '/' || path[1] == ':'))
    {
        if (!file.is_open ()) file.open (path);
        return file.is_open ();
    }

    // close file if it is already open
    if (file.is_open ()) file.close ();
    
    // otherwise try to prepend any of the build-in search paths
    if (IncludeSaveDir && file.open (SaveDataDir + path)) return true;
    if (IncludeUserDir && file.open (UserDataDir + path)) return true;
    if (file.open (GameDataDir + path)) return true;

    // print search paths on failure
    fprintf (stderr, "*** paths::open: file '%s' does not exist in search path:\n", path.c_str ());
    if (IncludeSaveDir) fprintf (stderr, "  - %s\n", SaveDataDir.c_str ());
    if (IncludeUserDir) fprintf (stderr, "  - %s\n", UserDataDir.c_str ());
    fprintf (stderr, "  - %s\n", GameDataDir.c_str ());

    return false;
}

// find the given path in Adonthell's search paths
bool paths::find_in_path (std::string & path) const
{
    struct stat statbuf;
    
    // check whether path is absolute in which case we return it as is
    if (path.length() > 1 && (path[0] == '~' || path[0] == '/' || path[1] == ':'))
    {
        return true;
    }

    // try whether path exists in the search path
    if (IncludeSaveDir && stat ((SaveDataDir + path).c_str (), &statbuf) != -1)
    {
        path.insert (0, SaveDataDir + "/");
        return true;
    }
    if (IncludeUserDir && stat ((UserDataDir + path).c_str (), &statbuf) != -1)
    {
        path.insert (0, UserDataDir + "/");
        return true;
    }
    if (stat ((GameDataDir + path).c_str (), &statbuf) != -1)
    {
        path.insert (0, GameDataDir + "/");
        return true;
    }
    
    // try whether file exists at given path
    if (stat (path.c_str (), &statbuf) != -1)
    {
        return true;
    }
    
    // print search paths on failure
    fprintf (stderr, "*** paths::find_in_path: file '%s' does not exist in search path:\n", path.c_str ());
    if (IncludeSaveDir) fprintf (stderr, "  - %s\n", SaveDataDir.c_str ());
    if (IncludeUserDir) fprintf (stderr, "  - %s\n", UserDataDir.c_str ());
    fprintf (stderr, "  - %s\n", GameDataDir.c_str ());

    char *cwd = getcwd (NULL, 0); 
    fprintf (stderr, "  - %s\n", cwd);
    free (cwd);
              
    return false;
}

// check whether given path exists at all
bool paths::exists (const std::string & path) const
{
    DIR *dir = opendir (path.c_str ());
    if (dir != NULL)
    {
        closedir (dir);
        return true;
    }

    // dir doesn't exist or not enough privileges ...
    fprintf (stderr, "*** warning: directory '%s' cannot be accessed!\n", path.c_str ());
    return false;
}
