/*
 $Id: placeable.h,v 1.5 2008/07/10 20:19:43 ksterker Exp $
 
 Copyright (C) 2002 Alexandre Courbot <alexandrecourbot@linuxgames.com>
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
 * @file   world/placeable.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the placeable class.
 * 
 * 
 */

#ifndef WORLD_PLACEABLE_H
#define WORLD_PLACEABLE_H

#include "world/placeable_model.h"

namespace world
{
    class area;

    /// allowed types of objects on the map
    typedef enum 
        {
            UNKNOWN = 0, 
            /// scenery object
            OBJECT,
            /// creature
            CHARACTER, 
            /// something that can be picked up
            ITEM
        } placeable_type; 

    /**
     * Class representing a placeable, i.e. something (character, object, ...)
     * that can be placed on a map and occupies some space on it.
     *
     * What makes this class different from placeable_model is that it has a type,
     * that higher-level class can overwrite to indicate what kind of placeable it is
     * and a reference to the map the placeable belongs to, so update functions can 
     * check the terrain around the placeable.
     */
    class placeable : public placeable_model
    {
    public:
        /**
         * Create a thing on the map.
         * @param mymap map this thing belongs to.
         */
        placeable(area & mymap);
    
        /**
         * Destructor.
         */
        virtual ~placeable() { }

        /**
         * Get type of this placeable.
         * @return one of OBJECT, CHARACTER or ITEM
         */
        placeable_type type () const
        {
            return Type; 
        }
        
        /**
         * Update placeable each game cycle. 
         * @return true on success, false otherwise.
         * @todo if only characters really require this, 
         *      then limit to those for efficiency. 
         */
        virtual bool update ()
        {
            return true;
        }
        
#ifndef SWIG
        /**
         * Allow %placeable to be passed as python argument
         */
        GET_TYPE_NAME_VIRTUAL (world::placeable)
#endif
            
    protected:
        /// whether placeable is character, scenery or item    
        placeable_type Type; 
        /// the map this placeable belongs to
        area & Mymap;
    
    private:
        /// forbid passing by value
        placeable (const placeable & p);
    };
}

#endif
