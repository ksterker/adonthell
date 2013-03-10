/*
 $Id: diskwriter_gz.cc,v 1.5 2009/04/25 13:17:49 ksterker Exp $
 
 Copyright (C) 2006 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file base/diskwriter_gz.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 *
 * @brief Read/write gz compressed data files.
 */

#include <cstdio>
#include "base.h"
#include "file.h"
#include "diskwriter_gz.h"
#include "logging.h"

using base::disk_writer_gz;

// write to gz-compressed binary file
bool disk_writer_gz::put_state (const std::string & name, base::flat & data) const
{
    // open file
    base::ogzstream out (name);
    if (!out.is_open ())
    {
        LOG(ERROR) << "disk_writer_gz::put_state: cannot open '" << name << "' for writing!";
        return false; 
    }
    
    // write byte order
    data.byte_order () >> out;
    
    // write header
    data.size () >> out;
    
    // write data
    out.put_block ((void*) data.getBuffer (), data.size ());
    
    // write checksum
    data.checksum () >> out;
    
    // reset
    data.clear ();
	
	return true;
}

// read from gz-compressed binary file
bool disk_writer_gz::get_state (const std::string & name, base::flat & data) const
{
    u_int8 byte_order;
    u_int32 length, checksum;
    base::igzstream in;
    
    // open file
    if (!in.open (name))
    {
        LOG(ERROR) << "disk_writer_gz::get_state: cannot open '" << name << "' for reading!";
        return false; 
    }
    
    // does file contain data
    if (in.eof ()) {
        LOG(ERROR) << "disk_writer_gz::get_state: file '" << name << "' is empty!";
        return false;
    }
    
    // get byte order
    byte_order << in;
    
    // check for correct format
    if (byte_order != 'L' && byte_order != 'B')
    {
        LOG(ERROR) << "disk_writer_gz::get_state: file '" << name << " has invalid byte order '" << byte_order << "'!";
        return false;
    }
    
    // get data size
    length << in;
    
    // create buffer for reading data
    char *buffer = new char[length];
    if (!buffer) {
        LOG(FATAL) << "disk_writer_gz::get_state: failed to allocate " << length << " bytes. Giving up ...";
    }
    
    // read data
    in.get_block (buffer, length);
    data.setBuffer (buffer, length);
    
    // read checksum
    checksum << in;
    
    // validate checksum
    if (checksum != data.checksum ()) {
        LOG(ERROR) << "disk_writer_gz::get_state: checksum error in file '" << name << "'.";
        LOG(ERROR) << "Data might be corrupt.";
        return false;
    }
    
    return true;
}
