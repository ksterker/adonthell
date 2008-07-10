/*
 $Id: area.h,v 1.8 2008/07/10 20:19:40 ksterker Exp $
 
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

#include "world/entity.h"
#include "world/square.h"
#include "world/chunk.h"

namespace world
{    
    /**
     * The plane of existance. A map consists of a grid of squares that
     * contain the actual scenery elements, characters and items.
     */
    class area
    {        
    public:
        /**
         * Create an empty map. Call resize to set its initial size
         * or load its state from disk.
         */
        area () { }
        
        /**
         * Delete the map and everything on it.
         */
        ~area ();
        
        chunk Chunk;
        
        /**
         * @name The Grid
         */
        //@{
        /**
         * Return size of the map grid along the X axis.
         * @return extension of the map in X direction.
         */
        u_int16 length () const
        {
            return Grid.size ();
        }
        
        /**
         * Return size of the map grid along the Y axis.
         * @return extension of the map in Y direction.
         */
        u_int16 height () const
        {
            if (Grid.size ()) return Grid[0].size ();
            else return 0; 
        }
        
        /**
         * Change the size of the map grid. It's probably a good
         * idea to do that before placing anything onto the map,
         * although growing the map should have no ill effect.
         * @param nx new number of cells in x direction.
         * @param ny new number of cells in y direction.
         */
        void resize (const u_int16 & nx, const u_int16 & ny);

        /**
         * Get the map square at the given location.
         * @param x x location on the map grid
         * @param y y location on the map grid
         * @return square at given position on the grid.
         */
        square * get (const u_int16 & x, const u_int16 & y);         
        //@}
        
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
        bool put_entity (const u_int32 & index, coordinates & pos);
        
        /**
         * Get named entity.
         * @param id unique identifier for the entity to retrieve.
         * @return the matching entity, or NULL if no such entity exists.
         */
        placeable * get_entity (const std::string & id);
        
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
        
        /// This class is allowed to change and move objects on the map.
        friend class moving;

    private:
            
#ifndef SWIG
        /**
         * Add static objects to map at given position.
         * @param obj the object to add
         * @param pos position where to add object
         * @return true on success, false otherwise
         */
        bool put (placeable * obj, coordinates & pos); 
        
        /**
         * Add moveable object to the map. Position will be the
         * object's current position.
         * @param obj the object to add.
         * @return true on success, false otherwise
         */
        bool put (moving * obj); 
        
        /**
         * Remove given static object from given location.
         * @param obj the object to remove from the map.
         * @param pos the location to remove the object from.
         * @return true on success, false otherwise.
         */
        bool remove (placeable * obj, coordinates & pos);
        
        /**
         * Remove given moveable object from the map.
         * @param obj the object to remove from the map.
         * @return true on success, false otherwise.
         */
        bool remove (moving * obj); 
                
        /**
         * Add object to all map squares enclosed by the rectangle [pos.x(), pos.y(); end_x, end_y].
         * This method should not be called directly. Use put(moving*) or put(object*, coordinates&)
         * instead.
         * @param end_x x coordinate of lower right square
         * @param end_y y coordinate of lower right square
         * @param obj the object to add
         * @param pos position where to add object
         * @param ground_z position of ground under object
         * @return true on success, false otherwise
         */
        bool put (u_int16 & end_x, u_int16 & end_y, placeable *obj, coordinates & pos, const s_int32 & ground_z); 
        
        /// The individual map squares
        std::vector <std::vector <square> > Grid;
        
        /// The individual objects on the map
        std::vector <world::entity *> Entities;
        
        /// Named objects on the map
        std::hash_map <std::string, world::named_entity *> NamedEntities;
        
#endif // SWIG
    }; 
}

#endif // WORLD_AREA_H
