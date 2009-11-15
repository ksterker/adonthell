/*
   Copyright (C) 2009 Frederico Cerveira <frederico.cerveira@gmail.com>
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
 * @file   rpg/faction.h
 * @author Frederico Cerveira <frederico.cerveira@gmail.com>
 *
 * @brief  Faction class.
 */

#ifndef FACTION_H
#define FACTION_H

#include <vector>
#include "rpg/group.h"

namespace rpg
{

    /**
     * Holds all the information of a particular faction
     */
    class faction : public group
    {
    public:

        faction(const std::string & name) : group(name)
        {
            // Update the path to the python modules
            set_group_package("groups.factions.");
            Factions.push_back(this);
        }

        /**
         * Wrapper call to python function that verifies if the faction
         * requirements are fulfilled
         * @return \b true on success, \b false otherwise
         */
        bool verify_requirements() const;

        /**
         * Returns a faction with that name
         * @param name the name of the faction
         * @return a pointer to the faction, NULL if none exists
         */
        static faction * get_faction(const std::string & name);

        /**
         * Cleans everything up
         */
        static void cleanup();

    private:

        /// List of all available factions
        static std::vector<faction *> Factions;
    };
}
#endif
