/*
   $Id: types.h,v 1.2 2003/11/22 09:35:21 ksterker Exp $

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

 
#ifndef TYPES_H__
#define TYPES_H__


// We should put ifdefs here to put the correct int values accordingly to the system type


/// 8 bits long unsigned integer
#define u_int8 unsigned char

/// 16 bits long unsigned integer
#define u_int16 unsigned short

/// 32 bits long unsigned integer
#define u_int32 unsigned int

/// 8 bits long signed integer
#define s_int8 signed char

/// 16 bits long signed integer
#define s_int16 signed short

/// 32 bits long signed integer
#define s_int32 signed int

#endif
