/*
   $Id: diskio.cc,v 1.9 2009/04/25 13:17:49 ksterker Exp $

   Copyright (C) 2004/2006 Kai Sterker <kaisterker@linuxgames.com>
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

#include <cstdio>
#include "base/diskio.h"
#include "base/diskwriter_gz.h"
#include "base/diskwriter_xml.h"

using base::flat;
using base::diskio;

// ctor
diskio::diskio (const diskio::file_format & format) : flat (256)
{
    switch (format)
    {
        case GZ_FILE:
        {
            Writer = new base::disk_writer_gz ();
            break;
        }
        case XML_FILE:
        {
            Writer = new base::disk_writer_xml ();
            break;
        }
        case BY_EXTENSION:
        {
            Writer = NULL;
            break;
        }
        default:
        {
            Writer = NULL;
            fprintf (stderr, "*** diskio: unknown format specified!\n");
            break;
        }
    }
}

// dtor
diskio::~diskio ()
{
    delete Writer;
}

// read record from file
bool diskio::get_record (const std::string & filename)
{
    if (Writer == NULL)
    {
        get_writer_for_extension (filename);
    }
    
    if (Writer != NULL)
    {
        return Writer->get_state (filename, *this);
    }
    
    return false;
}

// write record to file
bool diskio::put_record (const std::string & filename)
{
    if (Writer == NULL)
    {
        get_writer_for_extension (filename);
    }
    
    if (Writer != NULL)
    {
        return Writer->put_state (filename, *this);
    }
    
    return false;
}

// determine file format from file extension
void diskio::get_writer_for_extension (const std::string & filename)
{
    // treat file names with '.xml' extension as XML, all others as binary
    if (filename.compare (filename.length() - 4, 4, ".xml"))
    {
        Writer = new base::disk_writer_gz ();
    }
    else
    {
        Writer = new base::disk_writer_xml ();
    }
}
