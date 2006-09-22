/*
   $Id: diskio.h,v 1.7 2006/09/22 01:15:22 ksterker Exp $

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

#include <libxml/tree.h>

#include "base/flat.h"
#include "base/file.h"

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
             * @name Binary file IO
             */
            //@{
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
            //@}
            
            /**
             * @name XML file IO
             */
            //@{
            /**
             * Parser states.
             */
            enum { UNDEF, DATA, LIST, PARAM };
            
            /**
             * Load this record from the given XML file. Compared to get_record(),
             * no checksum is kept in the XML file, so file corruption cannot be
             * detected prior to loading.
             * @param filename file to read data from.
             * @return \b true on successful loading, \b false otherwise.
             * @sa get_record()
             */
            bool get_xml (const std::string & filename);
            
            /**
             * Save this record to an XML file. Compared to put_record(), no
             * checksum will be saved with the record. Multiple records can be
             * saved in the same file.
             * @param filename file to save record to.
             * @return true on success, false otherwise.
             * @sa put_record()
             */
            bool put_xml (const std::string & filename);
            //@}
#ifndef SWIG
            /// make this class available to python::pass_instance
            GET_TYPE_NAME(base::diskio)
    private:
    		/**
    		 * Add the given record as child to the given xml element.
    		 * @param record record to serialize to XML structure.
    		 * @param parent node to which record should be added.
    		 */
            void record_to_xml (base::flat &record, xmlNodePtr parent) const;
            
            /**
             * Convert the given value to an xml character string.
             * @param type data type of given value.
             * @param value data to convert.
             * @param size length of data.
             * @return string representation of given value.
             */ 
            xmlChar *value_to_xmlChar (const flat::data_type & type, void *value, const u_int32 & size) const;
            
            /// Table storing hex characters
            static char *Bin2Hex;
#endif
    };
}

#endif // BASE_DISKIO
