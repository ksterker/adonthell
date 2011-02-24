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

#include "base/hash_map.h"
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
        : Min (min), Max (max), Entity (e), Action(NULL)
        {
            calc_solid_max();
        }

        /**
         * Cleanup.
         */
        ~chunk_info ()
        {
            delete Action;
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
         * @name Object interaction
         */
        //@{
        /**
         * Assign an action to that location.
         * @return an empty action that must be initialized.
         */
        world::action *set_action()
        {
            if (Action == NULL)
            {
                Action = new world::action();
            }
            return Action;
        }
        
        /**
         * Get pointer to action associated with
         * the specific location or entity. If an action
         * is set for the location, that is returned.
         * Otherwise, the entity-specific action is returned.
         * May return NULL if there is neither.
         *
         * @return an action or NULL, if none is assigned.
         */
        world::action *get_action() const
        {
            if (Action) return Action;
            else return Entity->get_action();
        }
        
        /**
         * Check whether this location has a specific
         * action assigned. Allows to distinguish between
         * location and entity based action.
         * @return true if this location has an action, false otherwise.
         */
        bool has_action () const
        {
            return Action != NULL;
        }
        //@}
        
        /**
         * @name Position information
         */
        //@{
        /**
         * Return "real" position, taking only solid placeable shape offset into account.
         * @return lower coordinate of bounding box
         */
        vector3<s_int32> solid_min () const
        {
            const placeable *object = Entity->get_object(); 
            return center_min() + object->solid_min();
        }

        /**
         * Return "real" position, taking only solid placeable shape offset into account.
         * @return upper coordinate of bounding box
         */
        vector3<s_int32> solid_max () const
        {
            const placeable *object = Entity->get_object();
            //Subtracting object->entire_min to get the [0, 0, 0] location
            return  SolidMax - object->entire_min() + object->solid_min();
        }

        /**
         * Return "real" position, but adjusted so it points to the [0, 0, 0] portion of the object.
         * This is needed because SolidMinPos and EntireMinPos are relative to the [0, 0, 0] location, not the position.
         * @return position of the [0, 0, 0] of the object
         */
        vector3<s_int32> center_min () const
        {
            const placeable *object = Entity->get_object();
            return Min - object->entire_min();
        }
        //@}
        
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
        void add_shadow (placeable_model* model, const shadow_info & s)
        {
            Shadow[model].push_back (s);
        }
        
        /**
         * Remove shadow from the placeable.
         * @param x x-coordinate of shadow to remove.
         * @param y y-coordinate of shadow to remove.
         */
        void remove_shadow (const s_int32 & x, const s_int32 & y) 
        { 
            std::hash_map<placeable_model*, std::vector<shadow_info> >::iterator i;
            std::vector<shadow_info>::iterator shdw;

            for (i = Shadow.begin(); i != Shadow.end(); i++)
            {
                for (shdw = i->second.begin(); shdw != i->second.end(); shdw++)
                {
                    if (shdw->X == x && shdw->Y == y)
                    {
                        i->second.erase (shdw);
                        return;
                    }
                }
            }
        }
        
        /**
         * Get pointer to all shadows cast on this placeable.
         * @return the vector of shadows.
         */
        const std::vector<shadow_info> *get_shadow (placeable_model* model)
        {
            return &Shadow[model];
        }
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
        /// Forbid copy construction
        chunk_info (const chunk_info & ci); 

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
        /// location specific action
        action * Action;
        /// shadow cast on this object 
        std::hash_map<placeable_model*, std::vector<shadow_info> > Shadow;
        /// extend of the solid portion of the object
        vector3<s_int32> SolidMax;
    };
}

#endif
