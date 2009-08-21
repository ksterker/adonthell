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
 * @file   rpg/race.h
 * @author Frederico Cerveira <frederico.cerveira@gmail.com>
 *
 * @brief  Race class.
 */

#ifndef RACE_H
#define RACE_H

#include "rpg/terrain_effects.h"
#include "base/diskio.h"
#include "base/flat.h"

namespace rpg
{
    /**
     * Holds all the information of a particular race.
     */
    class race
    {
    public:

        /**
         * Returns the name of the race
         * @return name of the race
         */
        std::string name() const
        {
            return Name;
        }

        /**
         * Returns the alignment of the race
         * @return alignment of the race
         */
        s_int8 alignment() const;

        /**
         * Returns pointer to terrain_effects class
         * @return pointer to Terrain
         */
        const terrain_effects * terrain() const
        {
           return &Terrain;
        }

        /**
         * Save %race to stream
         * @param file stream to where %race is going to be saved
         * @return /b true on success, /b false otherwise
         */
        bool put_state (base::flat & file) const;

        /**
         * Load %race from stream
         * @param file stream from where %race is going to be loaded
         * @param name name of this race
         */
        bool get_state (base::flat & file, std::string & name);

        /**
         * Sets the name of the race
         * @param name new name of the race
         */
        void set_name(const std::string & name);

        /**
         * Returns a race with that name
         * @param name the name of the race
         * @return a pointer to the race, NULL if none exists
         */
        static race * get_race(const std::string & name);

        /**
         * Loads everything necessary
         * @param fname the race config file path
         */
        static void load(const std::string & fname);

        /**
         * Cleans everything up
         */
        static void cleanup();

    private:

        /// The name of the race
        std::string Name;

        /// The good/evil alignment of the race
        s_int8 Alignment;

        /// Holds the effects that a certain terrain has on this race
        terrain_effects Terrain;

        /// List of all available races
        static std::set<race *, order_by_name> Races;

        /// Ctor
        race(const std::string & name);
        race() {};
    };
}
#endif
