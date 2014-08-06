/*
 Copyright (C) 2014 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file gui/window.h
 *
 * @author Kai Sterker
 * @brief Base class for windows displayable by window manager.
 */

#ifndef GUI_WINDOW_H
#define GUI_WINDOW_H

#include <adonthell/gfx/screen.h>

namespace gui
{
    /**
     * Different fading styles that can be used when showing and hiding windows
     */
    enum fade_type
    {
        NONE,
        PLAIN,
        FADE,
        LEFT,
        RIGHT,
        BOTTOM,
        TOP
    };

    /**
     * Type of a window displayed by the window manager. Defines the initial
     * z-order of the window (above any existing windows of the same type) and
     * it's rendering logic.
     * Windows may be reordered within their type only, but i.e. a DIALOG 
     * window will never be obscured by a INTERFACE or WORLD_RELATIVE window, 
     * etc. 
     */
    enum window_type
    {
        /// a window that represents a view of the game world. All world-relative
        /// windows on a layer higher than the world view will be drawn "inside"
        /// this worldview. World views are the bottom-most layer.
        WORLD_VIEW = 0,
        /// a window that must be positioned relative to the world view. World
        /// relative windows reside above the WORLD_VIEW layer.
        WORLD_RELATIVE = 1,
        /// any permanent or temporarily present user interface elements that need
        /// not receive input focus. Above the WORLD_RELATIVE windows, but below
        /// normal dialog windows.
        INTERFACE = 2,
        /// all modal dialog windows. The topmost of those has the input focus.
        DIALOG = 3
    };
    
    /**
     * Default class for windows displayed by the window_manager.
     * Actual content of the window can depend on the implementation.
     */
    class window
    {
    public:
        window (gfx::drawable & content, const gfx::drawing_area & area, const window_type & type)
        {
            Content = &content;
            WindowType = type;
            FadeType = NONE;
            Showing = true;
            Area = area;
            Dx = 0;
            Dy = 0;
        }

        virtual ~window() { }
        
        /**
         * Get the display location of the window in X direction
         * @return window X coordinate offset by fading
         */
        s_int16 display_x() { return Dx + x(); }
        /**
         * Get the display location of the window in X direction
         * @return window Y coordinate offset by fading
         */
        s_int16 display_y() { return Dy + y(); }
        /**
         * Get the length of the window
         * @return window size in X direction
         */
        virtual u_int16 length() const { return Content->length(); }

        /**
         * Get the height of the window
         * @return window size in Y direction
         */
        virtual u_int16 height() const { return Content->height(); }
        
        /**
         * Actually draw the window to the screen
         *
         * @param x X position where to draw.
         * @param y Y position where to draw.
         * @param da_opt optional drawing_area to use during the drawing operation.
         * @param target pointer to the surface where to draw the drawable. If NULL, draw on the screen.
         */
        virtual void draw (const s_int16 & x = 0, const s_int16 & y = 0, const gfx::drawing_area *da_opt = NULL, gfx::surface *target = NULL)
        {
            gfx::drawing_area clip_rect (x + display_x(), y + display_y(), length(), height());
            if (da_opt != NULL)
            {
                clip_rect.assign_drawing_area(da_opt);
            }
            Content->draw (clip_rect.x(), clip_rect.y(), &clip_rect, target);
        }
        
        /**
         * Setup window to fade in.
         * @param fading the type of fading.
         */
        void fade_in(const fade_type & fading)
        {
            FadeType = fading;
            Showing = true;
        }
        /**
         * Setup window to fade out.
         * @param fading the type of fading.
         */
        void fade_out(const fade_type & fading)
        {
            // fading will actually cause the window to be removed, so NONE is not a valid option
            FadeType = fading == NONE ? PLAIN : fading;
            Showing = false;
        }
        /**
         * If fading is required.
         * @return true if fading is required, false otherwise.
         */
        bool fading() const { return FadeType != NONE; }
        /**
         * Do the actual fading.
         */
        virtual bool fade();
        
        /**
         * The type of window.
         * @return the type of this window.
         */
        window_type type() const { return WindowType; }
        
        /**
         * Let the window receive the input focus.
         */
        virtual void receive_focus();
        /**
         * Take input focus away from the window.
         */
        virtual void remove_focus();
        
        /**
         * Check two windows for equality.
         * @return true if they have the same content, false otherwise.
         */
        bool operator== (const window & w)
        {
            return Content == w.Content;
        }
        
    protected:
        /// element displayed by the window
        gfx::drawable *Content;
        
        /**
         * Get the position of the window in X direction
         * @return windows X coordinate
         */
        virtual s_int16 x() { return Area.x(); }

        /**
         * Get the position of the window in Y direction
         * @return windows Y coordinate
         */
        virtual s_int16 y() { return Area.y(); }
        
        /**
         * Whether this window can receive the input focus.
         * return true if the window is a DIALOG window.
         */
        bool can_focus() const { return WindowType == DIALOG; }
        
    private:
        /// window type, used for ordering of the windows
        window_type WindowType;
        /// the type of fading when showing or destroying the window
        fade_type FadeType;
        /// whether fading in or out
        bool Showing;
        /// position and size of the window
        gfx::drawing_area Area;
        /// current offset when fading from off-screen
        s_int16 Dx;
        /// current offset when fading from off-screen
        s_int16 Dy;
    };
}

#endif /* GUI_WINDOW */
