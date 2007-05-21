/*
 $Id: object.h,v 1.2 2007/05/21 04:44:11 ksterker Exp $
 
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
 * @file   world/object.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the object class.
 * 
 * 
 */

#ifndef WORLD_OBJECT_H
#define WORLD_OBJECT_H

#include "base/file.h"
#include "world/placeable.h"

namespace world
{
    /**
     * Implements a scenery object.
     *
     * An object is a placeable that has some update abilities and can be
     * placed several times on the same map. Note, however, that all instances of
     * the same map object will have the same state and will always look the same.
     * While this is most often ok, some special objects (like doors) will require
     * you to load one object per instance.
     * 
     */
    class object : public placeable
    {
    public:
        /**
         * Create a new object on the given map.
         * @param mymap map the object belongs to.
         */
        object (area & mymap); 

        /**
         * Update %object state. Called every game cycle.
         */
        virtual bool update ()
        {
            return true;
        }

        /**
         * Loading / Saving
         */
        //@{
        /**
         * Save %object state to stream. 
         * @param file stream to save %object to.
         * @return \b true if saving successful, \b false otherwise.
         */
        bool put_state (base::flat & file) const;
        
        /**
         * Load %object state from stream. 
         * @param file stream to load %object from.
         * @return \b true if loading successful, \b false otherwise.
         */
        bool get_state (base::flat & file);
        
        /**
         * Save %object state to file.
         * @param fname file name.
         * @return true on success, false otherwise.
         */
        bool save (const std::string & fname) const;
        
        /**
         * Load %object state from file.
         * @param fname file name.
         * @return true on success, false otherwise.
         */
        bool load (const std::string & fname);
        //@}
    };
}

#endif
