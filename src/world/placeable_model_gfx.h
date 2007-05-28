/*
 $Id: placeable_model_gfx.h,v 1.3 2007/05/28 22:26:14 ksterker Exp $
 
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
 * @file   world/placeable_model_gfx.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the placeable_model_gfx class.
 * 
 * 
 */


#ifndef WORLD_PLACEABLE_MODEL_GFX_H
#define WORLD_PLACEABLE_MODEL_GFX_H

#include "world/placeable_area_gfx.h"
#include "world/placeable_model.h"
#include "base/file.h"

namespace world
{
    /**
     * The graphical representation of a placeable_model.
     *
     * Each object of this class has to be associated at constructor
     * time with a placeable_model that it will stick to all life long.
     * 
     */
    class placeable_model_gfx : public gfx::drawable
    {
    public:
        placeable_model_gfx (placeable_model & target);

        ~placeable_model_gfx();
    
        placeable_area_gfx * current_gfx();

        void set_gfx (const std::string & name);

        world::placeable_area_gfx * get_gfx(const std::string & name);

        world::placeable_area_gfx * add_gfx(const std::string & name);
        
        bool del_gfx(const std::string & name);
    
        bool update ();
    
        /**
         * @name Rendering
         */
        //@{
        /** 
         * Draws the placeable representation.
         *
         * @todo: this function should call the Python drawing callback
         * function, if set. Currently it dumbly calls placeable_model::draw().
         * 
         * @param x X position where to draw
         * @param y Y position where to draw
         * @param da_opt Optionnal drawing area to use during the blit operation
         * @param target Optionnal surface to draw to (defaults to the screen)
         */
        void draw(s_int16 x, s_int16 y, const gfx::drawing_area * da_opt = NULL,
                  gfx::surface * target = NULL) const;
    
        void draw_walkable(s_int16 x, s_int16 y, const gfx::drawing_area * da_opt = NULL,
                           gfx::surface * target = NULL) const;

        void draw_border(s_int16 x, s_int16 y, const gfx::drawing_area * da_opt = NULL,
                         gfx::surface * target = NULL) const;
        //@}
        
        /**
         * @name Loading / Saving
         */
        //@{
        /**
         * Save model state to stream. 
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
        GET_TYPE_NAME_VIRTUAL(world::placeable_model_gfx)
#endif // SWIG
            
    protected:
        /// list of graphics
        mutable std::map <const std::string, placeable_area_gfx *> Gfxs;
        /// current graphics
        std::map <const std::string, placeable_area_gfx *>::iterator Current_gfx;
        /// model this graphics belong to
        placeable_model & Target;
        
    private:
        /**
         * Forbid copy construction
         */
        placeable_model_gfx(placeable_model_gfx &);
    };
}
    
#endif
