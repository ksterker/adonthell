/*
 Copyright (C) 2002 Alexandre Courbot <alexandrecourbot@linuxgames.com>
 Copyright (C) 2007/2008 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file   world/area.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * @author Kai Sterker <kaisterker@linuxgames.com>
 *
 * @brief  Declares the area class.
 *
 */


#ifndef WORLD_AREA_H
#define WORLD_AREA_H

#include <adonthell/base/hash_map.h>
#include <adonthell/base/diskio.h>

#include "chunk.h"
#include "zone.h"

/**
 * The graphical representation of the game %world is implemented by this module.
 */
namespace world
{
    /**
     * The plane of existance. It keeps track of all the scenery elements, characters
     * and items that are part of a map. Their actual locations are kept in the
     * underlying chunk, an octree-like structure.
     */
    class area : public chunk
    {
    public:
        /**
         * Create an empty map.
         */
        area () : chunk () { }

        /**
         * Delete the map and everything on it.
         */
        ~area ();

        /**
         * Remove all objects, characters and zones from the map.
         */
        void clear();

        /**
         * Update state of all characters and objects on the map.
         * @todo decide if we need that, or if everything can be
         *     updated based on events.
         */
        void update();

        /**
         * @name Map Object Handling.
         */
        //@{
        /**
         * Add given object to the map. The index returned
         * can then be used to actually place instances of 
         * the object onto the map.
         * 
         * @param object the object to add to the map.
         * @return index in list of objects or -1 on error.
         */
        s_int32 add_entity (entity * object);
            
        /**
         * Place object at a given index at the given location.
         * @param index index of object in the list of objects.
         * @param pos position to place object at.
         * @return newly placed chunk or NULL on failure.
         */
        chunk_info * place_entity (const s_int32& index, coordinates & pos);

        /**
         * Get entity at given index
         * @param index index of object in the list of objects.
         * @return the matching entity, or NULL if index out of range.
         */
        placeable * get_entity (const s_int32 & index) const;

        /**
         * Get named entity.
         * @param id unique identifier for the entity to retrieve.
         * @return the matching entity, or NULL if no such entity exists.
         */
        placeable * get_entity (const std::string & id) const;

        /**
         * Get the name of the entity that possess the placeable
         * @param object the placeable
         * @return the name
         */
        const std::string * get_entity_name (const placeable * object) const;
        //@}

        /**
         * @name Zone Markers
         */
        //@{
        /**
         * Add a zone to this map
         * @param zone a pointer to the zone
         * @return \b true on success, \b false otherwise
         */
        bool add_zone(world::zone * zone);

        /**
         * Remove a zone from this map
         * @param zone a pointer to the zone
         */
        void remove_zone(world::zone *zone);
        
        /**
         * Returns a zone given its name
         * @param name the name of the zone
         * @return a pointer to the zone, or NULL on error
         * @note name is case-sensitive
         */
        world::zone *get_zone(const std::string & name) const;
        
        /**
         * Return all zones that contain the given point of given type.
         * @param point a location on the map.
         * @param type a zone type.
         * @return all matching zones.
         */
        std::vector<world::zone*> find_zones (const world::vector3<s_int32> & point, const u_int32 & type);
        //@}

        /**
         * Loading / Saving
         */
        //@{
        /**
         * Save %area state to stream.
         * @param file stream to save %area to.
         * @return \b true if saving successful, \b false otherwise.
         */
        bool put_state (base::flat & file) const;

        /**
         * Load %area state from stream.
         * @param file stream to load %area from.
         * @return \b true if loading successful, \b false otherwise.
         */
        bool get_state (base::flat & file);

        /**
         * Save %area state to file.
         * @param fname file name.
         * @param format type of file to create.
         * @return true on success, false otherwise.
         */
        bool save (const std::string & fname, const base::diskio::file_format & format = base::diskio::BY_EXTENSION);

        /**
         * Load %area state from file.
         * @param fname file name.
         * @return true on success, false otherwise.
         */
        bool load (const std::string & fname);

        /**
         * Get the filename of this map.
         * @return the file this map was loaded from.
         */
        std::string filename () const
        {
            return Filename;
        }
        //@}

        /// This class is allowed to change and move objects on the map.
        friend class moving;

#ifndef SWIG
        /**
         * Allow %area to be passed as python argument
         */
        GET_TYPE_NAME (world::area)

    protected:
        /// The individual objects on the map
        std::vector <world::entity *> Entities;

        /// Named objects on the map
        std::hash_map <std::string, world::named_entity *> NamedEntities;

        /// Zones on the map
        std::list <world::zone *> Zones;

    private:
        /// name of map
        std::string Filename;
#endif // SWIG
    };
}

#endif // WORLD_AREA_H
