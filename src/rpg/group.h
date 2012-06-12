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
 * @file   rpg/group.h
 * @author Frederico Cerveira <frederico.cerveira@gmail.com>
 *
 * @brief  Group class.
 */

#ifndef GROUP_H
#define GROUP_H

#include "python/script.h"
#include "base/diskio.h"

namespace rpg
{

    /**
     * This class is the base for every other class that represents a group of
     * people. Namely the species and faction classes.
     */
    class group : public python::script
    {
    public:
        group(const std::string & name);

        /**
         * Convenience method to instanciate a group template. Its usage is
         * preferred over script::create_instance ().
         * @param templ name of the group template without .py extension
         * @param args argument tuple to pass to the template's constructor
         * @return \b true if instanciated successful, \b false otherwise.
         */
        bool create_instance (const std::string & templ, PyObject * args = NULL);

        /**
         * Wrapper to the python call that estimates the terrain impact
         * @param terrain the type of terrain to estimate for
         * @return a percentage representing how much the speed is going to be affected
         */
        s_int32 estimate_speed(const std::string & terrain) const;


        /**
         * Wrapper to the python call that estimates the terrain impact when
         * doing a pathfinding search
         * @param terrain the type of terrain to estimate for
         * @return a percentage representing how much the speed is going to be affected
         */
        s_int32 estimate_speed_for_pathfinding(const std::string & terrain) const;

        /**
         * Returns the name of the group
         * @return name of the group
         */
        std::string name() const
        {
            return Name;
        }

        /**
         * Returns the path to the location that holds the pathfinding_costs data of this group
         * @return the path of the pathfinding_costs file
         */
        std::string pathfinding_costs_path() const
        {
            return Pathfinding_Costs_Path;
        }

        /**
         * Sets the name of the group
         * @param name new name
         */
        void set_name(const std::string & name)
        {
            Name = name;
        }

        /**
         * Sets the path to the various python modules this group can
         * access. It can be used as a way to prevent unwanted use of templates
         * that do not fully implement the class. Ex: a faction template should
         * not be used by a specie class.
         * @param group_package the path to the various python modules
         */
        void set_group_package(const std::string & group_package)
        {
            Group_Package = group_package;
        }

        /**
         * @name Loading/Saving
         */
        //@{
        /**
         * Load group from named file. This will first load the %group template
         * to instanciate the underlying Python group class. Then it will
         * restore the actual %group data. If an group is already instanciated,
         * it will be replaced.
         *
         * @param file name of the file to load %group from.
         * @return \b true if loading successful, \b false otherwise.
         */
        bool get_state (base::flat & file);

        /**
         * Load %group from stream. This will first load the %group template
         * to instanciate the underlying Python group class. Then it will
         * restore the actual %group data. If an group is already instanciated,
         * it will be replaced.
         *
         * @param file stream to load group from.
         * @return \b true if loading successful, \b false otherwise.
         */
        bool get_state (const std::string & file);

         /**
         * Save %group to named file. This will save both the group template
         * plus the actual data to the given file. The file will be replaced
         * if it already exists.
         *
         * @param file name of the file to save %group to.
         * @param format whether to save as XML or compressed binary.
         * @return \b true if saving successful, \b false otherwise.
         */
        bool put_state (const string & file, const base::diskio::file_format & format = base::diskio::XML_FILE) const;

        /**
         * Save %group to stream. This will save both the %group template
         * plus the actual data to the given file. The file will be replaced
         * if it already exists.
         *
         * @param file stream to save %group to.
         * @return \b true if saving successful, \b false otherwise.
         */
        bool put_state (base::flat & file) const;

#ifndef SWIG
        GET_TYPE_NAME_VIRTUAL(rpg::group);
#endif // SWIG

    private:

        /// The name of this group
        std::string Name;

        /// The Python package containing %group templates.
        std::string Group_Package;

        /// Path to the xml file holding the %pathfinding_costs data for this %group.
        std::string Pathfinding_Costs_Path;

    };
}
#endif // GROUP_H
