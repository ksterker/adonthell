/*
   $Id: flat.cc,v 1.4 2004/04/29 08:07:49 ksterker Exp $

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

#include "base/flat.h"
#include <zlib.h>

using base::flat;
 
// ctor
flat::flat (const u_int16 & size)
{
    Buffer = new char[size];
    memset (Buffer, '\0', size);
    Capacity = size;
    Success = true;
    Ptr = Buffer;
    Data = NULL;
    Size = 0;
}

// 'copy' ctor
flat::flat (const char *buffer, const u_int32 & size) 
{
    Data = NULL;
    Buffer = NULL;
    Success = true;
    
    char *tmp = new char[size];
    memcpy (tmp, buffer, size);
    setBuffer (tmp, size);
}

// flatten the given data
void flat::put (const string & name, const u_int8 & type, const u_int32 & size, const void *data) 
{
    u_int32 tmp = SwapLE32(size);
    u_int32 nl = name.length () + 1;
    u_int32 need = size + nl + 5;
    while (Size + need > Capacity) grow ();
    
    memcpy (Ptr, name.c_str (), nl);
    Ptr += nl;
    
    memcpy (Ptr, &type, 1);
    memcpy (Ptr + 1, &tmp, 4);
    memcpy (Ptr + 5, data, size);
    
    Ptr += size + 5;
    Size += need;
}

// retrieve given data
flat::data* flat::get (const string & name, const u_int8 & type)
{
    if (Data == NULL) 
    {
        parse ();
        Decoded = Data;
    }
    
    data *result = NULL;
    data *current = Decoded;
    
    for (; Decoded != NULL; Decoded = Decoded->Next)
        if (strcmp (Decoded->Name, name.c_str ()) == 0) {
            result = Decoded;
            break;
        }
        
    // not found, so restart from beginning
    if (result == NULL)
        for (Decoded = Data; Decoded != current; Decoded = Decoded->Next)
            if (strcmp (Decoded->Name, name.c_str ()) == 0) {
                result = Decoded;
                break;
            }

    // in case we have a result ...
    if (result != NULL) {
        // fetch next piece of data
        Decoded = Decoded->Next;
        
        // check whether types match
        if (result->Type == type) {
            return result;
        } else {
            fprintf (stderr, "*** warning: flat::get: retrieving '%s' with wrong type\n", Decoded->Name);
            return result;
        }
    }
    
    // still not found -> panic
    fprintf (stderr, "*** error: flat::get: parameter '%s' not available\n", name.c_str ());
    Success = false;
    return NULL;
}

// iterate over data
int flat::next (void **value, int *size, char **name)
{
    if (Data == NULL)
    {
        parse ();
        Decoded = Data;
    }
    
    if (Decoded != NULL)
    {
        *value = Decoded->Content;
        if (size != NULL) *size = Decoded->Size;
        if (name != NULL) *name = Decoded->Name;
        int type = Decoded->Type;
    
        Decoded = Decoded->Next;
        return type;
    }
    
    // error respectively EOF
    return -1;
}

// unflatten data
void flat::parse ()
{
    if (Data != NULL) return;
    
    data *first, *decoded;
    u_int32 pos = 0;
    Ptr = Buffer;
    
    while (pos < Size) 
    {
        decoded = new data;
        if (Data != NULL) Data->Next = decoded;
        else first = decoded;
        
        decoded->Name = (char *) Ptr;
        Ptr += (strlen ((char*) Ptr) + 1);
        
        decoded->Type = *((u_int8*) Ptr);
        Ptr += 1;
        
        decoded->Size = SwapLE32(*((u_int32*) Ptr));
        Ptr += 4;
        
        decoded->Content = Ptr;
        Ptr = Ptr + decoded->Size;
        
        Data = decoded;
        pos += strlen (decoded->Name) + decoded->Size + 6;
    }

    decoded->Next = NULL;
    Data = first;
}

// calculate checksum of internal buffer
u_int32 flat::checksum () const
{
    u_int a32 = adler32 (0, NULL, 0);
    return adler32 (a32, (Bytef*) Buffer, Size);
}

// grow internal buffer
void flat::grow ()
{
    Capacity *= 2;
    char *tmp = new char[Capacity];
    
    if (tmp == NULL) {
        fprintf (stderr, "*** flat::grow: failed to allocate %i more bytes. Giving up ...\n", Capacity);
        exit (1);
    }
    
    memcpy (tmp, Buffer, Size);
    memset (tmp + Size, '\0', Capacity - Size);
    delete[] Buffer;
    
    Buffer = tmp;
    Ptr = Buffer + Size;
}
