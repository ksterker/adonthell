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

#include <adonthell/python/script.h>
#include <adonthell/base/diskio.h>

namespace rpg
{
    /**
     * Holds all the information of a particular faction
     */
    class faction : public python::script
    {
    public:
        faction(const std::string & name);

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
         * Wrapper to the python call that estimates the terrain impact
         * @param terrain the type of terrain to estimate for
         * @return a percentage representing how much the speed is going to be affected
         */
        s_int32 estimate_speed(const std::string & terrain) const;

        /**
         * Returns the name of the faction
         * @return name of the faction
         */
        std::string name() const
        {
            return Name;
        }

        /**
         * @name Loading/Saving
         */
        //@{
        /**
         * Load data of all factions. This will first load the %faction
         * template to instanciate the underlying Python faction class. Then it will
         * restore the actual %faction data. If the faction is already instanciated,
         * it will be replaced.
         *
         * @return \b true if loading successful, \b false otherwise.
         */
        static bool load ();

        /**
         * Save all existing %factions to given directory. This will save
         * both the %faction template plus the actual data to a file called
         * 'faction.data'. The file will be replaced if it already exists.
         *
         * @param path name of the directory to save %faction data to.
         * @return \b true if saving successful, \b false otherwise.
         */
        static bool save (const string & path);

        /**
         * Deletes all objects in the internal Faction map
         */
        static void cleanup();

        /**
         * Load faction from named file. This will first load the %faction template
         * to instanciate the underlying Python faction class. Then it will
         * restore the actual %faction data. If an faction is already instanciated,
         * it will be replaced.
         *
         * @param file name of the file to load %faction from.
         * @return \b true if loading successful, \b false otherwise.
         */
        bool get_state (base::flat & file);

        /**
         * Save %faction to stream. This will save both the %faction template
         * plus the actual data to the given file. The file will be replaced
         * if it already exists.
         *
         * @param file stream to save %faction to.
         * @return \b true if saving successful, \b false otherwise.
         */
        bool put_state (base::flat & file) const;
        //@}

#ifndef SWIG
        GET_TYPE_NAME_VIRTUAL(rpg::faction);
#endif // SWIG

    private:
        /**
         * Create an empty faction.
         */
        faction() {}
        /**
         * Delete faction.
         */
        virtual ~faction() {}

        /**
         * Convenience method to instanciate a faction template. Its usage is
         * preferred over script::create_instance ().
         * @param templ name of the faction template without .py extension
         * @param args argument tuple to pass to the template's constructor
         * @return \b true if instanciated successful, \b false otherwise.
         */
        bool create_instance (const std::string & templ, PyObject * args = NULL);

        /// The name of this %faction
        std::string Name;

        /// List of all available factions
        static std::vector<faction *> Factions;
    };
}
#endif // FACTION_H
