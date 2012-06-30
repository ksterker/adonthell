/*
 $Id: mapview.h,v 1.9 2009/04/03 21:56:53 ksterker Exp $
 
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

#ifndef WORLD_MAPVIEW_H
#define WORLD_MAPVIEW_H

#include <cmath>
#include <cstdlib>

#include <adonthell/python/method.h>
#include "renderer.h"
#include "zone.h"

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
         * Create a new view of the map with default renderer and length and height
         * matching the screen length and height.
         */
        mapview ();
        
        /**
         * Create a new view of the map with given length and height.
         * @param length extension of mapview along x axis in pixels.
         * @param height extension of mapview along y axis in pixels.
         * @param renderer object that will perform the actual drawing to the screen.
         */
        mapview (const u_int32 & length, const u_int32 & height, const renderer_base * renderer = NULL);
        
        /**
         * Close mapview.
         */
        ~mapview ();
        
        /**
         * Reset mapview to initial state
         */
        void clear ();
        
        /**
         * @name Member access.
         */
        //@{
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
            if (CurZ != Pos.z())
            {
                if (std::abs (Pos.z() - CurZ) < std::abs (Speed)) CurZ = Pos.z();
                else CurZ += Speed;
            }
            
            if (Schedule)
            {
                return Schedule->execute (Args);
            }
            
            return true;
        }
        
        /**
         * Center on the given pixel coordinates. This is the preferred way to
         * set the mapview position from within the game, as it takes care of
         * situations where the map is smaller than the view or the view overlaps
         * the edge of the map.
         *
         * @param x position on the map in pixels
         * @param y position on the map in pixels
         */
        void center_on (const s_int32 & x, const s_int32 & y);
        
        /**
         * Set the position of the view to the given pixel coordinates. Does
         * nothing to improve the visuals, so mostly useful if you know what
         * you're doing.
         *
         * @param x position on the map in pixels
         * @param y position on the map in pixels
         */
        void set_position (const s_int32 & x, const s_int32 & y, const s_int32 & z)
        {
            Pos.set (x, y, z);
            CurZ = z;
            Sx = x;
            Sy = y - z;
            Ox = 0;
            Oy = 0;
        }
        
        /**
         * Get current position of the mapview. This is the center of the view.
         * @return the current position of the view.
         */
        const vector3<s_int32> get_position() const
        {
            return Pos;
        }

        /**
         * Resise the map view.
         * @param length the new length.
         * @param height the new height.
         */
        void resize (const u_int32 & length, const u_int32 & height)
        {
            set_length (length);
            set_height (height);
        }
        
        /**
         * Return the height the mapview is going to display. It might 
         * currently be in process of scrolling to that height.
         * @return height of the mapview.
         */
        s_int32 get_z () const
        {
            return Pos.z();
        }
        
        /**
         * Return the x coordinate of the map, from which the view starts.
         * @return x coordinate in map's space
         */
        s_int32 get_view_start_x () const
        {
            return Sx;
        }

        /**
         * Return the y coordinate of the map, from which the view starts.
         * @return y coordinate in map's space
         */
        s_int32 get_view_start_y () const
        {
            return Sy;
        }

        /**
         * Return the x offset of the rendered map within the view.
         * This is used when map is smaller than the view.
         * @return offset of the map in the view in x axis.
         */
        u_int16 get_view_offset_x () const
        {
            return Ox;
        }

        /**
         * Return the x offset of the rendered map within the view.
         * This is used when map is smaller than the view.
         * @return offset of the map in the view in x axis.
         */
        u_int16 get_view_offset_y () const
        {
            return Oy;
        }

        /**
         * Smooth transition of the mapview to the new height.
         * @param height the new height to transition to.
         * @param speed number of pixels to scroll during each update.
         */
        void scroll_to_z (const s_int32 & height, const u_int16 & speed)
        {
            Pos.set_z (height);
            Speed = speed * (CurZ > Pos.z() ? -1 : 1);
        }
        //@}
        
        /**
         * @name Rendering
         */
        //@{
        /**
         * Change the class that is used to do the actual drawing.
         * Passing NULL will restore the default renderer.
         * @param renderer the new renderer
         */
        void set_renderer (const renderer_base * renderer = NULL);
        
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
        
        /**
         * Limit rendering to objects not extending the given z value.
         * @param limit objects above this plane will not be rendered.
         */
        void limit_z (const s_int32 & limit);
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
         * @name Positioning script 
         */
        //@{
        /// the python method determining %area of map in view.
        python::method *Schedule;
        /// arguments passed to the %mapview schedule
        PyObject *Args;
        //@}
        
        /**
         * @name Position related members
         */
        //@{
        /// actual position of map view
        vector3<s_int32> Pos;
        /// the current height for smooth transitions on z axis
        s_int32 CurZ; 
        /// nbr of pixels to scroll per update
        s_int16 Speed;
        //@}
        
        /**
         * @name Rendering
         */
        //@{
        /// the class taking care of the actual drawing.
        const renderer_base* Renderer;
        
        /// fallback if no renderer is specified.
        static default_renderer DefaultRenderer;
        
        /// zone limiting rendering to a certain height.
        zone *RenderZone;
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

#endif
