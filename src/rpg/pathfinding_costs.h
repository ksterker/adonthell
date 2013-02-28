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

#include <vector>

#include <adonthell/base/flat.h>
#include <adonthell/base/hash_map.h>

namespace rpg
{
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

    private:
        typedef std::hash_map<std::string, s_int16> Terrain_Cost_Hash;
        /// The hash_map
        Terrain_Cost_Hash Terrain_Cost;
    };

   /**
    * The class that associates the pathfinding type with the terrain costs.
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

       /**
        * Check if this type forces the non-explicited terrains to be considered impassable.
        * @return \b true if its enabled, \b false otherwise
        */
        bool has_forced_impassable() const;

       /**
        * Sets the forced impassable on/off.
        * @param forced \b true for on, \b false for off
        */
        void set_forced_impassable(bool forced);

    private:
        /// The type of this class
        std::string Type;
        /// Pointer to the hash that has the terrain/costs relationships.
        costs_hash * Costs_Hash;
        /// force only the explicited terrains to be walkable, all others will be impassable
        bool Forced_Impassable;
    };

   /**
    * The class that holds the pathfinding_costs of a character.
    */
    class pathfinding_costs
    {
    public:
        pathfinding_costs();
        ~pathfinding_costs();

       /**
        * Get the cost associated with that terrain, in the present pathfinding_type
        * @param name name of the terrain
        * @return the cost
        */
        s_int16 get_cost(const std::string & name) const
        {
            return CurrentPathfindingType->get_costs_hash()->get_cost(name);
        }

       /**
        * Set the actual pathfinding_type
        * @param type the actual type
        */
        void set_pathfinding_type(const std::string & type);

       /**
        * Get the actual pathfinding_type
        * @return the actual pathfinding_type
        */
        const std::string get_pathfinding_type() const
        {
            return CurrentPathfindingType->get_type();
        }

       /**
        * Return if forced impassable is enabled
        */
        bool has_forced_impassable() const
        {
            return CurrentPathfindingType->has_forced_impassable();
        }

       /**
        * Updates the internal cost/terrain/type relationships for the given specie.
        * @param specie name of the pathfinding data file to load
        */
        void init(const std::string & specie);

        /**
         * Cleanup all pathfinding costs.
         */
        void cleanup();

    private:
       /**
        * Load necessary data from a data file
        * @param file path to the file
        * @return \b true on success, \b false otherwise
        */
        bool get_state(const std::string & file);

       /**
        * Load necessary data from a data file
        * @param file flat with the data
        * @return \b true on success, \b false otherwise
        */
        bool get_state(const base::flat & file);

        /// the active path finding type
        type_costs *CurrentPathfindingType;

        /// all available pathfinding types
        std::vector<type_costs *> Costs;
    };
}


#endif // PATHFINDING_COSTS_H
