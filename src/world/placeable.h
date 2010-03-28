/*
 $Id: placeable.h,v 1.17 2009/04/26 18:53:00 ksterker Exp $

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
 * @author Kai Sterker <kai.sterker@gmail.com>
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
            /// not of any specific type
            UNKNOWN = 0,
            /// scenery object
            OBJECT = 1,
            /// creature
            CHARACTER = 2,
            /// something that can be picked up
            ITEM = 4,
            /// any of the above
            ANY = ITEM | CHARACTER | OBJECT
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
        virtual ~placeable();

        /**
         * Get type of this placeable.
         * @return one of OBJECT, CHARACTER or ITEM
         */
        placeable_type type () const
        {
            return Type;
        }

        /**
         * Get unique id of this placeable.
         * @return the unique id, or the empty string if it 
         * has none.
         */
        const std::string & uid () const;
        
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
        u_int16 max_length () const { return EntireMaxSize.x(); }
        /**
         * Get maximum placeable width.
         * @return max extension of placeable in y direction.
         */
        u_int16 max_width () const { return EntireMaxSize.y(); }
        /**
         * Get maximum placeable height.
         * @return max extension of placeable in z direction.
         */
        u_int16 max_height () const { return EntireMaxSize.z(); }

        s_int16 min_x () const { return EntireMinPos.x(); }
        s_int16 min_y () const { return EntireMinPos.y(); }
        s_int16 min_z () const { return EntireMinPos.z(); }

        /**
         * Get the minimum placement position
         * @return min placement position as a vector3
         */
        vector3<s_int16> entire_min () const { return EntireMinPos; }
        /**
         * Get the maximum placement size
         * @return max extension in all directions as a vector3
         */
        vector3<s_int16> entire_max () const { return EntireMaxSize; }

        /**
         * Get placeable's current length.
         * @return actual extension of placeable in x direction.
         */
        u_int16 length () const { return EntireCurSize.x(); }
        /**
         * Get placeable's current width.
         * @return actual extension of placeable in y direction.
         */
        u_int16 width () const { return EntireCurSize.y(); }
        /**
         * Get placeable's current height.
         * @return actual extension of placeable in z direction.
         */
        u_int16 height () const { return EntireCurSize.z(); }
        //@}

        s_int16 cur_x () const { return EntireCurPos.x(); }
        s_int16 cur_y () const { return EntireCurPos.y(); }
        s_int16 cur_z () const { return EntireCurPos.z(); }


       /**
         * Get maximum placeable length only accounting for solid shapes.
         * @return max extension of placeable in x direction.
         */
        u_int16 solid_max_length () const { return SolidMaxSize.x(); }
        /**
         * Get maximum placeable width only accounting for solid shapes.
         * @return max extension of placeable in y direction.
         */
        u_int16 solid_max_width () const { return SolidMaxSize.y(); }
        /**
         * Get maximum placeable height only accounting for solid shapes.
         * @return max extension of placeable in z direction.
         */
        u_int16 solid_max_height () const { return SolidMaxSize.z(); }

        s_int16 solid_min_x () const { return SolidMinPos.x(); }
        s_int16 solid_min_y () const { return SolidMinPos.y(); }
        s_int16 solid_min_z () const { return SolidMinPos.z(); }

        /**
         * Get placeable's current length only accounting for solid shapes
         * @return actual extension of placeable in x direction.
         */
        u_int16 solid_length () const { return SolidCurSize.x(); }
        /**
         * Get placeable's current width only accounting for solid shapes
         * @return actual extension of placeable in y direction.
         */
        u_int16 solid_width () const { return SolidCurSize.y(); }
        /**
         * Get placeable's current height only accounting for solid shapes
         * @return actual extension of placeable in z direction.
         */
        u_int16 solid_height () const { return SolidCurSize.z(); }

        /**
         * Get the minimum placement position of the solid
         * @return min placement position as a vector3
         */
        vector3<s_int16> solid_min () const { return SolidMinPos; }
        /**
         * Get the maximum size of the solid
         * @return max extension in all directions as a vector3
         */
        vector3<s_int16> solid_max () const { return SolidMaxSize; }
        //@}

        s_int16 solid_cur_x () const { return SolidCurPos.x(); }
        s_int16 solid_cur_y () const { return SolidCurPos.y(); }
        s_int16 solid_cur_z () const { return SolidCurPos.z(); }
        
        /**
         * Get placeable's map
         * @return map where the placeable exists
         */
        area & map() const { return Mymap; }
        
        /**
         * @name Solid / non-solid
         */
        //@{
        /**
         * Return whether this object is solid in its current state.
         * @return true if it is solid, false otherwise.
         */
        bool is_solid() const { return Solid; }
        
        /**
         * Return the surface of the object. Since it might be composed
         * of solid and non-solid components, this needs to be calculated
         * separately.
         *
         * @return the z-position of the object's solid components. 
         */
        s_int32 get_surface_pos() const;
        
        /**
         * Return the terrain type of this object.
         * @return pointer to the terrain. Do not delete.
         */
        const std::string *get_terrain() const;
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
        virtual bool put_state (base::flat & file) const;

        /**
         * Load %placeable state from stream.
         * @param file stream to load %placeable from.
         * @return \b true if loading successful, \b false otherwise.
         */
        virtual bool get_state (base::flat & file);

        /**
         * Save %placeable model name to stream. 
         * @param file stream to save the model name to.
         * @return \b true on success, \b false otherwise.
         */
        bool save_model (base::flat & file) const;

        /**
         * Load %placeable model from file. This is static data
         * that will never change throughout the game.
         * @param filename file to load the model from.
         * @return \b true on success, \b false otherwise.
         */
        bool load_model (const std::string & filename);
        
        /**
         * Load %placeable model from stream. This is static data
         * that will never change throughout the game.
         * @param model stream to load the model from.
         * @return \b true on success, \b false otherwise.
         */
        virtual bool load_model (base::flat & model);
        
        /**
         * Get the file the placeable model was loaded from.
         * @return the models file name.
         */
        std::string modelfile () const { return ModelFile; }
        //@}

#ifndef SWIG
        /**
         * Allow %placeable to be passed as python argument
         */
        GET_TYPE_NAME_VIRTUAL (world::placeable)
#endif

    protected:
        /// file this placeable's model was loaded from
        std::string ModelFile;
        /// representation of the placeable
        std::vector<world::placeable_model*> Model;
        /// bounding box of this placeable. It's updated when adding shapes. Only takes into account solid shapes.
        vector3<u_int16> SolidMaxSize;
        /// bounding box of this placeable. Takes into account both solid and non-solid shapes.
        vector3<u_int16> EntireMaxSize;
        /// position of this placeable. It's calculated when adding shapes.
        vector3<s_int16> SolidMinPos;
        /// position of this placeable. It's calculated when adding shapes. 
        vector3<s_int16> EntireMinPos;
        /// bounding box of this placeable. It's calculated when the state changes only from the solid shapes.
        vector3<u_int16> SolidCurSize;
        /// bounding box of this placeable. It's calculated when the state changes from both solid and non-solid shapes.
        vector3<u_int16> EntireCurSize;
        /// position of this placeable. It's calculated when the state changes.
        vector3<s_int16> SolidCurPos;
        /// position of this placeable. It's calculated when the state changes.
        vector3<s_int16> EntireCurPos;
        /// the placeables current state
        std::string State;
        /// whether placeable is character, scenery or item
        placeable_type Type;
        /// whether the placeable in its current state is solid or not 
        bool Solid;
        /// whether there is a solid added to SolidMinPos/SolidMaxSize yet
        bool HaveSolid;
        /// whether anything has been added to EntireMinPos/EntireMaxSize yet
        bool HaveEntire;
        /// the map this placeable belongs to
        area & Mymap;

    private:
        /// forbid passing by value
        placeable (const placeable & p);
    };
}

#endif
