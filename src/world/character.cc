/*
   $Id: character.cc,v 1.10 2008/10/05 09:22:03 ksterker Exp $

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
 * @file   world/character.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the character class.
 * 
 * 
 */

#include <math.h>

#include "base/diskio.h"
#include "world/character.h"

using world::character;
using world::area;

// ctor
character::character (area & mymap) : moving (mymap)
{
    Type = CHARACTER;
    Speed = 2;
    VSpeed = 0;
    IsRunning = false;
    ToggleRunning = false;
    CurrentDir = NONE;
}

// jump
void character::jump()
{
    // only jump if resting on the ground
	if (GroundPos == z())
	{
    	VSpeed = 10;
	}
}

// process character movement
bool character::update ()
{
    // reset vertical velocity
    set_vertical_velocity (VSpeed);
    moving::update ();

    if (GroundPos == z()) 
    {
        VSpeed = 0;
        
    	// character no longer jumping or falling
    	if (IsRunning != ToggleRunning)
    	{
    		IsRunning = ToggleRunning;
    		set_direction (current_dir());
    	}
    }
    else if (VSpeed > 0) VSpeed -= 0.4;
    
    return true;
}

// add direction to character movement
void character::add_direction(direction ndir)
{
    s_int32 tstdir = current_dir();
    switch (ndir)
    {
        case WEST:
            tstdir &= ~EAST;
            break;
        case EAST:
            tstdir &= ~WEST;
            break;
        case SOUTH:
            tstdir &= ~NORTH;
            break;
        case NORTH:
            tstdir &= ~SOUTH;
            break;
        default:
            break;
    }
    
    set_direction(tstdir | ndir);
}

// set character movement
void character::set_direction (const s_int32 & ndir)
{
    float vx = 0.0, vy = 0.0;
    
    if (ndir & WEST) vx = -speed() * (1 + is_running());
    if (ndir & EAST) vx = speed() * (1 + is_running());
    if (ndir & NORTH) vy = -speed() * (1 + is_running());
    if (ndir & SOUTH) vy = speed() * (1 + is_running());
    
    if (vx && vy)
    {
        float s = 1/sqrt (vx*vx + vy*vy);
        vx = (vx * fabs (vx)) * s;
        vy = (vy * fabs (vy)) * s;
    }
    
    set_velocity(vx, vy);
    update_state();
    
    CurrentDir = ndir;    
}

// figure out name of character shape (and animation) to use
void character::update_state()
{
    std::string state;
    float xvel = vx () > 0 ? vx () : -vx (); 
    float yvel = vy () > 0 ? vy () : -vy ();
    
    if (xvel || yvel) 
    {
        if (xvel > yvel) 
        {
            if (vx () > 0) state = "e";
            else if (vx () < 0) state = "w"; 
        }
        else if (yvel > xvel) 
        {
            if (vy () > 0) state = "s";
            else if (vy () < 0) state = "n"; 
        }
        else 
        {
            if ((vx() > 0) && (CurrentDir & WEST))
                state = "e";
            else if ((vx() < 0) && (CurrentDir & EAST))
                state = "w";
            else if ((vy() > 0) && (CurrentDir & NORTH))
                state = "s";
            else if ((vy() < 0) && (CurrentDir & SOUTH))
                state = "n";
            else state = placeable::state()[0];
        }
        state += is_running() ? "_run" : "_walk";
    }
    else
    {
        state = placeable::state()[0];
        state += "_stand";
    }
    
    set_state (state);
}

// save to stream
bool character::put_state (base::flat & file) const
{
    // FIXME: save movement and direction ...
    
    return placeable::put_state (file);
}

// load from stream
bool character::get_state (base::flat & file)
{
    // FIXME: load movement and direction ...
    placeable::get_state (file);
    
    return file.success ();
}

// save to XML file
bool character::save (const std::string & fname) const
{
    // try to save character
    base::diskio record (base::diskio::XML_FILE);
    if (!put_state (record))
    {
        fprintf (stderr, "*** character::save: saving '%s' failed!\n", fname.c_str ());        
        return false;
    }
    
    // write item to disk
    return record.put_record (fname);
}

// load from XML file
bool character::load(const std::string & fname)
{
    // try to load character
    base::diskio record (base::diskio::XML_FILE);
    
    if (record.get_record (fname)) 
        return get_state (record);
    
    return false;
}
