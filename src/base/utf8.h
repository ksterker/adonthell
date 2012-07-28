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
 * @file base/utf8.h 
 * @author Kai Sterker <kaisterker@linuxgames.com>
 *
 * @brief Internal helper class for UTF-8 encoded strings.
 */


#ifndef BASE_UTF8_H
#define BASE_UTF8_H

#include <string>
#include "types.h"

namespace base
{
  
/**
 * Helper class for dealing with UTF-8 encoded strings.
 * Uses code borrowed from grub (v1.95).
 */
class utf8
{
public:
    /**
     * @name Conversion
     */
    //@{
    /**
     * Convert an UTF-16 character to UTF-8.
     * @param chr a character in UTF-16.
     * @return UTF-8 representation.
     */
    static std::string from_utf16 (const u_int16 & chr);

    /**
     * Convert the UTF-8 character pointed to by iter
     * to UTF-32, moving iter to the next UTF-8 character.
     * @param source the source string in UTF-8 format.
     * @param iter iterator pointing into source.
     * @return UTF-32 representation.
     */
    static u_int32 to_utf32 (const std::string & source, std::string::const_iterator & iter);
    //@}
    
    /**
     * @name Iteration
     */
    //@{
    /**
     * Return the number of bytes the character before pos occupies.
     * @param source the string in UTF-8 encoding.
     * @param pos valid character index into the source string.
     * @return size of character to the left, 0 at start of string.
     */
    static u_int32 left (const std::string & source, const u_int32 & pos);
    
    /**
     * Return the number of bytes the character at pos occupies.
     * @param source the string in UTF-8 encoding.
     * @param pos valid character index into the source string.
     * @return size of character to the right, 0 at end of string.
     */
    static u_int32 right (const std::string & source, const u_int32 & pos);    
    //@}
private:
    /// buffer for converting UTF-16 surrogate pair to UTF-8
    static u_int32 CodeHigh;
};
    
}

#endif
