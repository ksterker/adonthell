/*
 $Id: character.h,v 1.2 2007/05/21 04:44:11 ksterker Exp $
 
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

#include "world/moving.h"

namespace world
{
    /**
     * Map representation of a character.
     *
     * This class implements all that you need to represent a character
     * on the map. It mostly defines some additionnal functions on moving
     * to set the correct state according to the character move, and limits 
     * it's speed.
     * 
     */
    class character : public moving
    {
    public:
        typedef enum { NONE = 0, WEST = 1, EAST = 2, NORTH = 4, SOUTH = 8 } direction;

        /**
         * Create a new character on given map.
         * @param mymap the place the character lives.
         */
        character (area & mymap);

        /**
         * Call every cycle to "process" the %character.
         */
        void update ();

        /**
         * Update %character state.
         */
        void update_state ();

        /**
         * @name Character Movement
         */
        //@{
        /**
         * Set base speed of the character. Running will double the base speed.
         * @pararm speed this %character's base speed.
         */
        void set_speed (float speed) 
        {
            Speed = speed;
        }
        
        /**
         * Get base speed of %character.
         * @return the %character's base speed.
         */
        float speed () 
        {
            return Speed; 
        }
        
        /**
         * Toggle running off.
         */
        void walk()
        {
            Is_running = false;
            set_direction (current_dir());
        }

        /**
         * Toggle running on.
         */
        void run()
        {
            Is_running = true;
            set_direction (current_dir());
        }

        /**
         * Check if %character is currently running.
         * @return true if the %character is running, false otherwise.
         */
        bool is_running()
        {
            return Is_running;
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
        //@}

        /**
         * @name Character Orientation
         */
        //@{
        /**
         * Get current direction(s) the %character is facing.
         * @return a flag indication orientation.
         */
        int current_dir()
        {
            return Current_dir;
        }

        /**
         * Set the %character's current direction.
         * @param flag indicating character's orientation.
         */
        void set_direction(int ndir);

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
         * @return true on success, false otherwise.
         */
        bool save (const std::string & fname) const;
        
        /**
         * Load character state from file.
         * @param fname file name.
         * @return true on success, false otherwise.
         */
        bool load (const std::string & fname);
        //@}
        
    protected:
        /// horizontal speed for walking / running
        float Speed; 
        /// vertical speed for jumping
        float VSpeed;
        
        /// whether character is running or not
        bool Is_running;
        /// current direction the character is facing
        int Current_dir;
    };
}

#endif
