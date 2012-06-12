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

#include "rpg/specie.h"

/// filename of time data file
#define SPECIE_DATA "specie.data"

using namespace rpg;

std::vector<specie *> specie::Species;

s_int8 specie::alignment() const
{
    return Alignment;
}

void specie::set_alignment(s_int8 alignment)
{
    Alignment = alignment;
}

specie * specie::get_specie(const std::string & name)
{
    std::vector<specie *>::iterator i;

    for (i = Species.begin(); i != Species.end(); i++)
    {
        if ((*i)->name() == name)
            return (*i);
    }
    return NULL;
}

bool specie::put_state (base::flat & file)
{
    if (!group::put_state(file))
        return false;

    file.put_sint8("Alignment", Alignment);
    return true;
}

bool specie::get_state (base::flat & file)
{
    if (!group::get_state(file))
        return false;

    file.get_sint8("Alignment", Alignment);

    return file.success();
}

bool specie::get_state (const std::string & filename)
{
    base::diskio file (base::diskio::XML_FILE);

    if (file.get_record (filename))
    {
        u_int32 size;
        char *data;
        char *id;

        if (file.next ((void**) &data, &size, &id) == base::flat::T_FLAT)
        {
            base::flat record (data, size); 
            return get_state(record);
        }
    }

    return false;

}

void specie::cleanup()
{
    std::vector<specie *>::iterator i;

    for (i = Species.begin(); i != Species.end(); i++)
    {
        delete (*i);
    }

    Species.clear();
}

bool specie::load ()
{
    base::diskio file;

    // try to read file from disk
    if (!file.get_record (SPECIE_DATA))
        return false;

    u_int32 size;
    char *data;
    char *id;

    specie *s;
    bool result = true;

    // iterate over all saved characters
    while (file.next ((void**) &data, &size, &id) == base::flat::T_FLAT)
    {
        s = new specie (id);
        base::flat record (data, size); 
        if (s->get_state(record))
        {
            // loading successful
            Species.push_back(s);
            continue;
        }

        fprintf (stderr, "*** specie::load: loading specie '%s' failed.\n", id);
        result = false;
        delete s;
    }

    return result;
}

bool specie::save (const std::string & path)
{
    base::diskio file;

    std::vector<specie *>::iterator i;

    for (i = Species.begin(); i != Species.end(); i++)
    {
        base::flat record;
        if (!(*i)->put_state (record))
        {
            fprintf (stderr, "*** specie::save: saving specie '%s' failed!\n",
                (*i)->name().c_str());
            return false;
        }
        file.put_flat ((*i)->name(), record);
    }

    return file.put_record (path + "/" + SPECIE_DATA);
}

void specie::add (specie *s)
{
    Species.push_back(s);
}

