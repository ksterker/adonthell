/*
 $Id: chunk_info.h,v 1.10 2009/04/26 18:52:59 ksterker Exp $
 
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
        : Min (min), Max (max), Entity (e)
        {
            calc_solid_max();
        }

        /**
         * Create a copy of another chunk_info. 
         * @param ci object to copy.
         */
        chunk_info (const chunk_info & ci) 
        : Min (ci.Min), Max (ci.Max), Entity (ci.Entity), Shadow (ci.Shadow)
        {
            calc_solid_max();
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
        world::placeable *get_object() const
        {
            return Entity->get_object();
        }
        
        /**
         * Get pointer to the entity.
         * @return pointer to the underlying entity.
         */
        world::entity *get_entity() const
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
         * Return "real" position, taking only solid placeable shape offset into account.
         * @return lower coordinate of bounding box
         */
        vector3<s_int32> solid_min () const
        {
            const placeable *object = Entity->get_object(); 
            return Min + vector3<s_int32>(object->solid_min_x(), object->solid_min_y(), object->solid_min_z());
        }

        /**
         * Return "real" position, taking only solid placeable shape offset into account.
         * @return upper coordinate of bounding box
         */
        vector3<s_int32> solid_max () const
        {
            const placeable *object = Entity->get_object();
            return  SolidMax + vector3<s_int32>(object->solid_min_x(), object->solid_min_y(), object->solid_min_z());
        }
        
        /**
         * @name Object shadow
         *
         * Handling of shadow cast onto this placeable by another object.
         */
        //@{
        /**
         * Add a shadow to this placeable.
         * @param shadow the shadow to add.
         */
        void add_shadow ( const shadow_info & s) 
        {
            // cast shadow on scenery only
            if (get_object()->type() == world::OBJECT)
            {
                Shadow.push_back (s); 
            }
        }
        
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
        
#ifndef SWIG
        /**
         * Allow %chunk_info to be passed as python argument
         */
        GET_TYPE_NAME (world::chunk_info)
#endif

    private:
        /**
         * Calculate constant SolidMax
         */
         void calc_solid_max()
         {
            const placeable *object = Entity->get_object();
            SolidMax = vector3<s_int32>(Min.x() + object->solid_max_length(),
                                        Min.y() + object->solid_max_width(),
                                        Min.z() + object->solid_max_height());
         }
         
        /// pointer to map object
        entity * Entity;
        /// shadow cast on this object 
        std::vector<shadow_info> Shadow;
        /// extend of the solid portion of the object
        vector3<s_int32> SolidMax;
    };
}

#endif
