/*
   Copyright (C) 2012 Kai Sterker <kai.sterker@gmail.com>
   Part of the Adonthell Project http://adonthell.nongnu.org

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
 * @file 	world/move_event.h
 *
 * @author 	Kai Sterker
 * @brief 	Declares the move_event class.
 */

#ifndef MOVE_EVENT_H
#define MOVE_EVENT_H

#include "event/event.h"
#include "world/moving.h"
#include "world/zone.h"

namespace world
{
    /**
     * The %move %event executes the callback attached to a listener whenever
     * the following conditions are met:
     *
	 * - A move on the given map occurs
     * - Any character enters the given zone
     * - Any character exits the given zone.
     * - Any character enters or exits the given zone.
	 * - The given character moves on the given map
	 * - The given character enters the given zone
	 * - The given character exits the given zone
     * - The given character enters or exits the given zone.
     *
     * @todo: properly implement support for multiple maps. That means we need
     * to keep track of the map, so the event can be restored properly.
     */
    class move_event : public events::event
    {
    public:
        /**
         * @name Initialization
         */
        //@{
        /**
         * Constructor used to register a listener for moves.
         *
         * If all parameters are NULL, registered callback will trigger on any move
         * that occurs. Usually you'll want to limit this to a certain character or
         * entering or exiting a certain area on the map. If character and one area
         * is specified, the callback is only triggered when that character enters or
         * leaves the given area. It's not permitted to specify different areas.
         *
         * @param actor (optional) character whose moves trigger the callback.
         * @param check_enter (optional) zone that triggers the callback when entered.
         * @param check_leave (optional) zone that triggers the callback when exited.
         */
        move_event (const world::area *map, const std::string & actor = "", const std::string & check_enter = "", const std::string & check_leave = "");
		
#ifndef SWIG
        /**
         * Constructor used by event system.
         */
        move_event () : Actor (NULL), CheckEnter (NULL), CheckLeave (NULL)
        {
        }

        /**
         * Constructor used to fire a move event.
         *
         * @param the actor performing the move, while still at its starting
         * position.
         */
        move_event (const world::moving *actor)
        {
            Actor = actor;
            Start = world::vector3<s_int32>(*actor);
        }
#endif // SWIG
        //@}

        /**
         * @name Event Handling
         */
        //@{
        /**
         * Compare two %move events for equality.
         *
         * @param e The %move %event to compare this to.
         * @return <b>True</b> if the two events match, <b>false</b> otherwise.
         */
        bool equals (const events::event * e) const;
        //@}

        /**
         * @name Member access
         */
        //@{
        /**
         * Get actor that is moving.
         * @return actor that is moving.
         */
        const world::moving *actor() const { return Actor; }

        /**
         * Get zone that was entered or left, if any.
         * @return zone that was entered or left.
         */
        const world::zone *zone () const { return CheckEnter ? CheckEnter : CheckLeave; }

#ifndef SWIG
        /**
         * Get name of %event.
         * @return the string "move_event";
         */
        const char* name () const
        {
            return "move_event";
        }

        /**
         * Get start position of movement.
         * @return start position of movement.
         */
        world::vector3<s_int32> start () const { return Start; }

        /**
         * Get end position of movement.
         * @return end position of movement.
         */
        world::vector3<s_int32> end () const { return *Actor; }
#endif // SWIG
        //@}
        
        /**
         * @name Loading / Saving
         */
        //@{
        
        /** 
         * Saves the %event %data to a stream.
         * 
         * @param file stream where to save the %event.
         */ 
        void put_state (base::flat& file) const;
        
        /** 
         * Loads the %event from a stream.
         * 
         * @param file stream to load the %event from.
         * @return \e true if the %event could be loaded, \e false otherwise
         */
        bool get_state (base::flat& file);
        //@}
    
#ifndef SWIG
        /**
         * Allow %event to be passed as python argument
         */
        GET_TYPE_NAME(world::move_event)
#endif

    private:
        /// object performing the move (optional when querying)
        const world::moving *Actor;
        /// Start position of the move
        world::vector3<s_int32> Start;

        /// Event triggers when move enters given zone (optional)
        const world::zone *CheckEnter;
        /// Event triggers when move leaves given zone (optional)
        const world::zone *CheckLeave;
    };
}

#endif // MOVE_EVENT_H
