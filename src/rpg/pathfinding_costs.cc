#include "rpg/pathfinding_costs.h"
#include "base/flat.h"

using namespace rpg;


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

void pathfinding_costs::add_to_vector(type_costs * type, bool is_default)
{
    if (is_default)
    {
        Costs.insert(Costs.begin(), type);
    } else {
       Costs.push_back(type);
    }
}

void pathfinding_costs::remove_from_vector(const std::string & type_name)
{
    std::vector<type_costs *>::iterator i = Costs.begin();
    ++i;
    for (; i != Costs.end(); i++)
    {
        if ((*i)->get_type() == type_name)
        {
            Costs.erase(i);
            return;
        }
    }
}

s_int16 pathfinding_costs::get_cost(const std::string & name) const
{
    std::vector<rpg::type_costs *>::const_iterator i = Costs.begin();

    for (; i != Costs.end(); i++)
    {
        if ((*i)->get_type() == Pathfinding_Type)
            break;
    }

    return (*i)->get_costs_hash()->get_cost(name);
}

bool pathfinding_costs::has_forced_impassable() const
{
    std::vector<rpg::type_costs *>::const_iterator i = Costs.begin();

    for (; i != Costs.end(); i++)
    {
        if ((*i)->get_type() == Pathfinding_Type)
            return (*i)->has_forced_impassable();
    }

    fprintf(stderr, "*** Actual Pathfinding_Costs type (%s) not found in vector.", Pathfinding_Type.c_str());
    return false;
}

void pathfinding_costs::update_costs(const std::vector<rpg::faction *> & factions, const rpg::specie * specie)
{
    // Update the pathfinding_costs for the specie
    if (specie != NULL) {
        parse_data_file(specie->pathfinding_costs_path());

    }

    // Update the pathfinding_costs for the factions
    std::vector<rpg::faction *>::const_iterator i = factions.begin();
    for (; i != factions.end(); i++)
    {
        if (*i != NULL)
            parse_data_file((*i)->pathfinding_costs_path());
    }

}


bool pathfinding_costs::parse_data_file(const base::flat & file)
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

        costs_hash * tmp_hash = new costs_hash();

        while (associations.next (&value, &size, &id) == base::flat::T_FLAT)
        {
            char * terrain = id;
            base::flat cost_flat((const char*) value, size);

            // Add cost to hash_map
            s_int16 cost = cost_flat.get_sint16("Cost");
            tmp_hash->add_terrain_cost(terrain, cost);
        }

        // Create type_cost and add it to vector
        type_costs * tmp = new type_costs(type);
        tmp->set_costs_hash(tmp_hash);
        tmp->set_forced_impassable(forced_impassable);
        add_to_vector(tmp);
    }

    return true;
}

bool pathfinding_costs::parse_data_file(const std::string & file)
{
    // try to load group
    base::diskio record (base::diskio::XML_FILE);

    if (record.get_record (file))
        return parse_data_file (record);

    return false;
}

void pathfinding_costs::set_pathfinding_type(const std::string & type)
{
    Pathfinding_Type = type;
}

const std::string pathfinding_costs::get_pathfinding_type() const
{
    return Pathfinding_Type;
}

pathfinding_costs::pathfinding_costs()
{
    set_pathfinding_type("Default");

    // Create the default type. It can't be eliminated
    type_costs * default_type = new type_costs("Default");

    add_to_vector(default_type, true);
}

