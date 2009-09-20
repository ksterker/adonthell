/*
   $Id: diskio.h,v 1.12 2009/03/21 11:59:46 ksterker Exp $

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
 * @file base/diskio.h 
 * @author Kai Sterker <kaisterker@linuxgames.com>
 *
 * @brief More secure I/O classes for writing to/reading from file.
 */

#ifndef BASE_DISKIO
#define BASE_DISKIO

#include "base/diskwriter_base.h"

namespace base {
    /**
     * This class provides a file writing/loading interface to the data flattener.
     * It allows to write a flat object to disk, including a checksum for error 
     * check when loading again later.
     */
    class diskio : public flat 
    {
        public:
            /**
             * Available file writers.
             */
            typedef enum
            {
                GZ_FILE,
                XML_FILE,
                BY_EXTENSION
            } file_format;
        
            /**
             * Constructor.
             * @param format File format to use for disk i/o.
             */
            diskio (const diskio::file_format & format = BY_EXTENSION);

            /**
             * Destructor.
             */
            ~diskio ();

            /**
             * @name I/O Methods
             */
            //@{
            /**
             * Load this record from the given file, using the specified file
             * type. After reading the file, a checksum comparison takes place,
             * resulting in an error if the checksum did not match. Data will
             * be read regardless of checksum errors, and it lies in the application's
             * responsibility whether it continues working with possibly corrupt data.
             * 
             * @param filename file to read data from.
             * @return \b true on successful loading, \b false otherwise.
             */
            bool get_record (const std::string & filename);
            
            /**
             * Save this record to given file, using the specified file type. 
			 * A checksum will be saved together with file.
			 * 
             * @param filename file to save record to.
             * @return \b true on success, \b false otherwise.
             */
            bool put_record (const std::string & filename);
            //@}
#ifndef SWIG
            /// make this class available to python::pass_instance
            GET_TYPE_NAME(base::diskio)
                
        private:
            /**
             * Determine file format by file extension. File names ending in '.xml'
             * are treated as XML files, all others as binary.
             *
             * @param filename file to load or save.
             */
            void get_writer_for_extension (const std::string & filename);

                
            /// writer to use for i/o operations
            base::disk_writer_base *Writer;
#endif
    };
}

#endif // BASE_DISKIO
