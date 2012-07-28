/*
   $Id: paths.h,v 1.11 2008/05/25 17:54:47 ksterker Exp $

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
 * @file   base/paths.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares some primitives to get essential game paths.
 * 
 * 
 */

#ifndef BASE_PATHS_H
#define BASE_PATHS_H

#ifndef LT_SCOPE
#define LT_SCOPE extern
#endif

#include "ltdl.h"

#include "file.h"

namespace base
{
    /**
     * Return handle to dynamic module specified by modname. First, the location
     * specified by the environment variable ADONTHELL_MODULES_PATH is searched
     * for the module. If that does not exist or does not contain the module, the
     * hard coded module path is searched: $(libdir)/adonthell.
     * @param modname name of the dynamic module to load.
     * @return handle to the module or \b NULL if loading failed.
     */
    lt_dlhandle get_module (const std::string & modname);
    
    /**
     * This class implements a way to open game data files without knowing the
     * exact location. For that it searches a number of directories, if specified,
     * in the following order:
     * - a saved game directory
     * - a user supplied data directory
     * - the builtin data directory
     */
    class paths
    {
        public:
            /**
             * Default constructor.
             */
            paths ();
        
            /**
             * Before any method of this class can be used, it must be initialized.
             * @param game name of the game the data files queried belong to.
             * @param userdatadir optional, user supplied data directory.
             * @return \c true if successful, \c false otherwise
             */
            bool init (const std::string & game, const std::string & userdatadir = "");
            
            /**
             * While loading a saved game, the name of the saved game directory must
             * be passed, so that it can be temporarily included into the Adonthell
             * search path. To remove it after loading, pass an empty string ("")
             * to this method.
             * @param dir the saved game directory.
             */
            void set_save_dir (const std::string & dir);

            /**
             * Try to open the given file at the given location. Searches for the
             * file in saved game dir (if set before with set_save_dir()), the
             * user supplied data directory and finally the built in data directory.
             * @param file a igzstream to be opened.
             * @param location file to locate within the Adonthell search path.
             * @return \c true on success, \c false if the file doesn't exist.
             */
            bool open (igzstream & file, const std::string & location) const;
            
            /**
             * Try to find a file at the given location. Searches for the
             * file in saved game dir (if set before with set_save_dir()), the
             * user supplied data directory and finally the built in data directory.
             * If found, the path to the location will be prepended to location.
             * @param location file to locate within the Adonthell search path.
             * @param log_error whether to log error if file not found. True by default.
             * @return \c true on success, \c false if the file doesn't exist.
             */
            bool find_in_path (std::string & location, const bool & log_error = true) const;
                
            /**
             * Return the configuration data directory.
             * @return path to the Adonthell configuration directory.
             */
            std::string cfg_data_dir () const { return CfgDataDir; }

            /**
             * Return the built in game data directory.
             * @return path to the data directory of current game.
             */
            std::string game_data_dir () const { return GameDataDir; }

            /**
             * Return the optional user data directory.
             * @return path to the user data directory, if present.
             */
            std::string user_data_dir () const { return UserDataDir; }
            
            /**
             * Return name of the game currently running.
             * @return name of current game.
             */
            std::string game () const { return Game; }
            
        private:
            /**
             * Check whether the given directory exists and is accessible.
             * @param path an absolute path to check.
             * @return \c true if directory can be opened, \c false otherwise.
             */
            bool exists (const std::string & path) const;
            
            /// current game
            std::string Game;
            /// directory of the saved game currently being loaded (if any)
            std::string SaveDataDir;
            /// user supplied game data directory (if any) 
            std::string UserDataDir;
            // builtin game data directory (mandatory)
            std::string GameDataDir;
            /// the (OS dependent) path to configuration and saved games
            std::string CfgDataDir;
            /// whether to include a saved game directory in Adonthell's search path
            bool IncludeSaveDir;
            /// whether to include user supplied data directory in search path
            bool IncludeUserDir;
    };
}

#endif // BASE_PATHS
