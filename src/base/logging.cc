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
 * @file   base/logging.cc
 * @author Josh Glover <jmglov@jmglov.net>
 *
 * @brief  Defines basic logging functions
 *
 */

#include "base/logging.h"

#include <string>

/**
 * Log indent level
 *
 */
static u_int8 log_indent_level = 0;


namespace logging
{
    const std::string indent() {
        return std::string(log_indent_level * LOG_INDENT_NUM_COLUMNS, ' ');
    }

    const u_int8 decrement_log_indent_level() {
        log_indent_level = log_indent_level > 0 ? log_indent_level - 1 : 0;

        return log_indent_level;
    }

    const u_int8 increment_log_indent_level() {
        return ++log_indent_level;
    }

} // namespace{}
