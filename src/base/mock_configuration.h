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
 * @file 	base/mock_configuration.h 
 * @author 	Josh Glover <jmglov@jmglov.net>
 *
 * @brief 	Mocks configuration class
 */

#ifndef BASE_MOCK_CONFIGURATION_H
#define BASE_MOCK_CONFIGURATION_H

#include <gmock/gmock.h>

namespace base
{

    class mock_configuration : public configuration {
    public:
        MOCK_CONST_METHOD3(option, cfg_option *(const string & section, const string & option, const u_int8 & type));
    }; // class{}

} // namespace{}

#endif // BASE_MOCK_CONFIGURATION_H
