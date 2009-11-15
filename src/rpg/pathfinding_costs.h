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
 * @file   rpg/pathfinding_costs.h
 * @author Frederico Cerveira <frederico.cerveira@gmail.com>
 *
 * @brief  Pathfinding_Costs class and related.
 */

#ifndef PATHFINDING_COSTS_H
#define PATHFINDING_COSTS_H

#if __GNUG__ > 2
#include <ext/hash_map>
using namespace __gnu_cxx;
#else
#include <hash_map>
using namespace std;
#endif
#include "base/hash_map.h"
#include "rpg/specie.h"
#include "rpg/faction.h"

namespace rpg
{
    /// Compares two string for equality
    struct eqstr
    {
        bool operator()(std::string s1, std::string s2) const
        {
            return (s1 == s2);
        }
    };

   /**
    * The class with the terrain/cost hash_map.
    */
    class costs_hash
    {
    public:
       /**
        * Adds a terrain/cost relationship to the hash_map
        * @param terrain name of the terrain
        * @param cost cost associated to the terrain
        */
        void add_terrain_cost(const std::string & terrain, const s_int16 cost);

       /**
        * Get the cost associated with a certain terrain
        * @param terrain the name of the terrain to which the cost is associated
        * @return the cost associated
        */
        s_int16 get_cost(const std::string & terrain);

        void show_all_data()
        {
            if (Terrain_Cost.empty()) return;
            Terrain_Cost_Hash::const_iterator i = Terrain_Cost.begin();
            for (; i != Terrain_Cost.end(); i++)
            {
                printf("%s %d\n", (*i).first.c_str(),(*i).second) ;
            }
        }
    private:
        typedef hash_map<std::string, s_int16, hash<std::string>, eqstr> Terrain_Cost_Hash;
        /// The hash_map
        Terrain_Cost_Hash Terrain_Cost;
    };

   /**
    * The class that associates the type with a hash_map.
    */
    class type_costs
    {
    public:
        type_costs(const std::string & type, costs_hash * costs = NULL);

       /**
        * Sets the type
        * @param type the name of the type
        */
        void set_type(const std::string & type);

       /**
        * Get the type
        * @return the type
        */
        const std::string get_type() const;

       /**
        * Set the hash_map with the terrain/costs relationships.
        * @param costs pointer to the hash_map
        */
        void set_costs_hash(costs_hash * costs);

       /**
        * Get the hash_map with the terrain/costs relationships.
        * @return pointer to the hash_map
        */
        costs_hash * get_costs_hash() const;

    private:
        /// The type of this class
        std::string Type;
        /// Pointer to the hash that has the terrain/costs relationships.
        costs_hash * Costs_Hash;
    };

   /**
    * The class that holds the pathfinding_costs of a character.
    */
    class pathfinding_costs
    {
    public:
        pathfinding_costs();

       /**
        * Get the cost associated with that terrain, in the present pathfinding_type
        * @param name name of the terrain
        * @return the cost
        */
        s_int16 get_cost(const std::string & name) const;

       /**
        * Set the actual pathfinding_type
        * @param type the actual type
        */
        void set_pathfinding_type(const std::string & type);

       /**
        * Get the actual pathfinding_type
        * @return the actual pathfinding_type
        */
        const std::string get_pathfinding_type() const;

       /**
        * Updates the internal cost/terrain/type relationships. This can happen at the beginning or
        * when a faction is added/removed, specie altered, ...
        * @param Factions a vector with the factions
        * @param Specie the specie of the character
        */
        void update_costs(const std::vector<rpg::faction *> & factions, const rpg::specie * specie);

       /**
        * Load necessary data from a data file
        * @param file path to the file
        * @return \b true on success, \b false otherwise
        */
        bool parse_data_file(const std::string & file);

       /**
        * Load necessary data from a data file
        * @param file flat with the data
        * @return \b true on success, \b false otherwise
        */
        bool parse_data_file(const base::flat & file);

        void show_all_data()
        {
            printf("Pathfinding_Costs Type:%s\n", get_pathfinding_type().c_str());
            std::vector<type_costs *>::const_iterator i = Costs.begin();
            ++i;
            for (; i != Costs.end(); i++) {
                printf("In Vector: Type:%s\n", (*i)->get_type().c_str());
                (*i)->get_costs_hash()->show_all_data();

            }
        }
    private:
        /**
         * Adds a type with its respective terrain/cost associations.
         * @param type a pointer to the type
         * @param is_default if true this type is the default type, that is used when
         *        the character has no specific type set
         */
        void add_to_vector(type_costs * type, bool is_default = false);

       /**
        * Removes a type from the vector
        * @param type_name the name of the type to be removed
        */
        void remove_from_vector(const std::string & type_name);

        std::string Pathfinding_Type;
        std::vector<type_costs *> Costs;

    };
}


#endif // PATHFINDING_COSTS_H
