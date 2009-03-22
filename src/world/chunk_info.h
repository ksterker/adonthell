/*
 $Id: chunk_info.h,v 1.7 2009/03/22 13:53:20 ksterker Exp $
 
 Copyright (C) 2008/2009 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file   world/chunk_info.h
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Declares the chunk_info class.
 * 
 * 
 */


#ifndef WORLD_CHUNK_INFO_H
#define WORLD_CHUNK_INFO_H

#include "world/entity.h"
#include "world/shadow_info.h"

namespace world
{
    /**
     * Container to store an object with its world-space coordinates.
     * Coordinates are expressed by an axis aligned bounding box (AABB).
     */
    class chunk_info
    {
    public:
        /**
         * Create a new chunk_info.
         * @param e the object at this point in world space
         * @param min the position of the object in world space
         * @param max the extend of the object in world space
         */
        chunk_info (entity *e, const vector3<s_int32> & min, const vector3<s_int32> & max) 
        : Entity (e), Min (min), Max (max)
        {
        }

        /**
         * Create a copy of another chunk_info. 
         * @param ci object to copy.
         */
        chunk_info (const chunk_info & ci) 
        : Entity (ci.Entity), Min (ci.Min), Max (ci.Max), Shadow (ci.Shadow)
        {
        }

        /**
         * Compare if two chunk infos are the same.
         * @return True if both position and object contained are equal.
         */
        bool operator == (const chunk_info & ci) const;

        /**
         * Get pointer to the map object instance.
         * @return pointer to the underlying placeable.
         */
        placeable *get_object() const
        {
            return Entity->get_object();
        }
        
        /**
         * Get pointer to the entity.
         * @return pointer to the underlying entity.
         */
        entity *get_entity() const
        {
            return Entity;
        }
        
        /**
         * Return "real" position, taking placeable shape offset into account.
         * @return lower coordinate of bounding box
         */
        vector3<s_int32> real_min () const
        {
            const placeable *object = Entity->get_object(); 
            return Min + vector3<s_int32>(object->min_x(), object->min_y(), object->min_z());
        }

        /**
         * Return "real" position, taking placeable shape offset into account.
         * @return upper coordinate of bounding box
         */
        vector3<s_int32> real_max () const
        {
            const placeable *object = Entity->get_object(); 
            return Max + vector3<s_int32>(object->min_x(), object->min_y(), object->min_z());
        }
        
        /**
         * @name Object shadow
         *
         * Handling of shadow cast onto this placeable by another object.
         * For now, only one shadow can be cast onto placeable. Possibly,
         * this needs to be extented to multiple shadows. 
         */
        //@{
        /**
         * Add a shadow to this placeable.
         * @param shadow the shadow to add.
         */
        void add_shadow ( const shadow_info & s) { Shadow.push_back (s); }
        
        /**
         * Remove shadow from the placeable.
         * @param x x-coordinate of shadow to remove.
         * @param y y-coordinate of shadow to remove.
         */
        void remove_shadow (const s_int32 & x, const s_int32 & y) 
        { 
            for (std::vector<shadow_info>::iterator shdw = Shadow.begin(); shdw != Shadow.end(); shdw++)
            {
                if (shdw->X == x && shdw->Y == y)
                {
                    Shadow.erase (shdw);
                    return;
                }
            }
        }
        
        /**
         * Get pointer to all shadows cast on this placeable.
         * @return the vector of shadows.
         */
        const std::vector<shadow_info> *get_shadow () const { return &Shadow; }
        //@}
        
        /// position of the object
        vector3<s_int32> Min;
        /// extend of the object
        vector3<s_int32> Max;
        
    private:
        /// pointer to map object
        entity * Entity;
        /// shadow cast on this object 
        std::vector<shadow_info> Shadow;
    };
}

#endif
