/*
   $Id: flat.cc,v 1.14 2007/06/10 03:58:21 ksterker Exp $

   Copyright (C) 2004/2006/2007 Kai Sterker <kaisterker@linuxgames.com>
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
#include "base/endians.h"

#include <zlib.h>

#ifdef __BIG_ENDIAN__
#define DATA_BYTE_ORDER 'B'
#else
#define DATA_BYTE_ORDER 'L'
#endif

using base::flat;

// names for supported data types
char* flat::TypeName[flat::NBR_TYPES] =  {
        "bool", "char", "u_int8", "s_int8", "u_int16", "s_int16",
        "u_int32", "s_int32", "string", "float", "double", "blob",
        "list" };

// ctor
flat::flat (const u_int16 & size)
{
    // this is the maximum capacity of the buffer
    Capacity = (size != 0 ? size : 1);
    
    Buffer = new char[Capacity];
    memset (Buffer, '\0', Capacity);
    
    // first byte in the buffer contains the byte order
    Buffer[0] = DATA_BYTE_ORDER;
    
    // this is the current size of content in the buffer
    Size = 1;
    
    Success = true;
    Ptr = Buffer + 1;
    Data = NULL;
}

// create a flat from internal buffer of another flat
flat::flat (const char *buffer, const u_int32 & size)
{
    Data = NULL;
    Buffer = NULL;
    
    char *tmp = new char[size];
    memcpy (tmp, buffer, size);
    setBuffer (tmp, size);
}

// copy constructor
flat::flat (const flat & f) 
{
    Data = NULL;
    Buffer = NULL;

    copy (f);
}

// copy one flat into another
void flat::copy (const flat & source)
{
    char *tmp = new char[source.size ()];
    memcpy (tmp, source.getBuffer (), source.size ());
    setBuffer (tmp, source.size ());
}

// flatten the given data
void flat::put (const string & name, const data_type & type, const u_int32 & size, const void *data) 
{
	u_int8 t = type;
    u_int32 nl = name.length () + 1;
    u_int32 need = size + nl + 5;
    while (Size + need > Capacity) grow ();
    
    memcpy (Ptr, name.c_str (), nl);
    Ptr += nl;
    
    memcpy (Ptr, &t, 1);
    memcpy (Ptr + 1, &size, 4);
    memcpy (Ptr + 5, data, size);
    
    Ptr += size + 5;
    Size += need;
}

// retrieve given data
flat::data* flat::get (const string & name, const data_type & type)
{
    if (Data == NULL) 
    {
        parse ();
        Decoded = Data;
    }
    
    data *result = NULL;
    data *current = Decoded;
    
    // search from current position
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
            fprintf (stderr, "*** warning: flat::get: retrieving '%s' with wrong type:\n", result->Name);
            fprintf (stderr, "    Expected type was '%s', got '%s' instead!\n", TypeName[type], TypeName[result->Type]);
            return result;
        }
    }
    
    // still not found -> panic
    fprintf (stderr, "*** error: flat::get: parameter '%s' not available\n", name.c_str ());
    Success = false;
    return NULL;
}

// iterate over data
flat::data_type flat::next (void **value, u_int32 *size, char **name)
{
    if (Data == NULL)
    {
        parse ();
        Decoded = Data;
    }
    
    if (Decoded != NULL)
    {
        data_type type = Decoded->Type;
        *value = Decoded->Content;

        if (size != NULL) *size = Decoded->Size;
        if (name != NULL) *name = Decoded->Name;
    
        Decoded = Decoded->Next;
        return type;
    }
    
    // error respectively EOF
    return flat::T_UNKNOWN;
}

// unflatten data
void flat::parse ()
{
    if (Size == 1 || Data != NULL) return;
    
    // whether we need to swap byte order or not
    bool swap = (Buffer[0] != DATA_BYTE_ORDER);
    Buffer[0] = DATA_BYTE_ORDER;
    
    data *first, *decoded;
    u_int32 pos = 1;
    Ptr = Buffer + 1;
    
    while (pos < Size) 
    {
        decoded = new data;
        if (Data != NULL) Data->Next = decoded;
        else first = decoded;
        
        decoded->Name = (char *) Ptr;
        Ptr += (strlen ((char*) Ptr) + 1);
        
        decoded->Type = (data_type) *((u_int8*) Ptr);
        Ptr += 1;
        
        if (swap)
        {
            // get size in correct endianess
            u_int32 tmp = Swap32 (*((u_int32*) Ptr));
            decoded->Size = tmp;
            
            // update buffer
            memcpy (Ptr, &tmp, 4);
            Ptr += 4;
            
            // check whether we need to change buffer
            switch (decoded->Type)
            {
                case T_UINT16:
                case T_SINT16:
                {
                    // update buffer
                    u_int16 t = Swap16 (*((u_int16*) Ptr));
                    memcpy (Ptr, &t, 2);
                    break;
                }
                case T_UINT32:
                case T_SINT32:
                {
                    // update buffer
                    tmp = Swap32 (*((u_int32*) Ptr));
                    memcpy (Ptr, &tmp, 4);
                    break;
                }
                default:
                {
                    break;
                }
            }
        }
        else
        {
            decoded->Size = *((u_int32*) Ptr);
            Ptr += 4;
        }
        
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
    u_int32 a32 = adler32 (0, NULL, 0);
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
