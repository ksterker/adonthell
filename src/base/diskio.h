/*
   $Id: diskio.h,v 1.2 2004/06/27 11:20:57 ksterker Exp $

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
 * @file base/diskio.h 
 * @author Kai Sterker <kaisterker@linuxgames.com>
 *
 * @brief More secure I/O classes for writing to/reading from file.
 */

#ifndef BASE_DISKIO
#define BASE_DISKIO

#include "base/flat.h"
#include "base/file.h"
#include "python/callback_support.h"

namespace base {
    /**
     *
     */
    class diskio : public flat {
        public:
            diskio ();
        
            bool get_record (igzstream & in);
            void put_record (ogzstream & out);
#ifndef SWIG
            GET_TYPE_NAME(base::diskio)
#endif
    };
}

#endif // BASE_DISKIO
