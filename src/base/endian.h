/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997, 1998, 1999, 2000, 2001, 2002  Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

/* Functions for reading and writing endian-specific values */

#ifndef BASE_ENDIAN_H
#define BASE_ENDIAN_H

/* These functions read and write data of the specified endianness, 
   dynamically translating to the host machine endianness. */

#include "config.h"
#include "base/types.h"

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/* The macros used to swap values */
/* Try to use superfast macros on systems that support them */
#ifdef HAVE_ASM_BYTEORDER_H
#include <asm/byteorder.h>
#ifdef __arch__swab16
#define Swap16  __arch__swab16
#endif
#ifdef __arch__swab32
#define Swap32  __arch__swab32
#endif
#endif // HAVE_ASM_BYTEORDER_H
/* Use inline functions for compilers that support them, and static
   functions for those that do not.  Because these functions become
   static for compilers that do not support inline functions, this
   header should only be included in files that actually use them.
*/
#ifndef Swap16
static __inline__ u_int16 Swap16(u_int16 D) {
	return((D<<8)|(D>>8));
}
#endif
#ifndef Swap32
static __inline__ u_int32 Swap32(u_int32 D) {
	return((D<<24)|((D<<8)&0x00FF0000)|((D>>8)&0x0000FF00)|(D>>24));
}
#endif

/* Byteswap item from the specified endianness to the native endianness */
#ifdef BIG_ENDIAN
#define SwapLE16(X)	(X)
#define SwapLE32(X)	(X)
#define SwapBE16(X)	Swap16(X)
#define SwapBE32(X)	Swap32(X)
#else
#define SwapLE16(X)	Swap16(X)
#define SwapLE32(X)	Swap32(X)
#define SwapBE16(X)	(X)
#define SwapBE32(X)	(X)
#endif

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#endif // BASE_ENDIAN_H
