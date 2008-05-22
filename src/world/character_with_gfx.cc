/*
 $Id: character_with_gfx.cc,v 1.11 2008/05/22 13:05:00 ksterker Exp $
 
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
 * @file   world/character_with_gfx.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the character_with_gfx class.
 * 
 * 
 */

#include "base/diskio.h"
#include "world/character_with_gfx.h"

using world::character;
using world::character_with_gfx;
using world::placeable;
using world::placeable_gfx;

// ctor
character_with_gfx::character_with_gfx (area & mymap) : character (mymap),
                                                           placeable_gfx((placeable &)*this),
                                                           shadow (40, 15)
{
	// TODO: shadow needs to be more fitting to shape of character
	shadow.fillrect (0, 0, 40, 15, 45, 20, 45);
	shadow.set_alpha (128);
}

// save character to screen
bool character_with_gfx::put_state (base::flat & file) const
{
    character::put_state (file);
    placeable_model_gfx::put_state (file);
    return true;
}

// load character from stream 
bool character_with_gfx::get_state (base::flat & file)
{
    character::get_state (file);
    placeable_model_gfx::get_state (file);
    set_gfx (current_shape_name ());
    
    return file.success ();
}

// save character to XML file
bool character_with_gfx::save (const std::string & fname) const
{
    // try to save character
    base::diskio record (base::diskio::XML_FILE);
    if (!put_state (record))
    {
        fprintf (stderr, "*** character_with_gfx::save: saving '%s' failed!\n", fname.c_str ());        
        return false;
    }

    // remember filename
    Filename = fname;
    
    // write character to disk
    return record.put_record (fname);
}

// load from XML file
bool character_with_gfx::load (const std::string & fname)
{
    // try to load character
    base::diskio record (base::diskio::XML_FILE);
    
    // remember file name
    Filename = fname;
    
    if (record.get_record (fname)) 
        return get_state (record);
    
    return false;
}

// render character on screen
void character_with_gfx::draw (const s_int16 & x, const s_int16 & y, const gfx::drawing_area * da_opt,
                               gfx::surface * target)
{
    draw_shadow (x, y, da_opt, target);
    placeable_model_gfx::draw (x, y, da_opt, target);
}

// render character shadow on screen
void character_with_gfx::draw_shadow (s_int16 x, s_int16 y, const gfx::drawing_area * da_opt,
                                      gfx::surface * target)
{
    // 'ground_pos() + z()' gives the current distance of the character above ground
    // 'shape->width() + shape->y() - shadow.height()/2' centers the shadow around the character's base
    const placeable_shape * shape = placeable_model_gfx::Target.current_shape();
    shadow.draw (x, y - ground_pos() + z() - shape->width() - shape->y() + shadow.height()/2, da_opt, target);
}
