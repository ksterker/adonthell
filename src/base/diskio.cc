/*
   $Id: diskio.cc,v 1.4 2006/06/18 19:25:52 ksterker Exp $

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

using base::flat;
using base::diskio;

char *diskio::Bin2Hex = "0123456789ABCDEF";
#define READ_BUFFER_SIZE 16384

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
        fprintf (stderr, "*** diskio::get_record: checksum error. Data might be corrupt.\n");
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

// read record from ASCII file
bool diskio::get_ascii (FILE * in)
{
    char *ptr, *val, *name;
    char type[1];
    char buffer[READ_BUFFER_SIZE];
    char *end = &buffer[READ_BUFFER_SIZE];
    u_int32 read = fread (buffer, 1, READ_BUFFER_SIZE, in);
    u_int32 size;
 
    // check that first byte is the beginning of a new record
    if (read > 0 && buffer[0] == '{')
    {
        ptr = buffer;
        while (ptr < end)
        {
            ptr++;

            // skip whitespace
            while (ptr < end && isspace (*ptr))
            {
                ptr++;
                continue;
            }
                   
            // read variable name --> everything before the '/'
            val = ptr;
            while (ptr < end && *ptr != '/')
            {
                ptr++;
                continue;
            }
            
            u_int32 len = ptr-val;
            name = new char[len];
            memcpy (name, val, len);
            name[len] = '\0';
            
            // read variable type
            type[0] = *(++ptr);
            
            // read variable size
            val = ptr++;
            while (ptr < end && isdigit (*ptr))
            {
                ptr++;
                continue;
            }
            
            // convert size
            if (val == ptr)
            {
                size = 1;
            }
            else
            {
                size = (u_int32) strtol (val, NULL, 10);
            }

            // read value
            switch (type[0])
            {
                // string
                case 'A':
                {
                    break;
                }
                // bool
                case 'B':
                {
                    break;
                }
                // char
                case 'C':
                {
                    break;
                }
                // double
                case 'F':
                {
                    break;
                }
                // signed int
                case 'S':
                {
                    break;
                }
                // unsigned int
                case 'U':
                {
                    break;
                }
                // record
                case 'R':
                {
                    break;
                }
                // binary
                case 'X':
                {
                    break;
                }
                default:
                {
                    fprintf (stderr, "*** diskio::get_ascii: unknown type '%c'!\n", type[0]);
                    return false;
                }
            }
        }
    }
    else
    {
        fprintf (stderr, "*** diskio::get_ascii: file does not point to start of new record\n!");
        return false;
    }
    
    return true;
}

// save record to ASCII file
void diskio::put_ascii (FILE * out)
{
    write_record (*this, out);
}

// recursively saves records to file
void diskio::write_record (base::flat &record, FILE * out, const u_int16 & indent)
{
    const char *in2 = std::string (indent+1, '\t').c_str ();
    std::string ind (indent, '\t');
    int type, size;
    void *value;
    char *name;
    
    // reset record iterator
    record.first ();
    
    // write record start
    fprintf (out, "{\n");
    
    // write record data
    while ((type = record.next (&value, &size, &name)) != -1)
    {
        switch (type)
        {
            // write boolean type
            case T_BOOL:
            {
                fprintf (out, "%s%s/B=%i\n", in2, name, *((bool*) value) ? 1 : 0);
                break;
            }
            // write signed integer types
            case T_SINT8:
            {
                fprintf (out, "%s%s/S1=%i\n", in2, name, *((s_int8*) value));
                break;
            }
            case T_SINT16:
            {
                fprintf (out, "%s%s/S2=%i\n", in2, name, *((s_int16*) value));
                break;
            }
            case T_SINT32:
            {
                fprintf (out, "%s%s/S4=%i\n", in2, name, *((s_int32*) value));
                break;
            }
            // write unsigned integer types
            case T_UINT8:
            {
                fprintf (out, "%s%s/U1=%u\n", in2, name, *((u_int8*) value));
                break;
            }
            case T_UINT16:
            {
                fprintf (out, "%s%s/U2=%u\n", in2, name, *((u_int16*) value));
                break;
            }
            case T_UINT32:
            {
                fprintf (out, "%s%s/U4=%u\n", in2, name, *((u_int32*) value));
                break;
            }
            // write float types
            case T_FLOAT:
            {
                float f = (float) strtod ((char *) value, NULL);
                fprintf (out, "%s%s/F4=%g\n", in2, name, f);
                break;
            }
            case T_DOUBLE:
            {
                double d = strtod ((char *) value, NULL);
                fprintf (out, "%s%s/F8=%g\n", in2, name, d);
                break;
            }
            // write character
            case T_CHAR:
            {
                fprintf (out, "%s%s/C=\"%c\"\n", in2, name, *((char*) value));
                break;
            }
            // write string
            case T_STRING:
            {
                fprintf (out, "%s%s/A%i=\"%s\"\n", in2, name, size-1, ((char*) value));
                break;
            }
            // write binary
            case T_BLOB:
            {
                u_int32 j = 0;
                char *bin = (char *) value;
                char *hex = new char[(size * 2) + 1];
                hex[size * 2] = 0;
                
                for (s_int32 i = 0; i < size; i++) 
                {
                    hex[j++] = Bin2Hex[(bin[i] >> 4) & 0x0f];
                    hex[j++] = Bin2Hex[bin[i] & 0x0f];
                }
                
                fprintf (out, "%s%s/X%i=%s\n", in2, name, size, hex);
                delete[] hex;

                break;
            }
            // write record
            case T_FLAT:
            {
                fprintf (out, "%s%s/R=", in2, name);
                base::flat rec = base::flat ((const char *) value, size);
                write_record (rec, out, indent+1);
                break;
            }
        }
    }

    // write record end;
    fprintf (out, "%s}\n", ind.c_str());
}
