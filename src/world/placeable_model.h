/*
 $Id: placeable_model.h,v 1.4 2007/06/03 02:28:38 ksterker Exp $
 
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
 * @file   world/placeable_model.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the placeable_model class.
 * 
 * 
 */


#ifndef WORLD_PLACEABLE_MODEL_H
#define WORLD_PLACEABLE_MODEL_H

#include <map>

#include "world/placeable_area.h"

namespace world
{
    /**
     * Represents a placeable, i.e. something (character, object, ...)
     * that can be placed on a map and occupies some space on it.
     *
     * This class is separated from placeable to allow easy editing
     * without depending on the whole map stuff. While it is suitable
     * for editing, it can't be actually placed on a map. Use placeable
     * for that instead.
     * 
     * FIXME: seems a lot in here actually duplicates stuff now located
     * in the animation class. Need to update this accordingly.
     */
    class placeable_model
    {
    public:
        typedef std::map <std::string, placeable_area>::iterator iterator;

        /**
         * Constructor.
         */
        placeable_model();
        
        /**
         * @name Map object states
         * A state is usually represented by an %animation
         */
        //@{
        iterator begin ()
        {
            return States.begin (); 
        }

        iterator end () 
        {
            return States.end (); 
        }

        world::placeable_area * current_state ();

        world::placeable_area * get_model_state (const std::string & name);
    
        const std::string current_state_name() const;
    
        world::placeable_area * add_state (const std::string & name);

        bool del_state (const std::string & name);
    
        void set_state (const std::string & name);
        //@}
        
        /**
         * Get file from which this model was loaded.
         * @return file name of this model.
         */
        const std::string & filename()
        {
            return Filename;
        }
        
        /**
         * @name Loading / Saving
         */
        //@{
        /**
         * Save %model state to stream. 
         * @param file stream to save model to.
         * @return \b true if saving successful, \b false otherwise.
         */
        bool put_state (base::flat & file) const;
        
        /**
         * Load model state from stream. 
         * @param file stream to load model from.
         * @return \b true if loading successful, \b false otherwise.
         */
        bool get_state (base::flat & file);
        //@}

        /**
         * This friendship is needed so placeable_model_gfx
         * can modify the Has_changed member.
         */
        friend class placeable_model_gfx;
        
    protected:
        /// filename of this object
        mutable std::string Filename;
        /// possible states of this object
        mutable std::map <std::string, placeable_area> States;
        /// current state of this object
        std::map <std::string, placeable_area>::iterator Current_state;
        /// whether state of object has changed recently
        bool State_changed;
    }; 
}

#endif
