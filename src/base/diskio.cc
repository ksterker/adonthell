/*
   $Id: diskio.cc,v 1.1 2004/03/13 12:38:10 ksterker Exp $

   Copyright (C) 2004 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file base/diskio.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 *
 * @brief More secure I/O classes for writing to/reading from file.
 */

#include "base/diskio.h"

using base::flat;
using base::diskio;

// ctor
diskio::diskio () : flat (256)
{
}

// read record from files
bool diskio::get_record (igzstream & in)
{
    if (in.eof ()) {
        fprintf (stderr, "*** diskio::get_record: already at end of file\n");
        return false;
    }
    
    u_int32 length, checksum;
    length << in;
    
    char *buffer = new char[length];
    if (!buffer) {
        fprintf (stderr, "*** diskio::get_record: failed to allocate %i bytes. Giving up ...\n", length);
        return false;
    }
    
    in.get_block (buffer, length);
    setBuffer (buffer, length);
    
    checksum << in;
    if (checksum != this->checksum ()) {
        fprintf (stderr, "*** diskio::get_record: checksum error. Data may be corrupted.\n");
        return false;
    }
    
    return true;
}

// write record to file
void diskio::put_record (ogzstream & out)
{
    size () >> out;
    out.put_block ((void*) getBuffer (), size ());
    checksum () >> out;
    
     // reset
     clear (); 
}
