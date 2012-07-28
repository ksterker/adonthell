/*
   $Id: configio.h,v 1.3 2005/06/03 17:29:13 ksterker Exp $

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
 * @file 	base/configio.h
 * @author 	Kai Sterker <kaisterker@linuxgames.com>
 *
 * @brief 	Loads and saves configuration file.
 */

#ifndef BASE_CONFIG_IO_H
#define BASE_CONFIG_IO_H

// low-level stuff swig/python needs not know about
#ifndef SWIG

#include "configuration.h"

namespace base {
    /**
     * Encapsulates reading and writing the XML configuration file.
     * Reading is done via a custom SAX parser. For writing, the xml
     * xml document is created in memory first and then saved to file.
     */
    class config_io
    {
        public:
            /**
             * Parser states.
             */
            enum { UNDEF, START, SECTION, OPTION };
        
            /**
             * Read a configuration from file.
             * @param filename name of configuration file without path and suffix.
             * @param config configuration to load given file into.
             * @return \c true on success, \c false otherwise
             */
            static bool read (const std::string & filename, base::configuration *config);

            /**
             * Write configuration to file.
             * @param filename name of configuration file without path and suffix.
             * @param config configuration to write to given file.
             * @return \c true on success, \c false otherwise
             */
            static bool write (const std::string & filename, const base::configuration *config);
    };
}

#endif // SWIG
#endif // BASE_CONFIG_IO_H
