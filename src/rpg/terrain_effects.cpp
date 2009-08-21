#include "rpg/terrain_effects.h"
#include <cctype>

using namespace rpg;


terrain_effects::~terrain_effects()
{
    for (iterator i = Terrains.begin(); i != Terrains.end(); i++)
    {
        delete *i;
    }
}

void terrain_effects::add_relation(const std::string & name, const terrain_affinity & affinity)
{
    terrain_relation * tmp = new terrain_relation(name, affinity);
    Terrains.insert(tmp);
}

void terrain_effects::delete_relation(const std::string & name)
{
    terrain_relation tmp(name);

    iterator i = Terrains.find(&tmp);

    delete *i;
    Terrains.erase(i);
}


terrain_effects::iterator terrain_effects::begin() const
{
    return Terrains.begin();
}

terrain_effects::iterator terrain_effects::end() const
{
    return Terrains.end();
}

bool terrain_effects::put_state(base::flat & file) const
{
    base::flat tmp;

    for (iterator i = begin(); i != end(); i++)
    {
        tmp.put_uint8("Affinity", (*i)->affinity());
        file.put_flat((*i)->name(), tmp);

        tmp.clear();
    }

    return true;
}

bool terrain_effects::get_state(base::flat & file)
{
    u_int32 size;
    u_int32 index = 0;
    void *value;
    char *id;

    while (file.next (&value, &size, &id) == base::flat::T_FLAT)
    {
        base::flat entity = base::flat ((const char*) value, size);
        const std::string entity_file = id;

        add_relation(entity_file, entity.get_uint8("Affinity"));
    }
}
