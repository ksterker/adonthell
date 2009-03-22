/*
 $Id: area.h,v 1.12 2009/03/22 13:53:20 ksterker Exp $
 
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

#include "base/hash_map.h"
#include "base/diskio.h"

#include "world/chunk.h"

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
         * Remove all objects and characters from the map.
         */
        void clear();
        
        /**
         * Update state of all characters and objects on the map.
         * @todo decide if we need that, or if everything can be
         *     updated based on events.
         */
        void update();

        /**
         * Place object at a given index at the given location.
         * @param index index of object in the list of objects.
         * @param pos position to place object at.
         */
        bool put_entity (const u_int32& index, coordinates & pos);
        
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
         * @name Map entity creation.
         */
        //@{
        /** 
         * Adds an object to this map. Creates a new, empty object and returns
         * it, so that it can be initialized by the caller of this method.
         * The map will take ownership of the object.
         *
         * @return pointer to the added object in case of success, \e NULL otherwise.
         */
        placeable * add_entity (const placeable_type & type);
        
        /** 
         * Adds a new, named entity to the map. The enitiy of the specified type 
         * will be created and returned for further initialization.
         *   
         * @return pointer to the added object in case of success, \e NULL otherwise.
         */
        placeable * add_entity (const placeable_type & type, const std::string & id);

        /**
         * Adds an existing object as a new, named entity to the map. This is useful
         * if different instances of an entity (identified by their unique id) can
         * share the same graphical representation.
         *
         * @return pointer to the added object in case of success, \e NULL otherwise.
         */
        placeable * add_entity (placeable * object, const std::string & id);
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
        bool save (const std::string & fname, const base::diskio::file_format & format = base::diskio::BY_EXTENSION) const;        
        
        /**
         * Load %area state from file.
         * @param fname file name.
         * @return true on success, false otherwise.
         */
        bool load (const std::string & fname);
        //@}        
        
        /// This class is allowed to change and move objects on the map.
        friend class moving;

    private:
            
#ifndef SWIG
        /// The individual objects on the map
        std::vector <world::entity *> Entities;
        
        /// Named objects on the map
        std::hash_map <std::string, world::named_entity *> NamedEntities;
        
#endif // SWIG
    }; 
}

#endif // WORLD_AREA_H
