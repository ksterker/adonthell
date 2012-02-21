/*
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

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>

#include "base/logging.h"
#include "base/paths.h"

#define MODULES_ENV "ADONTHELL_MODULES_PATH"

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

        LOG(FATAL) << logging::indent() << "Failed to load module " << mod_env << ": " << lt_dlerror();

        return NULL;
    }
}

// ctor
paths::paths ()
{
    /*** Josh Glover <jmglov@gmail.com> 2011/03/21
     *** Prevent init with blank game and userdir
    init ("", ".");
    ***/
}

// initialize data search paths
bool paths::init (const std::string & game, const std::string & userdatadir)
{
    Game = game;
    
    // make sure game name doesn't contain directory seperators
    std::string::iterator i = Game.begin();
    while (i != Game.end())
    {
        if (*i == '/' || *i == '\\') i = Game.erase (i);
        else i++;
    }

    // no save game directory unless we actually load a game
    IncludeSaveDir = false;
    IncludeUserDir = false;

    // set OS specific directory containing configuration and saved games
#if defined(__APPLE__)		// OSX
    CfgDataDir = string (getenv ("HOME")) + "/Library/Application Support/Adonthell/";
#elif defined (WIN32)		// Windows
    CfgDataDir = "./";
#else						// Unix
    char *xdgConfigDir = getenv ("XDG_CONFIG_HOME");
    if (xdgConfigDir != NULL && strlen (xdgConfigDir) > 0)
        CfgDataDir = string (xdgConfigDir) + "/adonthell";
    else
        CfgDataDir = string (getenv ("HOME")) + "/.config/adonthell/";
#endif

#ifndef WIN32
    // make sure configuration directory exists, otherwise create it
    if (!exists (CfgDataDir))
    {
        LOG(INFO) << "CfgDataDir '" << CfgDataDir << "' does not exist; creating";
        if (mkdir (CfgDataDir.c_str (), 0700) == -1)
        {
            int ecd = errno;
            LOG(FATAL) << "Creating CfgDataDir '" << CfgDataDir << "' failed: "
                       << strerror (ecd);
        }
    }
#endif

    // user data dir might be optional
    UserDataDir = userdatadir;
    if (UserDataDir != "")
    {
        if (UserDataDir[UserDataDir.size () - 1] != '/') UserDataDir += "/";
        if (Game != "") UserDataDir += Game + "/";

        // make sure the given user data dir is actually accessible
        if (exists (UserDataDir))
        {
            LOG(INFO) << "UserDataDir '" << UserDataDir << "' exists; using";
            IncludeUserDir = true;
        }
    }

    // builtin data directory
    GameDataDir = DATA_DIR;
    GameDataDir += "/games/";
    GameDataDir += Game + "/";
    LOG(INFO) << "GameDataDir: '" << GameDataDir << "'";

    // make sure game data dir exists
    return exists (GameDataDir) || IncludeUserDir || Game.empty();
}

// set path to saved game (before loading the game from that directory)
void paths::set_save_dir (const std::string & dir)
{
    if (dir != "" && exists (dir))
    {
        SaveDataDir = dir;
        if (SaveDataDir[SaveDataDir.size () - 1] != '/') SaveDataDir += "/";
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
    if (IncludeSaveDir && file.open (SaveDataDir + path)) 
    {
        VLOG(2) << "Found '" << path << "' in " << SaveDataDir;
        return true;
    }
    if (IncludeUserDir && file.open (UserDataDir + path))
    {
        VLOG(2) << "Found '" << path << "' in " << UserDataDir;
        return true;
    }
    if (file.open (GameDataDir + path))
    {
        VLOG(2) << "Found '" << path << "' in " << GameDataDir;
        return true;
    }
    
    // print search paths on failure
    LOG(ERROR) << "*** paths::open: file '" << path << "' does not exist in search path:";
    if (IncludeSaveDir) LOG(ERROR) << "  - " << SaveDataDir;
    if (IncludeUserDir) LOG(ERROR) << "  - " << UserDataDir;
    LOG(ERROR) << "  - " << GameDataDir;

    return false;
}

// find the given path in Adonthell's search paths
bool paths::find_in_path (std::string & path, const bool & log_error) const
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
        VLOG(2) << "Found '" << path << "' in " << SaveDataDir;
        path.insert (0, SaveDataDir);
        return true;
    }
    if (IncludeUserDir && stat ((UserDataDir + path).c_str (), &statbuf) != -1)
    {
        VLOG(2) << "Found '" << path << "' in " << UserDataDir;
        path.insert (0, UserDataDir);
        return true;
    }
    if (stat ((GameDataDir + path).c_str (), &statbuf) != -1)
    {
        VLOG(2) << "Found '" << path << "' in " << GameDataDir;
        path.insert (0, GameDataDir);
        return true;
    }
    
    // try whether file exists at given path
    if (stat (path.c_str (), &statbuf) != -1)
    {
        return true;
    }
    
    if (log_error)
    {
        // print search paths on failure
        LOG(ERROR) << "*** paths::find_in_path: file '" << path << "' does not exist in search path:";
        if (IncludeSaveDir) LOG(ERROR) << "  - " << SaveDataDir;
        if (IncludeUserDir) LOG(ERROR) << "  - " << UserDataDir;
        LOG(ERROR) << "  - " << GameDataDir;

        char *cwd = getcwd (NULL, 0);
        LOG(ERROR) << "  - " << cwd;
        free (cwd);
    }

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
    LOG(WARNING) << "*** paths::exists: directory '" << path << "' cannot be accessed!";
    return false;
}
