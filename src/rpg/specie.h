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
 * @file   rpg/specie.h
 * @author Frederico Cerveira <frederico.cerveira@gmail.com>
 *
 * @brief  Specie class.
 */

#ifndef SPECIE_H
#define SPECIE_H

#include <set>
#include "rpg/group.h"

namespace rpg
{
   
    /**
     * Holds all the information of a particular specie.
     */
    class specie : public group
    {
    public:
        
        specie(const std::string & name) : group(name) 
        {
            // Update the path to the python modules
            set_group_package("groups.species.");
            
            Species.insert(this);
        }
        
        /**
         * Returns the alignment of the specie
         * @return alignment of the specie
         */
        s_int8 alignment() const;
        
        /**
         * Sets the alignment of the specie
         * @param alignment the new alignment
         */
        void set_alignment(s_int8 alignment);

        /**
         * Returns a specie with that name
         * @param name the name of the specie
         * @return a pointer to the specie, NULL if none exists
         */
        static specie * get_specie(const std::string & name);

        /**
         * Cleans everything up
         */
        static void cleanup();

    private:

        /// The good/evil alignment of the specie
        s_int8 Alignment;

        /// List of all available species
        static std::set<specie *, order_by_name> Species;
    };
}
#endif
