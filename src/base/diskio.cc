/*
   $Id: diskio.cc,v 1.6 2006/09/22 05:13:16 ksterker Exp $

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

#include "base/diskio.h"
#include "base/diskwriter_gz.h"
#include "base/diskwriter_xml.h"

using base::flat;
using base::diskio;

// ctor
diskio::diskio (const diskio::disk_writer & writer) : flat (256)
{
    switch (writer)
    {
        case GZ_WRITER:
        {
            Writer = new base::disk_writer_gz ();
            break;
        }
        case XML_WRITER:
        {
            Writer = new base::disk_writer_xml ();
            break;
        }
        default:
        {
            Writer = NULL;
            fprintf (stderr, "*** diskio: unknown writer specified!\n");
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
    if (Writer != NULL)
    {
        return Writer->get_state (filename, *this);
    }
    
    return false;
}

// write record to file
bool diskio::put_record (const std::string & filename)
{
    if (Writer != NULL)
    {
        return Writer->put_state (filename, *this);
    }
    
    return false;
}
