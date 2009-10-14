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
 * @file   base/savegame.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Savegame management implementation.
 * 
 * 
 */

#include <algorithm>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

#include "base/savegame.h"
#include "base/base.h"
#include "base/diskio.h"

using base::savegame;
using base::savegame_data;

#ifdef WIN32
/// mingw dirent struct does not have d_type
static int get_file_type (const std::string & path, const std::string & name)
{
    struct stat statbuf;
    std::string file = path + "/" + name;
    if (stat (file.c_str (), &statbuf) != -1)
    {
        return statbuf.st_mode;
    } 
    return 0;
}
#endif


/// list of saved games
std::vector<savegame_data*> savegame::Games;

// ctor
savegame_data::savegame_data (const std::string & dir, const std::string & desc, const u_int32 & time)
{
    Timestamp = 0;
    Directory = dir;
    Description = desc;
    Gametime = time;    
}

// update existing data
void savegame_data::update (const std::string & desc, const u_int32 & time)
{
    Description = desc;
    Gametime = time;    
}

// ctor
savegame::savegame (base::functor_1<const s_int32> *callback)
{
    ProgressCallback = callback;
}

// dtor
savegame::~savegame ()
{
    delete ProgressCallback;
}

// load a saved game
bool savegame::load (const s_int32 & slot)
{
    savegame_data *data = get (slot);
    if (data == NULL) return false;
    
    u_int32 current = 1;
    u_int32 count = Serializer().size();
    
    // set the directory we're loading from
    base::Paths.set_save_dir (data->directory());
    
    // load
    std::list<base::serializer_base*>::iterator i;
    for (i = Serializer().begin(); i != Serializer().end(); i++)
    {
        if (!(*i)->load())
        {
            return false;
        }
        
        if (ProgressCallback != NULL)
        {
            (*ProgressCallback)((s_int32)((current * 100.0) / count));
        }

        current++;
    }
    
    return true;
}

// save the game
bool savegame::save (const s_int32 & slot, const std::string & desc, const u_int32 & gametime)
{
    if (slot == INITIAL_SAVE) return false;

    savegame_data *data = get (slot);
    if (data == NULL)
    {
        std::string filepath;
        u_int32 pos = 0;
        char t[10];
        
        // make sure we save to an unused directory
        while (pos <= 1000)
        {
            // that's the directory we're going to save to
            sprintf(t, "%03i", pos++);
            filepath = base::Paths.cfg_data_dir();
            filepath += "/" + base::Paths.game() + "-save-";
            filepath += t;
            
            DIR *dir = opendir (filepath.c_str ());
            if (dir != NULL)
            {
                closedir (dir);
                continue;
            }
            
            // found an unused directory
            break;
        }
        
        if (pos == 1000)
        {
            fprintf (stderr, "*** savegame::save: seems like you have no write permission in\n    %s\n",
                     base::Paths.cfg_data_dir().c_str ());
            return false;
        }

        // we'll need a new gamedata record
        data = new savegame_data (filepath, desc, gametime);
        Games.push_back (data);
    }
    else
    {
        // we have to update the gamedata record
        data->update (desc, gametime);
    }

    // prepare the directory before saving
    cleanup (data->directory());

    // save game data
    u_int32 current = 1;
    u_int32 size = Serializer().size();
    std::list<base::serializer_base*>::iterator i;
    for (i = Serializer().begin(); i != Serializer().end(); i++)
    {
        if (!(*i)->save (data->directory()))
        {
            cleanup (data->directory());
            return false;
        }
        
        if (ProgressCallback != NULL)
        {
            (*ProgressCallback)((s_int32)((current * 100.0) / size));
        }
        
        current++;
    }
    
    // finally save meta data, making the saved game valid
    save_meta_data (data);

    // update timestamp ...
    data->set_last_modified (time (NULL));
    
    // ... and re-sort
    std::sort (Games.begin()+SPECIAL_SLOT_COUNT, Games.end());

    return true;
}

// read available games
void savegame::init (const std::string & name)
{
    struct dirent *dirent;
    DIR *dir;

    // create initial saved game
    Games.push_back (new savegame_data ("", "Start New Game", 0));
    
    // create auto save slot
    std::string save_dir = base::Paths.cfg_data_dir() + "/" + name;
    if (!load_meta_data (save_dir + "-auto-save"))
    {
        Games.push_back (new savegame_data (save_dir + "-auto-save", "Autosave", 0));
    }
    
    // create quick save slot
    if (!load_meta_data (save_dir + "-quick-save"))
    {
        Games.push_back (new savegame_data (save_dir + "-quick-save", "Quicksave", 0));
    }
    
    // Read the user's saved games (if any) - they'll be located in
    // the configuration directory and called <gamename>-save-<xxx>
    if ((dir = opendir (base::Paths.cfg_data_dir().c_str ())) != NULL)
    {
        std::string name_save = name + "-save-";
        
        while ((dirent = readdir (dir)) != NULL)
        {
            std::string filepath = base::Paths.cfg_data_dir() + "/";
            filepath += dirent->d_name; 
            
            if (strncmp (name_save.c_str (), dirent->d_name, name_save.length ()) == 0)
            {
                load_meta_data (filepath);
            }
        }
        
        // sort user games by creation time
        std::sort (Games.begin()+SPECIAL_SLOT_COUNT, Games.end());
        closedir (dir);
    }
}

// remove files from a saved game directory
void savegame::cleanup (const std::string & name)
{
    struct dirent *dirent;
    DIR *dir;

    if ((dir = opendir (name.c_str ())) != NULL)
    {
        while ((dirent = readdir (dir)) != NULL)
        {
#ifdef WIN32
            if (S_ISREG (get_file_type (name, dirent->d_name)))
#else
            if (dirent->d_type == DT_REG)
#endif
            {
                std::string file = name + "/" + dirent->d_name;
                unlink (file.c_str());
            }
        }
        
        closedir (dir);
    }
    else
    {
#ifdef WIN32
        if (mkdir (name.c_str()))
#else
        if (mkdir (name.c_str(), 0700))
#endif
        {
            fprintf (stderr, "*** savegame::save: failed to create directory\n    %s\n", name.c_str ());
        }
    }
}

// get game at given slot
savegame_data *savegame::get (const s_int32 & slot)
{
    // avoid error message
    if (slot == NEW_SAVE) return NULL;
    
    s_int32 real_slot = slot + SPECIAL_SLOT_COUNT;
    if (real_slot < 0 || real_slot >= Games.size())
    {
        fprintf (stderr, "*** savegame::get: slot %i out of range [%i, %i[\n", 
                 slot, -SPECIAL_SLOT_COUNT, count());
        return NULL;
    }
    
    return Games[real_slot];
}

// save meta data
bool savegame::save_meta_data (savegame_data *data)
{
    base::diskio file;
    
    file.put_string ("desc", data->description());
    file.put_uint32 ("time", data->gametime());
    
    return file.put_record (data->directory() + "/meta.data");
}

// load saved game meta data
bool savegame::load_meta_data (const std::string & filepath)
{
    struct stat statbuf;
    
    if (stat (filepath.c_str (), &statbuf) != -1 && S_ISDIR (statbuf.st_mode)) 
    {
        base::diskio file;
        
        // found a (possibly) valid saved game directory
        // --> try to read the saved game's data record
        if (file.get_record (filepath + "/meta.data")) 
        {
            std::string desc = file.get_string("desc");
            u_int32 time = file.get_uint32("time");
            
            savegame_data *data = new savegame_data (filepath, desc, time);
            
            // get modification time of this game
            time_t mod_time = statbuf.st_mtime;
            data->set_last_modified(mod_time);
            
            Games.push_back (data);
            return true;
        }
    }
    
    return false;
}

// register serializer
void savegame::add (base::serializer_base* serializer)
{
    Serializer ().push_back (serializer);
}

// remove serializer
void savegame::remove (base::serializer_base* serializer)
{
    Serializer ().remove (serializer);
}

// factories for loading/saving game data
std::list<base::serializer_base*>& savegame::Serializer ()
{
    static std::list<base::serializer_base*> *Serializer = new std::list<base::serializer_base*>();
    return *Serializer;
}

