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

#include <vector>
#include "group.h"

namespace rpg
{

    /**
     * Holds all the information of a particular specie.
     */
    class specie : public group
    {
    public:

        specie(const std::string & name) : group(name), Alignment(0)
        {
            // Update the path to the python modules
            set_group_package("groups.species.");
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
         * Save specie to file
         * @param file object to write to
         * @return true on success
         */
        bool put_state (base::flat & file);

        /**
         * Load a specie from file.  Assumes that any surrounding
         * FLAT record has already been loaded. (see load() and
         * get_state(filename))
         * @param file object to read from
         * @return true on success
         */
        bool get_state (base::flat & file);

        /**
         * Convenience wrapper, to load a specie from an XML file.
         * @param XML path and filename
         * @return true on success
         */
        bool get_state (const std::string & filename);

#ifndef SWIG
        GET_TYPE_NAME_VIRTUAL(rpg::specie);
#endif // SWIG

        /**
         * Cleans everything up, and responsible for deleting all
         * internal objects in the vector, which are only added
         * by the load() call.
         */
        static void cleanup();

        /**
         * Load the specie. Load is responsible for adding all new
         * objects to the static Species vector.  Not the constructor.
         * Species is freed by cleanup().
         * @return true on success, false otherwise.
         */
        static bool load ();

        /**
         * Save the specie
         * @param path directory to save data to.
         * @return true on success, false otherwise.
         */
        static bool save (const std::string & path);

        /**
         * Adds a specie to Species and takes ownership of it
         */
        static void add (specie *s);

    private:

        /// The good/evil alignment of the specie
        s_int8 Alignment;

        /// List of all available species
        static std::vector<specie *> Species;
    };
}
#endif
