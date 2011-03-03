/*
 $Id: renderer.h,v 1.11 2009/04/08 21:52:10 ksterker Exp $
 
 Copyright (C) 2008/2009 Kai Sterker <kaisterker@linuxgames.com>
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

#include "world/chunk_info.h"
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
     * Destructor.
     */
    virtual ~renderer_base() { };
    
    /**
     * Draw objects in the draw queue on screen.
     * @param x offset on the x-axis.
     * @param y offset on the y-axis.
     * @param objectlist list of objects to draw on screen.
     * @param da clipping rectangle.
     * @param target surface to draw on, NULL for screen surface.
     */
    virtual void render (const s_int16 & x, const s_int16 & y, const std::list <world::chunk_info*> & objectlist, const gfx::drawing_area & da, gfx::surface * target) const = 0;
    
    /**
     * Draw objects in the given list on screen.
     * @param x offset on the x-axis.
     * @param y offset on the y-axis.
     * @param render_queue list of objects to draw on screen.
     * @param da clipping rectangle.
     * @param target surface to draw on, NULL for screen surface.
     */
    virtual void render (const s_int16 & x, const s_int16 & y, std::list <world::render_info> & render_queue, const gfx::drawing_area & da, gfx::surface * target) const = 0;    
    
protected:
    /**
     * Draw a single object to the screen.
     * @param x offset on the x-axis.
     * @param y offset on the y-axis.
     * @param obj the object to draw on screen.
     * @param da clipping rectangle.
     * @param target surface to draw on, NULL for screen surface.
     */
    virtual void draw (const s_int16 & x, const s_int16 & y, const render_info & obj, const gfx::drawing_area & da, gfx::surface * target) const;
    
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
     * Destructor.
     */
    virtual ~default_renderer() { };
        
    /**
     * Draw objects in the draw queue on screen.
     * @param x offset on the x-axis.
     * @param y offset on the y-axis.
     * @param objectlist list of objects to draw on screen.
     * @param da clipping rectangle.
     * @param target surface to draw on, NULL for screen surface.
     */
    virtual void render (const s_int16 & x, const s_int16 & y, const std::list <world::chunk_info*> & objectlist, const gfx::drawing_area & da, gfx::surface * target) const;

    /**
     * Draw objects in the given list on screen.
     * @param x offset on the x-axis.
     * @param y offset on the y-axis.
     * @param render_queue list of objects to draw on screen.
     * @param da clipping rectangle.
     * @param target surface to draw on, NULL for screen surface.
     */
    virtual void render (const s_int16 & x, const s_int16 & y, std::list <world::render_info> & render_queue, const gfx::drawing_area & da, gfx::surface * target) const;
    
#ifndef SWIG
    /**
     * Allow %default_renderer to be passed as python argument
     */
    GET_TYPE_NAME_VIRTUAL (world::default_renderer)
#endif
        
protected:
    /// short for a render-queue-iterator
    typedef std::list <world::render_info>::iterator iterator;
    /// short for a render-queue backwards iterator
    typedef std::list <world::render_info>::reverse_iterator reverse_iterator;
    /// short for a const render-queue-iterator
    typedef std::list <world::render_info>::const_iterator const_iterator;
    
    /**
     * Check if an object overlaps any other object in the view.
     * @param obj object to check.
     * @param begin begin of list of objects to check against
     * @param end end of list of objects to check against
     * @return whether to draw, remove or skip object.
     */
    u_int32 can_draw_object (render_info & obj, const_iterator & begin, const_iterator & end) const;
    
    /**
     * Check if obj1 is below obj2 in the view.
     * @param obj1 an object
     * @param obj2 another object
     * @return true if obj1 is located beneath obj2 in the view, false otherwise.
     */
    bool is_object_below (const render_info & obj1, const  render_info & obj2) const;
    
private:
    void visualize_deadlock (std::list <world::render_info> & render_queue) const;
};

/**
 * A renderer with various debugging functionalities.
 */
class debug_renderer : public default_renderer
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
     * Destructor.
     */
    virtual ~debug_renderer() { };
        
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
     * @param prnt whether to print drawing queue.
     */
    void print_queue (const bool & prnt) { Print = prnt; }
    
#ifndef SWIG
    /**
     * Allow %debug_renderer to be passed as python argument
     */
    GET_TYPE_NAME_VIRTUAL (world::debug_renderer)
#endif
    
protected:
    /**
     * Draw a single object to the screen.
     * @param x offset on the x-axis.
     * @param y offset on the y-axis.
     * @param obj the object to draw on screen.
     * @param da clipping rectangle.
     * @param target surface to draw on, NULL for screen surface.
     */
    virtual void draw (const s_int16 & x, const s_int16 & y, const render_info & obj, const gfx::drawing_area & da, gfx::surface * target) const;
    
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
