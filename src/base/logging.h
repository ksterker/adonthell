/*
   Copyright (C) 2010 Josh Glover <jmglov@jmglov.net>
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
 * @file   base/logging.h
 * @author Josh Glover <jmglov@jmglov.net>
 *
 * @brief  Defines the logging module functions.
 *
 */

#ifndef LOGGING_H_
#define LOGGING_H_

#include <string>

#if HAVE_GLOG_H
#include <glog/logging.h>
#else
// Mock up glog...
#include <iostream>
namespace google
{
    inline void InitGoogleLogging(char *ignore) { }

    class LogMessageVoidify {
    public:
        LogMessageVoidify() { }
        // This has to be an operator with a precedence lower than << but
        // higher than ?:
        void operator&(std::ostream&) { }
    };
}

#define LOG(x) true ? (void) 0 : google::LogMessageVoidify() & std::cout

#endif

#include "base/types.h"

/**
 * This module defines basic logging functions
 *
 */
namespace logging
{
    const u_int8 LOG_INDENT_NUM_COLUMNS = 2;

    /**
     * @param Log message indentation, as a string
     */
    const std::string indent();

    /**
     * @return New log message indent level
     */
    const u_int8 decrement_log_indent_level();

    /**
     * @return New log message indent level
     */
    const u_int8 increment_log_indent_level();

} // namespace{}

#endif
