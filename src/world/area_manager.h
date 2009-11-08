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
 * @file   world/area_manager.h
 * @author Kai Sterker <kaisterker@linuxgames.com>
 *
 * @brief  Declares the area manager class.
 */

#ifndef WORLD_AREA_MANAGER_H
#define WORLD_AREA_MANAGER_H

#include "base/hash_map.h"

#include "world/pathfinding_manager.h"
#include "world/mapview.h"
#include "world/area.h"

namespace world
{
class area;

/**
 * This class takes care of the currently active map
 * the associated schedule and path finder. It can be
 * used to serialize those to a file and restore them
 * to the same state later on.
 *
 * It furthermore allows to change the active area.
 */
class area_manager
{
public:
    /**
     * @name Map Handling
     */
    //@{
    /**
     * Set the given map file as the current active map. This
     * will save the state of the current map (if any) and load
     * the state of the given map.
     *
     * @param name name of the map to load.
     * @return true on success, false otherwise.
     */
    static bool set_active_map (const std::string & name)
    {
        set_active_map (name, true);
    }
    //@}
    
    /**
     * Update state of world module. Call once for each frame.
     */
    static void update ()
    {
        PathFinder.update();
        ActiveMap->update();
        MapView.update();
    }
    
    /**
     * @name member access
     */
    //@{
    /**
     * Access to the path finder to create path finding tasks.
     * @return pointer to the pathfinding manager.
     */
    static world::pathfinding_manager *get_pathfinder ()
    {
        return & PathFinder;
    }
    
    /**
     * Access to the mapview to change mapview schedule or 
     * renderer.
     * @return pointer to the mapview.
     */
    static world::mapview *get_mapview ()
    {
        return & MapView;
    }

    /**
     * Access to the current map.
     * @return pointer to the active map.
     */
    static world::area *get_map ()
    {
        return ActiveMap;
    }
    //@}

    /**
     * @name Loading/Saving
     */
    //@{
    /**
     * Load current map, pathfinding tasks and mapview from disk. 
     *
     * @return \b true if loading successful, \b false otherwise.
     */
    static bool load ();
    
    /**
     * Save current map, pathfinding tasks and mapview to disk. 
     *
     * @param path name of the directory to save %character data to.
     * @return \b true if saving successful, \b false otherwise.
     */
    static bool save (const string & path);
    //@}
    
private:
    /**
     * Set the given map file as the current active map
     *
     * @param name name of the map to load.
     * @param auto_save whether to save previous map.
     * @return true on success, false otherwise.
     */
    static bool set_active_map (const std::string & name, const bool & auto_save);
    
    /**
     * Copy all tainted maps from the source directory to the target
     * directory, except the currently active map. 
     * 
     * @param source the source directory.
     * @param target the target directory.
     * @return true on success, false otherwise.
     */
    static bool copy_tainted_maps (const std::string & source, const std::string & target);
        
    /// forbid instantiation
    area_manager() {};
    
    /// the current map
    static area *ActiveMap;
    /// the class taking care of pathfinding
    static pathfinding_manager PathFinder;
    /// the main view on the active map 
    static mapview MapView;
    
    /// list of maps that already have been in use
    static std::hash_set<std::string> TaintedMaps;
};

}

#endif
