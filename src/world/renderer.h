/*
 $Id: renderer.h,v 1.1 2008/10/18 12:41:18 ksterker Exp $
 
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
 * @file   world/renderer.h
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Defines classes for rendering the mapview on screen.
 * 
 */

#ifndef WORLD_RENDERER_H
#define WORLD_RENDERER_H

#include <list>

#include "gfx/surface.h"
#include "world/render_info.h"

namespace world
{
    
/**
 * Interface that all renderers must implement.
 */
class renderer_base
{
public:
    /**
     * Draw objects in the draw queue on screen.
     * @param x offset on the x-axis.
     * @param y offset on the y-axis.
     * @param queue list of objects to draw on screen.
     * @param da clipping rectangle.
     * @param target surface to draw on, NULL for screen surface.
     */
    virtual void render (const s_int16 & x, const s_int16 & y, const std::list <world::render_info> & queue, const gfx::drawing_area & da, gfx::surface * target) const = 0;

#ifndef SWIG
    /**
     * Allow %renderer_base to be passed as python argument
     */
    GET_TYPE_NAME_ABSTRACT (world::renderer_base)
#endif
};

/**
 * A simple renderer that draws all objects in the draw queue
 * without any special effects.
 */
class default_renderer : public renderer_base
{
public:
    /**
     * Draw objects in the draw queue on screen.
     * @param x offset on the x-axis.
     * @param y offset on the y-axis.
     * @param queue list of objects to draw on screen.
     * @param da clipping rectangle.
     * @param target surface to draw on, NULL for screen surface.
     */
    virtual void render (const s_int16 & x, const s_int16 & y, const std::list <world::render_info> & queue, const gfx::drawing_area & da, gfx::surface * target) const;
    
#ifndef SWIG
    /**
     * Allow %renderer_base to be passed as python argument
     */
    GET_TYPE_NAME_VIRTUAL (world::default_renderer)
#endif        
};

/**
 * A renderer with various debugging functionalities.
 */
class debug_renderer : public renderer_base
{
public:
    /**
     * Create a new debug renderer.
     * @param draw_bbox whether to draw object bounding boxes
     * @param delay pause after each object has been rendered
     */
    debug_renderer (const bool & draw_bbox = false, const u_int32 & delay = 0) 
     : DrawBBox (draw_bbox), Print (false), Delay (delay) 
    { }
    
    /**
     * Draw objects in the draw queue on screen.
     * @param x offset on the x-axis.
     * @param y offset on the y-axis.
     * @param queue list of objects to draw on screen.
     * @param da clipping rectangle.
     * @param target surface to draw on, NULL for screen surface.
     */
    virtual void render (const s_int16 & x, const s_int16 & y, const std::list <world::render_info> & queue, const gfx::drawing_area & da, gfx::surface * target) const;
    
    /**
     * Set Delay between drawing objects.
     * @param delay pause after each object has been rendered
     */
    void set_delay (const s_int32 & delay) { Delay = delay; }
    
    /**
     * Toggle drawing of bounding box.
     * @param draw_bbox whether to draw object bounding boxes
     */
    void set_draw_bbox (const bool & draw_bbox) { DrawBBox = draw_bbox; }

    /**
     * Toogle printing of drawing queue content to stdout.
     * @param print whether to print drawing queue.
     */
    void print_queue (const bool & print) { Print = print; }
    
#ifndef SWIG
    /**
     * Allow %renderer_base to be passed as python argument
     */
    GET_TYPE_NAME_VIRTUAL (world::debug_renderer)
#endif
    
private:
    /// whether to draw object bounding boxes
    bool DrawBBox;
    /// whether to print contents of queue to stdout
    bool Print;
    /// pause after each object has been rendered
    u_int32 Delay;
};

}

#endif // WORLD_RENDERER_H
