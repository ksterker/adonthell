/*
   $Id: paths.h,v 1.3 2004/08/02 07:35:28 ksterker Exp $

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

#include "ltdl.h"

#include <string>

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
             * Before any method of this class can be used, it must be initialized.
             * @param game name of the game the data files queried belong to.
             * @param userdatadir optional, user supplied data directory.
             */
            void init (const std::string & game, const std::string & userdatadir = "");
            
        private:
            /// directory of the saved game currently being loaded (if any)
            std::string SaveDataDir;
            /// user supplied game data directory (if any) 
            std::string UserDataDir;
            // builtin game data directory (mandatory)
            std::string GameDataDir;
    };
}

#endif // BASE_PATHS
