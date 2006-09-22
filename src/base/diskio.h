/*
   $Id: diskio.h,v 1.8 2006/09/22 05:13:16 ksterker Exp $

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
                GZ_WRITER,
                XML_WRITER,
            } disk_writer;
        
            /**
             * Constructor.
             * @param writer File writer to use for disk i/o.
             */
            diskio (const diskio::disk_writer & writer);

            /**
             * Destructor.
             */
            ~diskio ();

            /**
             * @name I/O Methods
             */
            //@{
            /**
             * Load this record from the given XML file. Compared to get_record(),
             * no checksum is kept in the XML file, so file corruption cannot be
             * detected prior to loading.
             * @param filename file to read data from.
             * @return \b true on successful loading, \b false otherwise.
             */
            bool get_record (const std::string & filename);
            
            /**
             * Save this record to an XML file. Compared to put_record(), no
             * checksum will be saved with the record. Multiple records can be
             * saved in the same file.
             * @param filename file to save record to.
             * @return \b true on success, \b false otherwise.
             */
            bool put_record (const std::string & filename);
            //@}
#ifndef SWIG
            /// make this class available to python::pass_instance
            GET_TYPE_NAME(base::diskio)
                
        private:
            /// writer to use for i/o operations
            base::disk_writer_base *Writer;
#endif
    };
}

#endif // BASE_DISKIO
