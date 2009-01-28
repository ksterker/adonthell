/*
 $Id: placeable.h,v 1.9 2009/01/28 21:39:10 ksterker Exp $
 
 Copyright (C) 2002 Alexandre Courbot <alexandrecourbot@linuxgames.com>
 Copyright (C) 2008/2009 Kai Sterker <kai.sterker@gmail.com>
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
 * @authos Kai Sterker <kai.sterker@gmail.com> 
 *
 * @brief  Declares the placeable class.
 * 
 * 
 */

#ifndef WORLD_PLACEABLE_H
#define WORLD_PLACEABLE_H

#include "world/placeable_model.h"
#include "world/shadow.h"

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
     * that can be placed on a map and occupies some space on it. Each placeable
     * is represented by one or more placeable_models, that contain the graphics
     * and %collision data of the placeable. 
     *
     * It has a type that higher-level classes can overwrite to indicate what kind of 
     * placeable it is (%item, %character or scenery) and a reference to the map it 
     * belongs to, so update functions can check the terrain around the placeable.
     */
    class placeable
    {
    public:
        /// type definitions for iterator over models
        typedef std::vector<world::placeable_model*>::const_iterator iterator;
        
        /**
         * Create a thing on the map.
         * @param mymap map this thing belongs to.
         */
        placeable (area & mymap);
    
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
        
        /**
         * @name Placeable representation
         *
         * The sprites and shapes a placeable is composed of.
         */
        //@{
        /**
         * Add model to the placeable. Each placeable needs at least
         * one model, but can be composed of more than one.
         * @param model model 
         */
        void add_model (placeable_model *model);
        
        /**
         * Return const iterator to the first model the placeable
         * consists of.
         * @return iterator pointing to first model.
         */
        iterator begin () const { return Model.begin (); }
        
        /**
         * Return const iterator indicating the end of the list 
         * of models the placeable consists of.
         * @return iterator pointing after last model.
         */
        iterator end () const { return Model.end (); }
        //@}
        
        /**
         * @name Placeable extension
         *
         * The maximum size of a placeable is the space that
         * encloses all its models with all their states. The
         * current size may vary from that, depending on its 
         * current state.
         */
        //@{
        /**
         * Get maximum placeable length.
         * @return max extension of placeable in x direction.
         */
        u_int16 max_length () const { return MaxSize.x(); }
        /**
         * Get maximum placeable width.
         * @return max extension of placeable in y direction.
         */
        u_int16 max_width () const { return MaxSize.y(); }
        /**
         * Get maximum placeable height.
         * @return max extension of placeable in z direction.
         */
        u_int16 max_height () const { return MaxSize.z(); }
        
        s_int16 min_x () const { return MinPos.x(); }
        s_int16 min_y () const { return MinPos.y(); }
        s_int16 min_z () const { return MinPos.z(); }
        
        /**
         * Get placeable's current length.
         * @return actual extension of placeable in x direction.
         */
        u_int16 length () const { return CurSize.x(); }
        /**
         * Get placeable's current width.
         * @return actual extension of placeable in y direction.
         */
        u_int16 width () const { return CurSize.y(); }
        /**
         * Get placeable's current height.
         * @return actual extension of placeable in z direction.
         */
        u_int16 height () const { return CurSize.z(); }        
        //@}

        s_int16 cur_x () const { return CurPos.x(); }
        s_int16 cur_y () const { return CurPos.y(); }
        s_int16 cur_z () const { return CurPos.z(); }
        
        
        /**
         * @name Placeable shadow
         *
         * Handling of shadow cast onto this placeable by another object.
         * For now, only one shadow can be cast per placeable. Possibly,
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
        shadow *get_shadow () { return Shadow; }
        //@}
        
        /**
         * @name Placeable state
         *
         * The state usually determines the representation
         * that is used to display the placeable on the map.
         */
        //@{
        /**
         * Set the state of the underlying model. This will
         * become the placeables new state, even if one or more
         * of the models do not have a representation for this
         * state.
         *
         * @param state the new state of the placeable.
         */
        void set_state (const std::string & state);
        
        /**
         * Get the current state of the placeable.
         * @return current state of the placeable.
         */
        const std::string & state () const { return State; }
        //@}
        
        /**
         * @name Loading / Saving
         */
        //@{
        /**
         * Save %placeable state to stream. 
         * @param file stream to save %placeable to.
         * @return \b true if saving successful, \b false otherwise.
         */
        bool put_state (base::flat & file) const;
        
        /**
         * Load %placeable state from stream. 
         * @param file stream to load %placeable from.
         * @return \b true if loading successful, \b false otherwise.
         */
        bool get_state (base::flat & file);
        //@}
        
#ifndef SWIG
        /**
         * Allow %placeable to be passed as python argument
         */
        GET_TYPE_NAME_VIRTUAL (world::placeable)
#endif
            
    protected:
        /// representation of the placeable
        std::vector<world::placeable_model*> Model;
        /// bounding box of this placeable. It's updated when adding shapes.
        vector3<u_int16> MaxSize;
        /// position of this placeable. It's calculated when adding shapes.
        vector3<s_int16> MinPos;
        /// bounding box of this placeable. It's calculated when the state changes.
        vector3<u_int16> CurSize;
        /// position of this placeable. It's calculated when the state changes.
        vector3<s_int16> CurPos;
        /// the placeables current state
        std::string State;
        /// whether placeable is character, scenery or item    
        placeable_type Type; 
        /// the map this placeable belongs to
        area & Mymap;
        
    private:
        /// shadow cast on this object 
        shadow *Shadow;
        /// forbid passing by value
        placeable (const placeable & p);
    };
}

#endif
