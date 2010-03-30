/*
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

#include <cmath>

#include "base/logging.h"
#include "rpg/character.h"
#include "world/area.h"
#include "world/character.h"
#include "world/shadow.h"

using world::character;
using world::area;

// ctor
character::character (area & mymap, rpg::character * mind) : moving (mymap)
{
    Type = CHARACTER;
    VSpeed = 0;
    IsRunning = false;
    ToggleRunning = false;
    CurrentDir = NONE;
    Heading = NONE;
    Old_Terrain = NULL;
    Schedule.set_owner (this);

    // save the representation of this character on the rpg side
    Mind = mind;

    std::cerr << "world::character::character() invoked" << std::endl;
}

// dtor
character::~character ()
{
}

// get real speed
float character::speed ()
{
    // We need the rpg side
    if (Mind == NULL) return 1.5f;

    // Obtain floor below character and check if it is the same as the last frame
    if ((Terrain != NULL) && (Terrain != Old_Terrain))
    {
        // Update speed
        mind()->update_speed(*Terrain);

        Old_Terrain = Terrain;
    }

    return mind()->speed();
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
    // character movement
    Schedule.update ();

    // reset vertical velocity
    set_vertical_velocity (VSpeed);

    // update character
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
        else
        {
            // Update speed over different terrains
            // But only when character is moving
            if (current_dir() != character::NONE)
            {
                update_velocity (current_dir());
            }
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
    LOG(INFO) << logging::indent() << "set_direction(" << ndir << ") called";
    logging::increment_log_indent_level();

    update_velocity(ndir);
    update_state();

    LOG(INFO) << logging::indent() << "CurrentDir was: " << CurrentDir;
    CurrentDir = ndir;
    LOG(INFO) << logging::indent() << "CurrentDir is: "  << CurrentDir;

    logging::decrement_log_indent_level();
}

// recalculate the character's speed
void character::update_velocity (const s_int32 & ndir)
{
    LOG(INFO) << logging::indent() << "update_velocity(" << ndir << ") called";
    logging::increment_log_indent_level();

    float vx = 0.0;
    float vy = 0.0;

    if (ndir & WEST)  vx = -speed() * (1 + is_running());
    if (ndir & EAST)  vx =  speed() * (1 + is_running());
    if (ndir & NORTH) vy = -speed() * (1 + is_running());
    if (ndir & SOUTH) vy =  speed() * (1 + is_running());

    LOG(INFO) << logging::indent() << "vx: " << vx;
    LOG(INFO) << logging::indent() << "vy: " << vy;

    if (vx && vy && ! std::isnan(vx) && ! std::isnan(vy))
    {
        float s = 1/sqrt (vx*vx + vy*vy);

        vx = (vx * std::fabs (vx)) * s;
        vy = (vy * std::fabs (vy)) * s;

        LOG(INFO) << logging::indent() << "vx (adjusted): " << vx;
        LOG(INFO) << logging::indent() << "vy (adjusted): " << vy;
    }

    set_velocity(vx, vy);

    logging::decrement_log_indent_level();
}

// figure out name of character shape (and animation) to use
void character::update_state()
{
    LOG(INFO) << logging::indent() << "update_state() called";
    logging::increment_log_indent_level();

    std::string state;

    float xvel = vx () > 0 ? vx () : -vx ();
    float yvel = vy () > 0 ? vy () : -vy ();

    LOG(INFO) << logging::indent() << "xvel: " << xvel;
    LOG(INFO) << logging::indent() << "yvel: " << yvel;

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

    LOG(INFO) << logging::indent() << "state: " << state;

    // set direction the character is actually facing now
    if      (state[0] == 'e') Heading = EAST;
    else if (state[0] == 'w') Heading = WEST;
    else if (state[0] == 's') Heading = SOUTH;
    else                      Heading = NORTH;

    // update sprite
    set_state (state);

    logging::decrement_log_indent_level();
}

// save to stream
bool character::put_state (base::flat & file) const
{
    // save base data
    placeable::put_state (file);

    // save movement and direction ...
    Position.put_state (file, "pos");
    file.put_sint32 ("dir", CurrentDir);
    file.put_float ("vspeed", VSpeed);
    
    // save schedule
    base::flat record;
    Schedule.put_state (record);
    file.put_flat ("schedule", record);    
}

// load from stream
bool character::get_state (base::flat & file)
{
    // load other parts of placeable
    placeable::get_state (file);

    // load movement
    Position.set_str (file.get_string ("pos"));
    set_position (Position.x(), Position.y());
    set_altitude (Position.z());
    calculate_ground_pos ();
    
    // update direction
    set_direction (file.get_sint32 ("dir"));
    VSpeed = file.get_float ("vspeed");
    
    // load schedule
    base::flat record = file.get_flat ("schedule");
    return Schedule.get_state (record);
}

// load placeable model
bool character::load_model (base::flat & model)
{
    // load shapes and sprites
    if (placeable::load_model (model))
    {
        // load (optional) shadow
        std::string shadow_file = model.get_string ("shadow", true);
        if (shadow_file.length() > 0) MyShadow = new shadow (shadow_file, this, EntireCurPos);
    }
    
    return model.success();
}
