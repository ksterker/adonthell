/*
   $Id: character.cc,v 1.17 2009/04/26 18:52:59 ksterker Exp $

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
    Speed = 2;
    VSpeed = 0;
    IsRunning = false;
    ToggleRunning = false;
    CurrentDir = NONE;
    Heading = NONE;
    Schedule.set_map (&mymap);

    // save the representation of this character on the world side
    Mind = mind;
    if (Mind != NULL)
    {
        // and do the reverse.
        Mind->set_body(this);
    }
}

// dtor
character::~character ()
{
}

// get real speed
float character::speed () const
{
    // We need the rpg side if we want to have variable speed
    if (Mind == NULL) return base_speed();

    // Obtain floor below character
    if (Terrain != NULL)
    {
        // Got a possibly valid terrain, let's see if it really exists
        float new_speed = mind()->get_speed_on_terrain (*Terrain);

        if (new_speed != -1)
        {
            // printf("Base Speed is %f. Actual Speed is %f. We're walking over %s\n", base_speed(), new_speed, Terrain->c_str());
            return new_speed;
        }
    }

    // Failed. Let's return the default speed
    return base_speed();
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
    update_velocity(ndir);
    update_state();

    Heading = ndir != 0 ? ndir : Heading;
    CurrentDir = ndir;
}

// recalculate the character's speed
void character::update_velocity (const s_int32 & ndir)
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
    base::flat entity = file.get_flat ("entity");

    // FIXME: load movement and direction ...

    // load other parts of placeable
    placeable::get_state (entity);

    // load shadow
    std::string shadow_file = entity.get_string ("shadow");
    MyShadow = new shadow (shadow_file, this, CurPos);

    return file.success ();
}

// save to file
bool character::save (const std::string & fname, const base::diskio::file_format & format) const
{
    // try to save character
    base::diskio record (format);
    if (!put_state (record))
    {
        fprintf (stderr, "*** character::save: saving '%s' failed!\n", fname.c_str ());
        return false;
    }

    // write item to disk
    return record.put_record (fname);
}

// load from file
bool character::load(const std::string & fname)
{
    // try to load character
    base::diskio record (base::diskio::BY_EXTENSION);

    if (record.get_record (fname))
    {
        Filename = fname;
        return get_state (record);
    }

    return false;
}
