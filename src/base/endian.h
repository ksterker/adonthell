/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

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

/* Use inline functions for compilers that support them, and static
   functions for those that do not.  Because these functions become
   static for compilers that do not support inline functions, this
   header should only be included in files that actually use them.
*/
#if defined(__GNUC__) && defined(__i386__)
static __inline__ u_int16 SDL_Swap16(u_int16 x)
{
	__asm__("xchgb %b0,%h0" : "=q" (x) :  "0" (x));
	return x;
}
#elif defined(__GNUC__) && defined(__x86_64__)
static __inline__ u_int16 SDL_Swap16(u_int16 x)
{
	__asm__("xchgb %b0,%h0" : "=q" (x) :  "0" (x));
	return x;
}
#elif defined(__GNUC__) && (defined(__powerpc__) || defined(__ppc__))
static __inline__ u_int16 SDL_Swap16(u_int16 x)
{
	u_int16 result;

	__asm__("rlwimi %0,%2,8,16,23" : "=&r" (result) : "0" (x >> 8), "r" (x));
	return result;
}
#else
static __inline__ u_int16 SDL_Swap16(u_int16 x) {
	return((x<<8)|(x>>8));
}
#endif

#if defined(__GNUC__) && defined(__i386__)
static __inline__ u_int32 SDL_Swap32(u_int32 x)
{
	__asm__("bswap %0" : "=r" (x) : "0" (x));
	return x;
}
#elif defined(__GNUC__) && defined(__x86_64__)
static __inline__ u_int32 SDL_Swap32(u_int32 x)
{
	__asm__("bswapl %0" : "=r" (x) : "0" (x));
	return x;
}
#elif defined(__GNUC__) && (defined(__powerpc__) || defined(__ppc__))
static __inline__ u_int32 SDL_Swap32(u_int32 x)
{
	u_int32 result;

	__asm__("rlwimi %0,%2,24,16,23" : "=&r" (result) : "0" (x>>24), "r" (x));
	__asm__("rlwimi %0,%2,8,8,15"   : "=&r" (result) : "0" (result),    "r" (x));
	__asm__("rlwimi %0,%2,24,0,7"   : "=&r" (result) : "0" (result),    "r" (x));
	return result;
}
#else
static __inline__ u_int32 SDL_Swap32(u_int32 x) {
	return((x<<24)|((x<<8)&0x00FF0000)|((x>>8)&0x0000FF00)|(x>>24));
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
