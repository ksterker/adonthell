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
 * @file   world/area_manager.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 *
 * @brief  Defines the area manager class.
 */

#include <fstream>

#include "base/savegame.h"
#include "world/area_manager.h"

using world::area_manager;

/// buffer for copying tainted map files
#define BUF_SIZE 8192
/// file name of data file
#define DATA_FILE "world.data"

// no map loaded initially
world::area* area_manager::ActiveMap = NULL;

// the map view
world::mapview area_manager::MapView;

// the pathfinder
world::pathfinding_manager area_manager::PathFinder;

// list of maps that have been modified
std::hash_set<std::string> area_manager::TaintedMaps;

// reset area manager
void area_manager::cleanup ()
{
    ActiveMap = NULL;
    PathFinder.clear ();
    MapView.clear ();
    TaintedMaps.clear ();
}

// set active map
bool area_manager::set_active_map (const std::string & name, const bool & auto_save)
{
    // save current map before loading new one
    if (ActiveMap != NULL)
    {
        if (auto_save)
        {
            // there is a slight risk here that we're still running from
            // the initial save game, which we shouldn't override. The
            // proper thing here might be to do an auto-save here, but for
            // now we just save the active map to the current slot.
            std::string savedir = base::savegame::current_path();
            ActiveMap->save (savedir);
        }
        
        // the current map has been modified, so we must make sure
        // it will be copied from one save directory to another,
        // even if it is not currently active.
        TaintedMaps.insert (ActiveMap->filename());
        
        // cleanup
        ActiveMap->clear();
        PathFinder.clear();        
    }
    else
    {
        // create initial area
        ActiveMap = new area();
    }
    
    // load the map
    ActiveMap->load (name);

    return true;
}

// save to disk
bool area_manager::save (const std::string & path)
{
    base::diskio file;
    base::flat record;
    bool result = true;
        
    // need to copy tainted maps to new save directory?
    if (path != base::savegame::current_path())
    {
        result &= copy_tainted_maps (base::savegame::current_path(), path);
    }
    
    // save current map
    if (!ActiveMap)
    {
        LOG(ERROR) << "*** area_manager::save: no active map!";
        return false;
    }
    result &= ActiveMap->save (path + "/" + ActiveMap->filename());
    
    // save world data
    file.put_string ("area", ActiveMap->filename());
    for (std::hash_set<std::string>::const_iterator i = TaintedMaps.begin(); i != TaintedMaps.end(); i++)
    {
        record.put_string ("", *i);
    }
    file.put_flat ("tainted", record);
    
    // save map view and pathfinding state
    MapView.put_state (file);
    PathFinder.put_state (file);
    
    // write to disk
    result &= file.put_record (path + "/" + DATA_FILE);
    
    return result;
}

// load from disk
bool area_manager::load ()
{
    base::diskio file;
    char *value;
    
    // cleanup
    TaintedMaps.clear();
    
    // try to load world data
    if (!file.get_record (DATA_FILE)) return false;
    
    std::string filename = file.get_string ("area");
    set_active_map (filename, false);
    
    base::flat record = file.get_flat ("tainted");
    while (record.next((void**)&value) == base::flat::T_STRING)
    {
        TaintedMaps.insert (value);
    }
    
    // load map view and pathfinding state
    MapView.get_state (file);
    PathFinder.get_state (file);
    
    return file.success();
}

// copy tainted maps from previous to new save game folder
bool area_manager::copy_tainted_maps (const std::string & source, const std::string & target)
{
    std::hash_set<std::string>::const_iterator i;
    char buffer[BUF_SIZE];
    
    for (i = TaintedMaps.begin(); i != TaintedMaps.end(); i++)
    {
        // do not copy current map, it'll be saved anyway
        if (*i == ActiveMap->filename()) continue;
        
        int bytes_read = 1;
        
        ifstream source_file ((source + *i).c_str(), ios::binary);
        ofstream target_file ((target + *i).c_str(), ios::binary | ios::trunc);
        
        while (bytes_read != 0)
        {
            source_file.read (buffer, BUF_SIZE);
            bytes_read = source_file.gcount ();
            target_file.write (buffer, bytes_read);
        }
        
        source_file.close();
        target_file.close();
    }
    
    return true;
}
