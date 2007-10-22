/*
 $Id: diskwriter_base.h,v 1.2 2007/10/22 02:19:46 ksterker Exp $
 
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

#include "base/flat.h"

namespace base {
    /**
     *
     */
    class disk_writer_base
    {
    public:
	
		/**
		 *
		 */
		virtual ~disk_writer_base () {}
		
        /*
         *
         */
        virtual bool put_state (const std::string & name, base::flat & data) const = 0;
        
        /**
         *
         */
        virtual bool get_state (const std::string & name, base::flat & data) const = 0;

#ifndef SWIG
        /// make this class available to python::pass_instance
        GET_TYPE_NAME_ABSTRACT(base::disk_writer_base)
#endif // SWIG
    };
}

#endif // BASE_DISKWRITER_BASE
