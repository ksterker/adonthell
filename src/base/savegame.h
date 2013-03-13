/*
 Copyright (C) 2009 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file   base/savegame.h
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Savegame management.
 * 
 * 
 */

#ifndef BASE_SAVEGAME_H
#define BASE_SAVEGAME_H

#define SPECIAL_SLOT_COUNT 3

#include <ctime>
#include <vector>
#include <list>
#include <string>

#include "types.h"
#include "serializer.h"

namespace base
{
    /**
     * Savegame meta data.
     */
    class savegame_data
    {
    public:
        /**
         * Create save game data.
         *
         * @param dir the saved game directory.
         * @param desc description of the saved game.
         * @param time in-game time when saving the game.
         */
        savegame_data (const std::string & dir, const std::string & desc, const u_int32 & time);
        
        /**
         * @name Member access.
         */
        //@{
        /**
         * Get the directory the game is saved in.
         * @return the savegame's directory.
         */
        std::string directory () const { return Directory; }
        
        /**
         * Get the description of the save game.
         * @return the savegame's description.
         */
        std::string description () const { return Description; }
        
        /**
         * Get the in-game time at the point where the game was saved.
         * @return the in-game timestamp.
         */
        u_int32 gametime () const { return Gametime; }
        
        /**
         * Get time when the saved game was created/last saved.
         * @return the real-world timestamp.
         */
        time_t last_modified () const { return Timestamp; }
        //@}
    
        /**
         * Update existing data.
         * @param desc description of the saved game.
         * @param time in-game time when saving the game.
         */
        void update (const std::string & desc, const u_int32 & time);

        /**
         * Set time when the saved game was created/last saved.
         * @param t the timestamp.
         */
        void set_last_modified (const time_t & t ) { Timestamp = t; }
        
        /**
         * Order saved games by creation date.
         * @param s the game to compare to this one.
         * @return True if this is newer than s.
         */
        bool operator < (const savegame_data & s) const
        {
            return Timestamp < s.Timestamp;
        }
        
    private:
        /// time when the game was created
        time_t Timestamp;
        /// time in game
        s_int32 Gametime;
        /// directory where the game resides
        std::string Directory;
        /// a description entered by the user
        std::string Description;
    };
    
    /**
     * This class is used to save or load the players progress through
     * the game. For that purpose it provides two distinct functionalities:
     *
     * \li - Handling of the physical saved game directories. 
     * \li - A common place to register serializers, classes that know how
     *       to write and read game date.
     *
     * Apart from user created saved games, there are 4 special game slots:
     * \li - new save: This slot can be used to create a fresh saved game.
     * \li - initial save: This read only slot points to the game data as
     *       it exists at the very beginning of a game.
     * \li - auto save: This slot can be used to automatically save the users
     *       progress at key points of the game. 
     * \li - quick save: This slot can be used to save/load without bringing
     *       up a GUI.
     */
    class savegame
    {
    public:
        /// Special slots.
        enum
        {
            NEW_SAVE = -4,
            INITIAL_SAVE = -3,
            AUTO_SAVE = -2,
            QUICK_SAVE = -1
        };
        
        /**
         * Create an instance for loading or saving the game, with 
         * a callback to get notified about overall progress. 
         * The callback will be executed for each registered 
         * serializer. It's argument will be set to a number 
         * between 0 and 100.
         *
         * @param callback the callback to set.
         */        
        savegame (base::functor_1<const s_int32> *callback = NULL);
        
        /**
         * Destructor.
         */
        ~savegame ();
        
        /**
         * @name Accessing Saved Games
         */
        //@{
        /**
         * Get the number of user-generated saved games,
         * excluding any special slots.
         *
         * @return number of saved games.
         */
        u_int32 count () const
        {
            return Games().size() - SPECIAL_SLOT_COUNT;
        }
        
        /**
         * Get information about saved game in the given
         * slot. Should be either a named special slot or
         * a slot in the range [0, count()[.
         *
         * @param slot index of saved game slot.
         */
        savegame_data *get (const s_int32 & slot);
        //@}
    
        /**
         * @name Loading / Saving
         */
        //@{
        /**
         * Load the game at the given slot.
         *
         * @param slot index of saved game slot.
         * @return true on success, false otherwise.
         */
        bool load (const s_int32 & slot);
        
        /**
         * Save the game at the given slot. Note that 
         * it is not allowed to overwrite INITIAL_SAVE.
         * Pass NEW_SAVE to write to an unused slot.
         *
         * @param slot index of saved game slot.
         * @param desc user supplied description of the game.
         * @param gametime in-game timestamp.
         * @return true on success, false otherwise.
         */
        bool save (const s_int32 & slot, const std::string & desc, const u_int32 & gametime);
        //@}
        
        /**
         * Load information about existing saved games for
         * the current game.
         */
        static void init ();

        /**
         * Shutdown savegame system and free all static resources..
         */
        static void cleanup();

        /**
         * Register a serializer for saving/loading game data.
         * @param serializer the serializer to add.
         */
        static void add (base::serializer_base* serializer);
        
        /**
         * Remove an existing serializer.
         * @param serializer the serializer to remove.
         */
        static void remove (base::serializer_base* serializer);
        
        /**
         * Return the path to the current slot the game is
         * running from. The slot is updated every time a game 
         * has been loaded or saved successfully.
         *
         * @return path to current saved game data directory.
         */
        static std::string current_path ();
        
    protected:
        /**
         * Delete all regular files in the given directory.
         * @param name the directory name.
         */
        void cleanup (const std::string & name);
            
        /**
         * Save meta information for the given saved gaem.
         * @param data the game data structure to save.
         * @return true on success, false otherwise.
         */
        bool save_meta_data (savegame_data *data);
        
        /**
         * Load meta information of given saved game. If
         * successful, the data is added to the list of known
         * saved games.
         *
         * @param filepath full path to (potential) saved game directory.
         * @return true on success, false otherwise.
         */
        static bool load_meta_data (const std::string & filepath);
        
    private:
        /// the slot the current game is running from
        static s_int32 CurrentSlot;
        /// list of available saved games
        static std::vector<savegame_data*>& Games();
        /// classes that read write game data
        static std::list<base::serializer_base*>& Serializer();
        /// notify about load/save progress
        base::functor_1<const s_int32> *ProgressCallback;
    };
}

#endif
