/*
   $Id: diskio.h,v 1.3 2004/08/02 07:35:28 ksterker Exp $

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
     * This class provides a file writing/loading interface to the data flattener.
     * It allows to write a flat object to disk, including a checksum for error 
     * check when loading again later.
     */
    class diskio : public flat {
        public:
            /**
             * Constructor.
             */
            diskio ();

            /**
             * Load this record from the given file. After loading, it will
             * compare the checksum read from file with the one computed from
             * the data read. It thus can detect data corruption.
             * @param in file stream to read data from.
             * @return \b true on successful loading, \b false otherwise.
             */
            bool get_record (igzstream & in);

            /**
             * Save this record to the given file. This also saves a short header
             * (the length of the record) and a checksum of the record to file.
             * So multiple records can be put into a single file and easily 
             * recovered later.
             * @param out file stream to save record to.
             */
            void put_record (ogzstream & out);
            
#ifndef SWIG
            /// make this class available to python::pass_instance
            GET_TYPE_NAME(base::diskio)
#endif
    };
}

#endif // BASE_DISKIO
