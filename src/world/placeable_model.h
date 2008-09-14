/*
 $Id: placeable_model.h,v 1.8 2008/09/14 14:25:26 ksterker Exp $
 
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

#include "gfx/sprite.h"
#include "world/placeable_shape.h"

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
     */
    class placeable_model
    {
    public:
        /// short name to iterator over shapes map
        typedef std::map <std::string, placeable_shape>::iterator iterator;

        /**
         * Constructor.
         */
        placeable_model();
        
        /**
         * Destructor.
         */
        virtual ~placeable_model ();
        
        /**
         * @name Map object shapes.
         * Each map object is accompanied by a distinct shape.
         */
        //@{
        /**
         * Return iterator to first object shape.
         * @return iterator pointing to first shape.
         */
        iterator begin ()
        {
            return Shapes.begin ();
        }

        /**
         * Return iterator pointing after last object shape.
         * @return iterator pointing past last shape.
         */
        iterator end () 
        {
            return Shapes.end (); 
        }

        /**
         * Get current shape. This is the shape matching the %animation
         * being played for this object.
         * @return current shape.
         */
        world::placeable_shape * current_shape ();

        /**
         * Get name of current shape.
         * @return name of current shape.
         */
        const std::string current_shape_name() const;
        
        /**
         * Get shape with given name.
         * @param name of the shape.
         * @return shape or NULL if no such shape exists.
         */
        world::placeable_shape * get_shape (const std::string & name);
    
        /**
         * Add a shape with given name. The shape is created internally
         * and returned for further initialization.
         * @param name name of shape to create.
         * @return newly created shape.
         */
        world::placeable_shape * add_shape (const std::string & name);

        /**
         * Delete the shape with given name.
         * @param name name of shape to delete.
         * @return true on success, false otherwise.
         */
        bool del_shape (const std::string & name);
    
        /**
         * Make shape with given name the current shape. If no such 
         * shape exists, the current shape remains unchanged.
         * @param name name of the shape to activate.
         */
        void set_shape (const std::string & name);
        //@}
        
        /**
         * @name Map object graphics.
         * Each map object is represented on screen by a sprite.
         */
        //@{
        /**
         * Assign a (new) sprite to the model. The currently assigned
         * sprite (if any) will be deleted.
         * @param name file containing the new graphical representation of this model.
         */
        void set_sprite (const std::string & name);
        
        /**
         * Get sprite associated with this model. If the sprite
         * is not valid yet, if will be loaded the first time 
         * this method is called.
         * @return Sprite or NULL if none is assigned yet.
         */
        gfx::sprite * get_sprite ();
        //@}
        
        /**
         * @name Loading / Saving
         */
        //@{
        /**
         * Get file from which this model was loaded.
         * @return file name of this model.
         */
        const std::string & filename()
        {
            return Filename;
        }
        
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

#ifndef SWIG
        /**
         * Allow %placeable_model to be passed as python argument
         */
        GET_TYPE_NAME_VIRTUAL (world::placeable_model)
#endif
            
    protected:
        /// filename of this object
        mutable std::string Filename;
        /// possible states of this object
        mutable std::map <std::string, placeable_shape> Shapes;
        /// current state of this object
        std::map <std::string, placeable_shape>::iterator CurrentShape;
        /// the sprite associated with this model
        gfx::sprite Sprite;
        
    private:
        /// forbid passing by value
        placeable_model (const placeable_model & p);
    };    
}

#endif
