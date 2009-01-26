/*
 $Id: object.cc,v 1.5 2009/01/26 21:09:15 ksterker Exp $
 
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
 * @file   world/object.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the object class.
 * 
 * 
 */

#include "base/diskio.h"
#include "world/object.h"

using world::object;

// create a new scenery object
object::object(world::area & mymap) : placeable(mymap) 
{
    Type = OBJECT; 
}

// save object to stream
bool object::put_state (base::flat & file) const
{
    placeable::put_state (file);
    return true;
}

// load object from stream
bool object::get_state (base::flat & file)
{
    base::flat entity = file.get_flat ("entity");    
    placeable::get_state (entity);
    
    return file.success ();
}

// save object to XML file
bool object::save (const std::string & fname) const
{
    // try to save object
    base::diskio record (base::diskio::XML_FILE);
    if (!put_state (record))
    {
        fprintf (stderr, "*** object::save: saving '%s' failed!\n", fname.c_str ());        
        return false;
    }
    
    // write object to disk
    return record.put_record (fname);
}

// load object from XML file
bool object::load (const std::string & fname)
{
    // try to load character
    base::diskio record (base::diskio::XML_FILE);
    
    if (record.get_record (fname)) 
        return get_state (record);
    
    return false;
}
