/*
 $Id: object_with_gfx.cc,v 1.3 2007/10/15 02:19:31 ksterker Exp $
 
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
 * @file   world/object_with_gfx.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the object_with_gfx class.
 * 
 * 
 */

#include "base/diskio.h"
#include "world/object_with_gfx.h"

using world::object_with_gfx;

// ctor
object_with_gfx::object_with_gfx (world::area & mymap) : object (mymap), placeable_gfx ((placeable &) *this) 
{
    set_shape ("default"); 
}

// save to stream
bool object_with_gfx::put_state (base::flat & file) const
{
    object::put_state (file);
    placeable_model_gfx::put_state (file);
    
    return true;
}

// load from stream
bool object_with_gfx::get_state (base::flat & file)
{
    object::get_state (file);
    placeable_model_gfx::get_state (file);
    set_gfx (current_shape_name());
    
    return file.success ();
}

bool object_with_gfx::save (const std::string & fname) const
{
    // try to save character
    base::diskio record (base::diskio::XML_FILE);
    if (!put_state (record))
    {
        fprintf (stderr, "*** object_with_gfx::save: saving '%s' failed!\n", fname.c_str ());        
        return false;
    }
    
    // remember filename
    Filename = fname;
    
    // write character to disk
    return record.put_record (fname);
}

// load from XML file
bool object_with_gfx::load (const std::string & fname)
{
    // try to load character
    base::diskio record (base::diskio::XML_FILE);
    
    // remember file name
    Filename = fname;
    
    if (record.get_record (fname)) 
        return get_state (record);
    
    return false;
}

// render object
void object_with_gfx::draw (s_int16 x, s_int16 y, const gfx::drawing_area * da_opt,
                            gfx::surface * target) const
{
    placeable_model_gfx::draw (x, y, da_opt, target);
}
