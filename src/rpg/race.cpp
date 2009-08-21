#include "rpg/race.h"
#include <cctype>

using namespace rpg;

std::set<race *, order_by_name> race::Races;

race::race(const std::string & name)
{
    set_name(name);
}

s_int8 race::alignment() const
{
    return Alignment;
}

void race::set_name(const std::string & name)
{
    // Uppercase all the letters of the name
    std::transform(Name.begin(), Name.end(), Name.begin(), (int(*)(int)) std::toupper);
    Name = name;
}

bool race::put_state(base::flat & file) const
{
    base::flat race_flat;

    race_flat.put_uint8("Alignment", Alignment);
    Terrain.put_state(race_flat);

    file.put_flat(Name, race_flat);
}

bool race::get_state(base::flat & file, std::string & name)
{
    set_name(name);
    Alignment = file.get_uint8("Alignment");
    Terrain.get_state(file);
}

void race::load(const std::string & fname)
{
    // Load the races
    base::diskio record(base::diskio::BY_EXTENSION);
    record.get_record (fname);

    u_int32 size;
    u_int32 index = 0;
    void *value;
    char *id;

    while (record.next (&value, &size, &id) == base::flat::T_FLAT)
    {
        base::flat entity = base::flat ((const char*) value, size);
        std::string entity_file = id;

        // Creates a new race and loads its parameters
        race * tmp = new race;
        tmp->get_state(entity, entity_file);

        // Insert into set
        Races.insert(tmp);
    }

}

void race::cleanup()
{
    for (std::set<race *>::iterator i = Races.begin(); i != Races.end(); i++)
    {
        delete *i;
    }
}

race * race::get_race(const std::string & name)
{
    race tmp(name);

    std::set<race *>::const_iterator i = Races.find(&tmp);

    return (i != Races.end()) ? (*i) : NULL;
}
