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
 * @file   rpg/pathfinding_costs.cc
 * @author Frederico Cerveira <frederico.cerveira@gmail.com>
 *
 * @brief  Pathfinding_Costs class and related.
 */

#include "pathfinding_costs.h"

#include <adonthell/base/diskio.h>
#include <adonthell/base/logging.h>
#include <adonthell/base/base.h>

using namespace rpg;

/// directory containing the pathfinding data
#define PATHFINDING_DIRECTORY std::string("characters/pathfinding/")

/// a default pathfinding type that is always available
type_costs DefaultType("Default");

void costs_hash::add_terrain_cost(const std::string & terrain, const s_int16 cost)
{
    Terrain_Cost.insert(Terrain_Cost_Hash::value_type(terrain, cost));
}

s_int16 costs_hash::get_cost(const std::string & terrain)
{
    return Terrain_Cost[terrain];
}

type_costs::type_costs(const std::string & type, costs_hash * costs)
{
    set_type(type);
    set_costs_hash(costs);
}

void type_costs::set_type(const std::string & type)
{
    Type = type;
}

void type_costs::set_costs_hash(costs_hash * costs)
{
    Costs_Hash = costs;
}

costs_hash * type_costs::get_costs_hash() const
{
    return Costs_Hash;
}

const std::string type_costs::get_type() const
{
    return Type;
}

bool type_costs::has_forced_impassable() const
{
    return Forced_Impassable;
}

void type_costs::set_forced_impassable(bool forced)
{
    Forced_Impassable = forced;
}

void pathfinding_costs::init(const std::string & specie)
{
    if (!Costs.empty()) cleanup();

    // initialize the pathfinding_costs for the given specie
    std::string path = PATHFINDING_DIRECTORY + specie + ".data";
    if (base::Paths().find_in_path(path))
    {
        get_state (path);
    }
}

void pathfinding_costs::cleanup()
{
    for (std::vector<type_costs *>::iterator i = Costs.begin(); i != Costs.end(); i++)
    {
        delete *i;
    }
    Costs.clear();
    CurrentPathfindingType = &DefaultType;
}

bool pathfinding_costs::get_state(const base::flat & file)
{
    u_int32 size;
    void *value;
    char *id;
    base::flat record = file;

    while (record.next (&value, &size, &id) == base::flat::T_FLAT)
    {
        char * type = id;
        base::flat associations((const char*) value, size);

        // Get if forced impassable is enabled
        bool forced_impassable = associations.get_bool("forced_imp");
        base::flat terrain_costs = associations.get_flat("costs");

        costs_hash * tmp_hash = new costs_hash();

        while (terrain_costs.next (&value, &size, &id) == base::flat::T_SINT16)
        {
            tmp_hash->add_terrain_cost(id, *((s_int16*)value));
        }

        // Create type_cost and add it to vector
        type_costs * tmp = new type_costs(type);
        tmp->set_costs_hash(tmp_hash);
        tmp->set_forced_impassable(forced_impassable);

        Costs.push_back(tmp);
    }

    return true;
}

bool pathfinding_costs::get_state(const std::string & file)
{
    // try to load group
    base::diskio record (base::diskio::XML_FILE);

    if (record.get_record (file))
        return get_state (record);

    return false;
}

void pathfinding_costs::set_pathfinding_type(const std::string & type)
{
    std::vector<rpg::type_costs *>::const_iterator i = Costs.begin();

    for (; i != Costs.end(); i++)
    {
        if ((*i)->get_type() == type)
        {
            CurrentPathfindingType = *i;
            return;
        }
    }

    LOG(WARNING) << "set_pathfinding_type: Unknown pathfinding type '" << type << "'.";
}

pathfinding_costs::pathfinding_costs()
{
    // initially use a default path finding type, so pathfinding
    // will also work if no pathfinding data is present
    CurrentPathfindingType = &DefaultType;
}

pathfinding_costs::~pathfinding_costs()
{
    cleanup();
}
