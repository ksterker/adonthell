/*
 $Id: mapview.h,v 1.4 2008/07/10 20:19:42 ksterker Exp $
 
 Copyright (C) 2008 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file   world/mapview.h
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Defines a part of the map visible on screen.
 * 
 */

#include <list>
#include <cmath>
#include <cstdlib>

#include "gfx/surface.h"
#include "python/method.h"
#include "world/chunk.h"

namespace world
{
    /**
     * Displays a part of a map on screen. Which part of a map
     * is displayed is determined by a python script that is
     * running once per game cycle to determine the new position
     * of the view.
     */
    class mapview : public gfx::drawable
    {
    public:
        /**
         * Create a new view of the map with given length and height.
         * @param length extension of mapview along x axis in pixels.
         * @param height extension of mapview along y axis in pixels.
         */
        mapview (const u_int32 & length, const u_int32 & height);
        
        /**
         * @name Member access.
         */
        //@{
        /**
         * Set the map being displayed by the mapview.
         * @param map pointer to the map instance.
         */
        void set_map (chunk * map)
        {
            Map = map;
        }
        
        /**
         * Get the map this view is attached to.
         * @return map this view is attached to.
         */
        chunk * get_map () const
        {
            return Map;
        }
        
        /**
         * Set the schedule script which will determine the position of the mapview.
         * It will be called once per game cycle with the mapview instance as its
         * first parameter and any parameters passed into this method as additional
         * arguments.
         *
         * @param method name of the method in <i>schedules/map/mapview.py</i> that is
         *     used to determine the position of the mapview.
         * @param extraArgs optional arguments that will be passed to the schedule in
         *     addition to the mapview instance.
         * @return \b true on success, \b false otherwise.
         */
        bool set_schedule (const std::string & method, PyObject *extraArgs = NULL);
        //@}
        
        /**
         * @name Position manipulation
         */
        //@{
        /**
         * Update the position of the mapview. Call this every game
         * cycle to make sure the position of the mapview is up to
         * date.
         */
        bool update ()
        {
            if (Z != FinalZ)
            {
                if (std::abs (FinalZ - Z) < std::abs (Speed)) Z = FinalZ;
                else Z += Speed;
            }
            
            if (Schedule)
            {
                return Schedule->execute (Args);
            }
            
            return true;
        }
        
        /**
         * Center the mapview on the given position.
         * @param x position on the world grids x axis.
         * @param y position on the world grids y axis.
         * @param ox pixel offset on x axis.
         * @param oy pixel offset on y axis.
         */
        void center_on (const u_int16 & x, const u_int16 & y, const u_int16 & ox = 0, const u_int16 & oy = 0);
        
        /**
         * Center on the given pixel coordinates.
         * @param x position on the map in pixels
         * @param y position on the map in pixels
         */
        void center_on (const u_int32 & x, const u_int32 & y)
        {
            center_on (x / SQUARE_SIZE, y / SQUARE_SIZE, x % SQUARE_SIZE, y % SQUARE_SIZE);
        }
        
        /**
         * Set the height of the mapview. 
         * @param height the new height of the mapview.
         */
        void set_z (const s_int32 & height)
        {
            Z = height;
            FinalZ = height;
        }
        
        /**
         * Return the height the mapview is going to display. It might 
         * currently be in process of scrolling to that height.
         * @return height of the mapview.
         */
        s_int32 get_z () const
        {
            return FinalZ;
        }
        
        /**
         * Smooth transition of the mapview to the new height.
         * @param height the new height to transition to.
         * @param speed number of pixels to scroll during each update.
         */
        void scroll_to_z (const s_int32 & height, const u_int16 & speed)
        {
            FinalZ = height;
            Speed = speed * (Z > FinalZ ? -1 : 1);
        }
        //@}
        
        /**
         * @name Rendering
         */
        //@{
        /** 
         * Draw the object on the %screen.
         * 
         * @param x X position where to draw.
         * @param y Y position where to draw.
         * @param da_opt optional drawing_area to use during the drawing operation.
         * @param target pointer to the surface where to draw the drawable. If NULL,
         *               draw on the screen.
         */
        void draw (const s_int16 & x, const s_int16 & y, const gfx::drawing_area * da_opt = NULL, gfx::surface * target = NULL) const;        
        //@}
        
        /**
         * Loading / Saving
         */
        //@{
        /**
         * Save %mapview state to stream. 
         * @param file stream to save %mapview to.
         * @return \b true if saving successful, \b false otherwise.
         */
        bool put_state (base::flat & file) const;
        
        /**
         * Load %mapview state from stream. 
         * @param file stream to load %mapview from.
         * @return \b true if loading successful, \b false otherwise.
         */
        bool get_state (base::flat & file);
        //@}
        
#ifndef SWIG
        /**
         * Allow %mapview to be passed as python argument
         */
        GET_TYPE_NAME (world::mapview)
#endif
        
    private:
        /**
         * Draw the objects contained in queue in proper order on screen.
         * @param drawqueue list of objects to render.
         * @param da rectangle restricting the visible area.
         * @param target surface to draw on. NULL for screen surface.
         */
        void render (std::list <world::chunk_info> & drawqueue, const gfx::drawing_area & da, gfx::surface * target) const;
        
        /// the map displayed by this view.
        chunk *Map;

        /**
         * @name Positioning script 
         */
        //@{
        /// the python method determining %area of map in view.
        python::method *Schedule;
        /// arguments passed to the %mapview schedule
        PyObject *Args;
        //@}
        
        /**
         * @name Height related members
         */
        //@{
        /// current height of the mapview
        s_int32 Z;
        /// the height to scroll mapview to
        s_int32 FinalZ; 
        /// nbr of pixels to scroll per update
        s_int16 Speed;
        //@}
        
        /**
         * @name Rendering coordinates
         */
        //@{
        /// x-offset of map in view (if map smaller than view).
        u_int16 Ox;
        /// y-offset of map in view (if map smaller than view).
        u_int16 Oy;
        /// position from where to start rendering map (x axis).
        s_int32 Sx;
        /// position from where to start rendering map (y axis).
        s_int32 Sy;
        //@}
    };
}
