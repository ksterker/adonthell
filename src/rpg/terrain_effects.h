/*
   Copyright (C) 2003/2004 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file   rpg/terrain_effects.h
 * @author Frederico Cerveira <frederico.cerveira@gmail.com>
 *
 * @brief  The terrain_effects class.
 */

#ifndef TERRAIN_EFFECTS_H
#define TERRAIN_EFFECTS_H

#include <string>
#include <set>
#include <algorithm>
#include "base/flat.h"
#include "base/types.h"

namespace rpg
{
    struct order_by_name
    {
        template <class T>
        bool operator()(T * i, T * a) const
        {
            return (i->name() < a->name());
        }
    };

    typedef u_int8 terrain_affinity;

    /**
     * Represents a relation between a name and a terrain_affinity.
     */
    class terrain_relation
    {
    public:

        terrain_relation(const std::string & name, const terrain_affinity affinity = 100)
        {
            set_name(name);
            set_affinity(affinity);
        }

        /**
         * Set the name of the terrain
         * @param name the name
         */
        void set_name(const std::string & name)
        {
            // Uppercase all the letters of the name
            std::transform(Name.begin(), Name.end(), Name.begin(), (int(*)(int)) std::toupper);
            Name = name;
        }

        /**
         * Set the affinity
         * @param affinity the affinity
         */
        void set_affinity(const terrain_affinity affinity)
        {
            Affinity = affinity;
        }

        /**
         * Return the name of the terrain
         * @return the name
         */
        std::string name() const
        {
            return Name;
        }

        /**
         * Return the affinity
         * @return the affinity
         */
        terrain_affinity affinity() const
        {
            return Affinity;
        }

        bool operator==(terrain_relation & i) const
        {
            return (name() == i.name());
        }

    private:
        std::string Name;
        terrain_affinity Affinity;
    };

    /**
     * Holds a ordered vector that makes the relation between a name and
     * the terrain affinity. Each race has one of these.
     */
    class terrain_effects
    {
    public:
        typedef std::set<terrain_relation *>::const_iterator iterator;

        /// Dtor
        ~terrain_effects();

        /**
         * Adds a relation between a name and a terrain_affiniy to the list
         * @param name the name of the terrain
         * @param affinity the affinity
         */
        void add_relation(const std::string & name, const terrain_affinity & affinity);

        /**
         * Deletes a relation based on its name
         * @param name the name of the relation to be deleted
         */
        void delete_relation(const std::string & name);

        /**
         * Get a relation base on its name
         * @param the name to be searched for
         * @return the affinity associated it that name, 0 if no association was found
         */
        terrain_affinity get_relation(const std::string & name) const
        {
            // Create fake terrain_relation in order to search for a "true" one
            // with that name
            terrain_relation tmp(name);

            iterator i = Terrains.find(&tmp);

            return (i != Terrains.end()) ? (*i)->affinity() : 0;
        }

        /// Constant iterators
        iterator begin() const;
        iterator end() const;

        bool put_state(base::flat & file) const;
        bool get_state(base::flat & file);
    private:
        /// Ordered vector that associates a name with a terrain_affinity
        std::set<terrain_relation *, order_by_name> Terrains;
    };
}

#endif // TERRAIN_EFFECTS_H
