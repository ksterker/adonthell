/*
   Copyright (C) 2011 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file 	gui/ui_event.h
 *
 * @author 	Kai Sterker
 * @brief 	Declares the ui_event class.
 */

#ifndef GUI_UI_EVENT_H
#define GUI_UI_EVENT_H

#include "event/event.h"
#include "gui/widget.h"

namespace gui
{
    /**
     * Defines the event fired whenever an action is performed
     * on a widget. This structure is used both when registering
     * listeners for certain actions of certain widgets and to
     * notify listeners that an action has been triggered.
     */
    class ui_event : public events::event
    {
    public:

        /**
         * Source for listeners that want to listen to events of a given type from any source.
         */
        static const int ANY_SOURCE = 0x1; // definitely not a valid pointer

        /**
         * @name Initialization
         */
        //@{
        /**
         * Standard constructor.
         */
        ui_event () : Source (NULL), Action (""), UserData (NULL)
        { }

        /**
         * Create a new ui %event.
         *
         * @param source the widget that generated the event.
         * @param action the action that triggered the event.
         * @param user_data optional data to pass back to the user when the event is triggered
         */
        ui_event (widget *source, const std::string & action, void *user_data = NULL);
        //@}
        
        /**
         * @name Event Handling
         */
        //@{
        /**
         * Compare two ui events for equality.
         *
         * @param e The ui event to compare this to.
         * @return <b>True</b> if the two events equal, <b>false</b> otherwise.
         */
        bool equals (const events::event* e) const
        {
            const ui_event *evt = (const ui_event *) e;
            return (Source == evt->source () || (uint64_t)Source == ANY_SOURCE) && Action == evt->action();
        }

#ifndef SWIG
        /**
         * Get name of %event.
         * @return the string "ui_event";
         */
        const char* name () const
        {
            return "ui_event";
        }
#endif // SWIG
        //@}

        /**
         * Return the widget that is the source of the event.
         * @return event source.
         */
        widget* source() const { return Source; }
        
        /**
         * Return the action that triggered the event.
         * @return event action.
         */
        const std::string action() const { return Action; }

        /**
         * Return the user specific data.
         * @return user data.
         */
        void* user_data() const { return UserData; }

#ifndef SWIG
        /**
         * Allow %event to be passed as python argument
         */
        GET_TYPE_NAME(gui::ui_event)
#endif

    private:
        /// the event source
        widget *Source;
        /// the event action
        std::string Action;
        /// user specific data
        void* UserData;
    };
}

#endif // UI_EVENT_H
