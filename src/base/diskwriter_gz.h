/*
 $Id: diskwriter_gz.h,v 1.1 2006/09/22 05:13:16 ksterker Exp $
 
 Copyright (C) 2006 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file base/diskwriter_gz.h 
 * @author Kai Sterker <kaisterker@linuxgames.com>
 *
 * @brief Read/write gz compressed data files.
 */

#ifndef BASE_DISKWRITER_GZ
#define BASE_DISKWRITER_GZ

#include "diskwriter_base.h"

namespace base {
    
    /**
     * This class provides a file writing/loading interface to the data flattener.
     * It allows to write a flat object to a gz compressed file, including a checksum 
     * for detecting data corruption when loading again later.
     */
    class disk_writer_gz : public disk_writer_base
    {
    public:
        /**
         * Save given record to file. This also saves a short header
         * (the length of the record) and a checksum of the record.
         * @param name file to save record to.
         * @param data record to save to file.
         * @return \b true on success, \b false otherwise.
         */
        bool put_state (const std::string & name, base::flat & data) const;
        
        /**
         * Initialize record from the given file. After loading, it will
         * compare the checksum read from file with the one computed from
         * the data read. It thus can detect data corruption.
         * @param name file to read data from.
         * @param data empty record to fill from file.
         * @return \b true on successful loading, \b false otherwise.
         */
        bool get_state (const std::string & name, base::flat & data) const;

#ifndef SWIG
        /// make this class available to python::pass_instance
        GET_TYPE_NAME(base::disk_writer_gz)
#endif // SWIG
    };
}

#endif // BASE_DISKWRITER_GZ
