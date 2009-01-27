/*
   $Id: types.h,v 1.4 2009/01/27 21:24:21 fr3dc3rv Exp $

   Copyright (C) 1999/2000/2001/2002/2003 Alexandre Courbot.
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
 * @file base/types.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * @brief Declares some basic types.
 */


#ifndef BASE_TYPES_H
#define BASE_TYPES_H
#include <stdint.h>

// We are assuming CMAKE guarantees the existence of <stdint.h>

/// 32 bits long unsigned
    typedef uint32_t u_int32;

/// 32 bits long signed
    typedef int32_t s_int32;

/// 16 bits long unsigned
    typedef uint16_t u_int16;

/// 16 bits long signed
    typedef int16_t s_int16;

/// 8 bits long unsigned
    typedef uint8_t u_int8;

/// 8 bits long signed
    typedef int8_t s_int8;

#endif // BASE_TYPES_H
