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

void specie::cleanup()
{
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
