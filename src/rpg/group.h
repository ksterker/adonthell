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

namespace rpg
{
    struct order_by_name
    {
        template <class T>
        bool operator()(T * i, T * a) const
        {
            return (i->name() < a->name());
        }
    };
    
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
    private:

        /// The name of this group
        std::string Name;
        
        /// The Python package containing %group templates.
        std::string Group_Package;

    };
}
#endif // GROUP_H
