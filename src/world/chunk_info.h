/*
 $Id: chunk_info.h,v 1.4 2009/02/01 15:18:24 ksterker Exp $
 
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

#include "world/placeable.h"
#include "world/shadow.h"

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
         * @param o the object at this point in world space
         * @param min the position of the object in world space
         * @param max the extend of the object in world space
         */
        chunk_info (placeable *o, const vector3<s_int32> & min, const vector3<s_int32> & max) 
        : Object (o), Min (min), Max (max), Shadow (NULL)
        {
        }

        /**
         * Create a copy of another chunk_info. 
         * @param ci object to copy.
         */
        chunk_info (const chunk_info & ci) 
        : Object (ci.Object), Min (ci.Min), Max (ci.Max), Shadow (ci.Shadow)
        {
        }

        /**
         * Compare if two chunk infos are the same.
         * @return True if both position and object contained are equal.
         */
        bool operator == (const chunk_info & ci) const;

        /**
         * Return "real" position, taking placeable shape offset into account.
         * @return lower coordinate of bounding box
         */
        vector3<s_int32> real_min () const
        {
            return Min + vector3<s_int32>(Object->min_x(), Object->min_y(), Object->min_z());
        }

        /**
         * Return "real" position, taking placeable shape offset into account.
         * @return upper coordinate of bounding box
         */
        vector3<s_int32> real_max () const
        {
            return Max + vector3<s_int32>(Object->min_x(), Object->min_y(), Object->min_z());
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
        void add_shadow (shadow *s) { Shadow = s; }
        
        /**
         * Remove shadow from the placeable.
         */
        void remove_shadow () { Shadow = NULL; }
        
        /**
         * Get shadow that has been cast on this placeable.
         * @return shadow on this placeable or NULL.
         */
        shadow *get_shadow () const { return Shadow; }
        //@}
                
        /// pointer to map object
        placeable * Object;
        /// position of the object
        vector3<s_int32> Min;
        /// extend of the object
        vector3<s_int32> Max;
        
    private:
        /// shadow cast on this object 
        shadow *Shadow;
    };
}

#endif
