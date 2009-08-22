/*
 $Id: character.h,v 1.12 2009/05/03 16:26:00 ksterker Exp $

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
 * @file   world/character.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 *
 * @brief  Declares the character class.
 *
 *
 */

#ifndef WORLD_CHARACTER_H
#define WORLD_CHARACTER_H

#include "base/diskio.h"
#include "world/moving.h"
#include "world/schedule.h"

namespace rpg
{
    class character;
}

namespace world
{

    /**
     * Map representation of a character.
     *
     * This class implements all that you need to represent a character
     * on the map. It mostly defines some additionnal functions on moving
     * to set the correct representation according to the character movement
     * on the map, and limits its speed.
     *
     * @note methods that need to access rpg::character can't be implemented
     *       in this file, and should therefore be implemented in the .cc file
     */
    class character : public moving
    {
    public:
        typedef enum { NONE = 0, WEST = 1, EAST = 2, NORTH = 4, SOUTH = 8 } direction;

        /**
         * Create a new character on given map.
         * @param mymap the place the character lives.
         * @param mind representation of this character on the rpg side.
         */
        character (area & mymap, rpg::character * mind = NULL);

        /**
         * Free character and associated resources.
         */
        virtual ~character ();

        /**
         * Call every cycle to "process" the %character. This
         * takes care of the %character's physics, like falling
         * or jumping.
         */
        virtual bool update ();

        /**
         * Update %character state. This takes care of the
         * character's movement state -- like whether he's
         * running, walking or standing still -- and direction
         * he is facing.
         */
        void update_state ();

        /**
         * @name Character Movement
         */
        //@{
        /**
         * Set base speed of the character. Running will double the base speed.
         * @param speed this %character's base speed.
         */
        void set_speed (float speed)
        {
            Speed = speed;
        }

        /**
         * Get base speed of %character.
         * @return the %character's base speed.
         */
        float base_speed () const
        {
            return Speed;
        }

        /**
         * Get speed of %character.
         * Varies depending on the terrain he is walking over.
         * @return the %character's actual speed
         * @todo maybe remember the type of terrain the character was over in
         *       the last iteration, to increase performance when walking over
         *       the same terrain. As of now this function is called every frame
         *       a character is moving/jumping, so any improvement is a big improvement.
         */
        float speed () const;

        /**
         * Toggle running off.
         */
        void walk()
        {
    		ToggleRunning = false;
        	if (VSpeed == 0.0)
        	{
            	IsRunning = false;
            	set_direction (current_dir());
        	}
        }

        /**
         * Toggle running on.
         */
        void run()
        {
    		ToggleRunning = true;
        	if (VSpeed == 0.0)
        	{
	            IsRunning = true;
    	        set_direction (current_dir());
        	}
        }

        /**
         * Check if %character is currently running.
         * @return true if the %character is running, false otherwise.
         */
        bool is_running() const
        {
            return IsRunning;
        }

        /**
         * Let character jump by setting a vertical speed != 0.
         */
        void jump();

        /**
         * Halt the %character. Vertical movement is not affected.
         */
        void stop()
        {
            set_velocity(0.0, 0.0);
        }

        /**
         * Return schedule of this character. This is the high-level
         * script that controls the characters behaviour.
         *
         * @return the character's schedule.
         */
        world::schedule* get_schedule ()
        {
            return &Schedule;
        }

        /**
         * Returns the representation of this character in the rpg side.
         * @return rpg::character that represents this world::character.
         */
        rpg::character * mind() const
        {
            return Mind;
        }

       /**
        * Set the rpg side representation
        * @param mind the rpg side representation
        */
        void set_mind(rpg::character * mind)
        {
            Mind = mind;
        }

        //@}

        /**
         * @name Character Orientation
         */
        //@{
        /**
         * Get the character's orientation. Return the direction
         * the character is currently facing, regardless of movement.
         * @return character orientation.
         */
        s_int32 heading() const
        {
            return Heading;
        }

        /**
         * Get current direction(s) the %character is moving. Returns
         * a direction or combination of directions indicating the
         * current movement direction. Returns 0 if the character is
         * standing still.
         * @return direction of movement, 0 if character is not moving.
         */
        s_int32 current_dir() const
        {
            return CurrentDir;
        }

        /**
         * Set the %character's current direction.
         * @param flag indicating character's orientation.
         */
        void set_direction(const s_int32 & ndir);

        /**
         * Change the %character's orientation by given direction.
         * @param one of NORTH, EAST, SOUTH or WEST.
         */
        void add_direction(direction ndir);

        /**
         * Change the %character's orientation by no longer facing given direction.
         * @param one of NORTH, EAST, SOUTH or WEST.
         */
        void remove_direction(direction ndir)
        {
            set_direction(current_dir() & ~ndir);
        }
        //@}

        /**
         * Loading / Saving
         */
        //@{
        /**
         * Save %character state to stream.
         * @param file stream to save %character to.
         * @return \b true if saving successful, \b false otherwise.
         */
        bool put_state (base::flat & file) const;

        /**
         * Load %character state from stream.
         * @param file stream to load %character from.
         * @return \b true if loading successful, \b false otherwise.
         */
        bool get_state (base::flat & file);

        /**
         * Save character state to file.
         * @param fname file name.
         * @param format type of file to create.
         * @return true on success, false otherwise.
         */
        bool save (const std::string & fname, const base::diskio::file_format & format = base::diskio::BY_EXTENSION) const;

        /**
         * Load character state from file.
         * @param fname file name.
         * @return true on success, false otherwise.
         */
        bool load (const std::string & fname);
        //@}

#ifndef SWIG
        /**
         * Allow %character to be passed as python argument
         */
        GET_TYPE_NAME_VIRTUAL (world::character)
#endif

    protected:
        /**
         * Update velocity based on current terrain.
         * @param ndir direction(s) the character is moving in.
         */
        void update_velocity (const s_int32 & ndir);
            
        /// horizontal speed for walking / running
        float Speed;
        /// vertical speed for jumping
        float VSpeed;

        /// whether character is running or not
        bool IsRunning;
        /// whether character should be running once on the ground
        bool ToggleRunning;
        /// current direction the character is moving
        s_int32 CurrentDir;
        /// direction the character is facing
        s_int32 Heading;

    private:
        /// forbid passing by value
        character (const character & p);

        /// Schedule assigned to this character
        schedule Schedule;

        /// RPG side representation of this character
        rpg::character * Mind;
    };
}

#endif
