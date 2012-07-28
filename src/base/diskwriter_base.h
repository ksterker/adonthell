/*
 $Id: diskwriter_base.h,v 1.3 2008/04/14 11:03:08 ksterker Exp $
 
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
 * @file base/diskwriter_base.h 
 * @author Kai Sterker <kaisterker@linuxgames.com>
 *
 * @brief Base class for file I/O.
 */

#ifndef BASE_DISKWRITER_BASE
#define BASE_DISKWRITER_BASE

#include "flat.h"

namespace base {
    /**
     * Base class for saving structured data to or loading it back from a file. The type of
     * file will be determined by the actual implementation. It might be an XML file or a
     * compressed binary file.
     */
    class disk_writer_base
    {
    public:
	
		/**
		 * Destructor.
		 */
		virtual ~disk_writer_base () {}
		
        /*
         * Save given record in file with give name.
         * @param name file name
         * @param data data to save
         * @return \b true on success, \b false otherwise.
         */
        virtual bool put_state (const std::string & name, base::flat & data) const = 0;
        
        /**
         * Load content of file into given record.
         * @param name file name
         * @param data container for data to load
         * @return \b true on success, \b false otherwise.
         */
        virtual bool get_state (const std::string & name, base::flat & data) const = 0;

#ifndef SWIG
        /// make this class available to python::pass_instance
        GET_TYPE_NAME_ABSTRACT(base::disk_writer_base)
#endif // SWIG
    };
}

#endif // BASE_DISKWRITER_BASE
