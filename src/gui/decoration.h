/*
 Copyright (C) 2010 Kai Sterker <kai.sterker@gmail.com> 
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
 * @file gui/decoration.h
 * @author Kai Sterker <kai.sterker@gmail.com>
 * @brief  Declares the gui decoration class.
 */

#ifndef GUI_DECORATION_H
#define GUI_DECORATION_H

#include <vector>

#include "base/hash_map.h"
#include "base/flat.h"

namespace gfx
{
    class surface;
    class drawing_area;
}

namespace gui
{

/**
 * Resources for the UI decoration of a widget
 * in a given state.
 */
class decoration_info 
{
public:
    /**
     * Create an empty decoration for a certain state.
     */
    decoration_info () : Length(0), Height(0), Cache(NULL)
    {
    }

    /**
     * Destroy decoration state.
     */
    ~decoration_info ();
    
    /**
     * Initialize from the given record.
     */
    bool init (base::flat & record);
    
    /**
     * Set the size of the decoration. If the size differs
     * from the current size, it will invalidate the cache.
     * The decoration will be recreated the next time it is
     * requested with get_surface().
     * @param length the new length.
     * @param height the new height.
     */
    void set_size (const u_int16 & length, const u_int16 & height);

    /**
     * Get current length.
     * @return the length.
     */
    u_int16 length () const { return Length; }
    
    /**
     * Get current height.
     * @return the height.
     */    
    u_int16 height () const { return Height; }
    
    /**
     * Get the decoration image. If not cached yet, it
     * will be created first.
     * @return pointer to the rendered decoration.
     */
    gfx::surface *get_surface ();

private:
    /**
     * Add an element to the list of decoration elements.
     * @param record the data file.
     * @param id the name in the configuration file.
     */
    bool add_element (base::flat & record, const std::string &id);
        
    /// the length of the decoration
    u_int16 Length;
    /// the height of the decoration
    u_int16 Height;

    /// the different elements as stored in the elements vector
    enum { BACKGROUND, 
           BORDER_TOP_LEFT, BORDER_BOTTOM_LEFT, BORDER_TOP_RIGHT, BORDER_BOTTOM_RIGHT, 
           BORDER_LEFT, BORDER_RIGHT, BORDER_TOP, BORDER_BOTTOM };
    
    /// the elements of the decoration
    std::vector<const gfx::surface*> Elements;
    /// the rendered decoration
    gfx::surface *Cache;
};
    
/**
 * GUI decoration, consisting of a background and an
 * optional border. These elements are available in
 * different variations to represent the different
 * states of the user interface element that is decorated,
 * such as default, activated, disabled. It also provides
 * an overlay for when the element has the input focus.
 */
class decoration
{
public:
    /**
     * Create a new UI decorator.
     */
    decoration()
    {
        CurrentState = Decoration.end();
        FocusOverlay = Decoration.end();
    }
    
    /**
     * Destroy UI decorator;
     */
    ~decoration()
    {
        cleanup();
    }

    /**
     * Initialize the decorator to the given decoration.
     * This will search a file with the given name and
     * load the graphical resources associated with the
     * decoration.
     * @param name decoration file name.
     * @return true on success, false otherwise.
     */
    bool init (const std::string & name);
    
    /**
     * Prepare the decoration for the given size. If
     * omitted, the decoration will be sized to the
     * proportions of the background image. Otherwise
     * the background will be tiled to the given size.
     * @param length the length.
     * @param height the height.
     */
    void set_size (const u_int16 & length, const u_int16 & height);
    
    /**
     * Get size of the decoration's "Default" state.
     * @param x parameter to store the length
     * @param y parameter to store the height
     * @return true on success, false otherwise.
     */
    bool get_size (u_int16 &x, u_int16 &y) const
    {
        decoration_map::const_iterator def = Decoration.find ("Default");
        if (def != Decoration.end())
        {
            x = def->second->length ();
            y = def->second->height ();
            return true;
        }
        
        return false;
    }
    
    /**
     * Set state of decoration. Supported states are "Default", 
     * "Activated" and "Disabled" (and anything else defined in the
     * appropriated decoration data file).
     * @param state name of the current state.
     * @param has_focus whether the widget currently has the focus. 
     */
    void set_state (const std::string & state = "Default", const bool & has_focus = false);

    /**
     * Draw the given decoration at the given offset 
     * with the given clipping rectangle to the given
     * target surface.
     * @param x offset in x direction.
     * @param y offset in y direction.
     * @param da clipping rectangle.
     * @param target target surface.
     */
    void draw (const s_int16 & x, const s_int16 & y, const gfx::drawing_area * da, gfx::surface * target) const;
    
protected:
    /**
     * Reset to initial state.
     */
    void cleanup ();

private:
    typedef std::hash_map<std::string, decoration_info*> decoration_map;
    /// pointer to focus overlay
    decoration_map::const_iterator FocusOverlay;
    /// pointer to current state
    decoration_map::const_iterator CurrentState;
    /// decoration by state
    decoration_map Decoration;
};

}

#endif

