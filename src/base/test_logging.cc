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
 * @brief  Unit tests for logging functionality
 * 
 * 
 */


#include "base/logging.h"

#include <gtest/gtest.h>

#include <string>

namespace logging
{
    class logging_Test : public ::testing::Test {

    protected:
        logging_Test() {
        }

        virtual ~logging_Test() {
        }

        // If the constructor and destructor are not enough for setting up
        // and cleaning up each test, you can define the following methods:

        virtual void SetUp() {
            // Code here will be called immediately after the constructor (right
            // before each test).
        }

        virtual void TearDown() {
            // Code here will be called immediately after each test (right
            // before the destructor).
        }
    }; // class{}

    TEST_F(logging_Test, indent_Default) {
        EXPECT_EQ(std::string(0 * LOG_INDENT_NUM_COLUMNS, ' '), indent());
    }

    TEST_F(logging_Test, indent_IncrementOnce) {
        EXPECT_EQ(std::string(0 * LOG_INDENT_NUM_COLUMNS, ' '), indent());
        increment_log_indent_level();
        EXPECT_EQ(std::string(1 * LOG_INDENT_NUM_COLUMNS, ' '), indent());
    }

    TEST_F(logging_Test, indent_DecrementOnce) {
        EXPECT_EQ(std::string(1 * LOG_INDENT_NUM_COLUMNS, ' '), indent());
        decrement_log_indent_level();
        EXPECT_EQ(std::string(0 * LOG_INDENT_NUM_COLUMNS, ' '), indent());
    }

    TEST_F(logging_Test, indent_DecrementAtZero) {
        EXPECT_EQ(std::string(0 * LOG_INDENT_NUM_COLUMNS, ' '), indent());
        decrement_log_indent_level();
        EXPECT_EQ(std::string(0 * LOG_INDENT_NUM_COLUMNS, ' '), indent());
    }

} // namespace{}


int main(int argc, char **argv) {
    google::InitGoogleLogging(argv[0]);
    
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
