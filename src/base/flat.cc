/*
   $Id: flat.cc,v 1.1 2004/03/13 12:38:10 ksterker Exp $

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
 * @file base/flat.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 *
 * @brief An internal class used to flatten various data.
 */

#include <zlib.h>
#include "base/flat.h"

using base::flat;
 
// ctor
flat::flat (const u_int16 & size)
{
    Buffer = new char[size];
    memset (Buffer, '\0', size);
    Capacity = size;
    Ptr = Buffer;
    Data = NULL;
    Size = 0;
}

// flatten the given data
void flat::put (const string & name, const u_int8 & type, const u_int32 & size, const void *data) 
{
    u_int32 nl = name.length ();
    u_int32 need = size + nl + 6;
    while (Size + need >= Capacity) grow ();
    
    memcpy (Ptr, name.c_str (), nl);
    Ptr += nl + 1;
    
    memcpy (Ptr, &type, 1);
    memcpy (Ptr + 1, &size, 4);                
    memcpy (Ptr + 5, data, size);
    
    Ptr += size + 5;
    Size += need;
}

// retrieve given data
flat::data* flat::get (const string & name, const u_int8 & type)
{
    if (Data == NULL) parse ();
    
    static data *decoded = Data;
    data *current = decoded;
    
    while (decoded != NULL) {
        if (strcmp (decoded->Name, name.c_str ()) == 0) {
            if (decoded->Type == type) {
                return decoded;
            } else {
                fprintf (stderr, "*** warning: flat::get: retrieving '%s' with wrong type\n", decoded->Name);
                return decoded;
            }
        } else {
            decoded = decoded->Next;
        }
    }
    
    // not found, so restart from beginning
    decoded = Data;
    while (decoded != current) {
        if (strcmp (decoded->Name, name.c_str ()) == 0) {
            if (decoded->Type == type) {
                return decoded;
            } else {
                fprintf (stderr, "*** warning: flat::get: retrieving '%s' with wrong type\n", decoded->Name);
                return decoded;
            }
        } else {
            decoded = decoded->Next;
        }        
    }
    
    // still not found -> panic
    fprintf (stderr, "*** error: flat::get: parameter '%s' not available\n", name.c_str ());
    return NULL;
}

// unflatten data
void flat::parse ()
{
    if (Data != NULL) return;
    
    data *first, *decoded;
    Ptr = Buffer;
    
    while (*Ptr != '\0') 
    {
        decoded = new data;
        if (Data != NULL) Data->Next = decoded;
        else first = decoded;
        
        decoded->Name = (char *) Ptr;
        Ptr += (strlen ((char*) Ptr) + 1);
        
        decoded->Type = *((u_int8*) Ptr);
        Ptr += 1;
        
        decoded->Size = *((u_int32*) Ptr);
        Ptr += 4;
        
        decoded->Content = Ptr;
        Ptr = Ptr + decoded->Size;
        
        Data = decoded;
    }

    decoded->Next = NULL;
    Data = first;
}

u_int32 flat::checksum () const
{
    u_int a32 = adler32 (0, NULL, 0);
    return adler32 (a32, (Bytef*) Buffer, Size);
}
