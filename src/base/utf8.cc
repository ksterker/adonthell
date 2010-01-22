/*
 Copyright (C) 2010 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file base/utf8.cc 
 * @author Kai Sterker <kaisterker@linuxgames.com>
 *
 * @brief Internal helper class for UTF-8 encoded strings.
 */

#include <algorithm>
#include "base/utf8.h"

using base::utf8;

/// buffer for converting UTF-16 surrogate pair to UTF-8
u_int32 utf8::CodeHigh = 0;

// convert UTF-16 character to UTF-8
std::string utf8::from_utf16 (const u_int16 & chr)
{
    // code borrowed from grub
    char result[5];
    u_int32 code = chr;
    
    if (CodeHigh)
    {
        if (code >= 0xDC00 && code <= 0xDFFF)
        {
            // Surrogate pair.
            code = ((CodeHigh - 0xD800) << 12) + (code - 0xDC00) + 0x10000;
            
            result[0] = (code >> 18) | 0xF0;
            result[1] = ((code >> 12) & 0x3F) | 0x80;
            result[2] = ((code >> 6) & 0x3F) | 0x80;
            result[3] = (code & 0x3F) | 0x80;
            result[4] = 0;
        }
        
        CodeHigh = 0;
    }
    else
    {
        if (code <= 0x007F)
        {
            result[0] = code;
            result[1] = 0;
        }
        else if (code <= 0x07FF)
        {
            result[0] = (code >> 6) | 0xC0;
            result[1] = (code & 0x3F) | 0x80;
            result[2] = 0;
        }
        else if (code >= 0xD800 && code <= 0xDBFF)
        {
            CodeHigh = code;
            return "";
        }
        else
        {
            result[0] = (code >> 16) | 0xE0;
            result[1] = ((code >> 12) & 0x3F) | 0x80;
            result[2] = (code & 0x3F) | 0x80;
            result[3] = 0;
        }
    }
    
    return std::string(result);
}

u_int32 utf8::to_utf32 (const std::string & source, std::string::const_iterator & iter)
{
    int count = 0;
    u_int32 code = 0;
    
    do
    {
        u_int32 c = *iter++;
        
        if (count)
        {
            if ((c & 0xc0) != 0x80)
            {
                /* invalid */
                return '?';
            }
            else
            {
                code <<= 6;
                code |= (c & 0x3f);
                count--;
            }
        }
        else
        {
            if ((c & 0x80) == 0x00)
            {
                code = c;
            }
            else if ((c & 0xe0) == 0xc0)
            {
                count = 1;
                code = c & 0x1f;
            }
            else if ((c & 0xf0) == 0xe0)
            {
                count = 2;
                code = c & 0x0f;
            }
            else if ((c & 0xf8) == 0xf0)
            {
                count = 3;
                code = c & 0x07;
            }
            else if ((c & 0xfc) == 0xf8)
            {
                count = 4;
                code = c & 0x03;
            }
            else if ((c & 0xfe) == 0xfc)
            {
                count = 5;
                code = c & 0x01;
            }
            else
            {
                /* invalid */
                return '?';
            }
        }
    }
    while (count > 0 && iter != source.end());
    
    return code;
}

// backward iteration
u_int32 utf8::left (const std::string & source, const u_int32 & pos)
{
    u_int32 count = 1;
    
    if (pos > 0 && (source[pos-1] & 0x80) == 0x00)
    {
        count = 1;
    }
    else if (pos > 1 && (source[pos-2] & 0xe0) == 0xc0)
    {
        count = 2;
    }
    else if (pos > 2 && (source[pos-3] & 0xf0) == 0xe0)
    {
        count = 3;
    }
    else if (pos > 3 && (source[pos-4] & 0xf8) == 0xf0)
    {
        count = 4;
    }
    else if (pos > 4 && (source[pos-5] & 0xfc) == 0xf8)
    {
        count = 5;
    }
    else if (pos > 5 && (source[pos-6] & 0xfe) == 0xfc)
    {
        count = 6;
    }
    
    return std::min (count, pos);    
}

// forward iteration
u_int32 utf8::right (const std::string & source, const u_int32 & pos)
{
    unsigned long count = 0;
    u_int32 c = source[pos];
    
    if ((c & 0x80) == 0x00)
    {
        count = 1;
    }
    else if ((c & 0xe0) == 0xc0)
    {
        count = 2;
    }
    else if ((c & 0xf0) == 0xe0)
    {
        count = 3;
    }
    else if ((c & 0xf8) == 0xf0)
    {
        count = 4;
    }
    else if ((c & 0xfc) == 0xf8)
    {
        count = 5;
    }
    else if ((c & 0xfe) == 0xfc)
    {
        count = 6;
    }
    
    return (u_int32) std::min (count, source.length() - pos);
}
