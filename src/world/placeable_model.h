/*
 $Id: placeable_model.h,v 1.1 2007/05/19 07:42:10 ksterker Exp $
 
 Copyright (C) 2002 Alexandre Courbot <alexandrecourbot@linuxgames.com>
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
 * @file   world/placeable_model.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the placeable_model class.
 * 
 * 
 */


#ifndef WORLD_PLACEABLE_MODEL_H
#define WORLD_PLACEABLE_MODEL_H

#include <map>
#include <string>

#include "world/placeable_area.h"
#include "base/file.h"

namespace world
{
    /**
     * Represents a placeable, i.e. something (character, object, ...)
     * that can be placed on a map and occupies some space on it.
     *
     * This class is separated from placeable to allow easy editing
     * without depending on the whole map stuff. While it is suitable
     * for edition, it can't be actually placed on a map. Use placeable
     * for that instead.
     * 
     */
    class placeable_model
    {
    protected:
        mutable std::string Filename;

        mutable std::map <std::string, placeable_area> States;
        std::map <std::string, placeable_area>::iterator Current_state;

        bool State_changed;
    
    public:
        typedef std::map <std::string, placeable_area>::iterator iterator;

        iterator begin () 
        {
            return States.begin (); 
        }

        iterator end () 
        {
            return States.end (); 
        }

        placeable_model();

        placeable_area * current_state();

        placeable_area * get_state(const std::string & name);
    
        const std::string current_state_name();
    
        placeable_area * add_state (const std::string & name);

        bool del_state (const std::string & name);
    
        void set_state (const std::string & name);

        void put(base::ogzstream & file) const;

        void get(base::igzstream & file);

        const std::string & filename()
        {
            return Filename;
        }

        /**
         * This friendship is needed so placeable_model_gfx
         * can modify the Has_changed member.
         * 
         */
        friend class placeable_model_gfx;
    }; 
}

#endif
