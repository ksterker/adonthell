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

#include "rpg/faction.h"

using namespace rpg;

std::set<faction *, order_by_name> faction::Factions;

bool faction::verify_requirements() const
{
    PyObject * ret = call_method_ret("requirements");
        
    s_int32 ret_int = python::retrieve_instance<s_int32, s_int32>(&*ret);
    
    if (ret_int == 1)
    {
        // Success. Requirements fulfilled
        return true;
            
    } else if (ret_int != 0) {
        // Invalid return code
        printf ("*** faction::verify_requirements. %s Faction. Invalid return code for the \"requirements\" python func.\n",
                name().c_str());
    }
    
    // Failed
    return false;
}

void faction::cleanup()
{
}

faction * faction::get_faction(const std::string & name)
{
    faction tmp(name);

    std::set<faction *>::iterator i = Factions.find(&tmp);

    return (i != Factions.end()) ? (*i) : NULL;
}
