/*
   $Id: screen.cc,v 1.7 2009/04/25 13:17:50 ksterker Exp $

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
 * @file   test/audio/audio_manager.cc
 * @author Josh Glover <jmglov@jmglov.net>
 * 
 * @brief  Unit tests for the audio_manager class.
 * 
 * 
 */


#include "audio/audio_manager.h"

#include <gtest/gtest.h>

namespace audio
{
    class audio_manager_Test : public ::testing::Test {

    protected:
        audio_manager_Test() {
        }

        virtual ~audio_manager_Test() {
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

    TEST_F(audio_manager_Test, set_audio_rate_Default) {
        audio_manager::set_audio_rate();

        EXPECT_EQ(44100, audio_manager::get_audio_rate());
    }

    TEST_F(audio_manager_Test, set_audio_format_Default) {
        audio_manager::set_audio_format();

        EXPECT_EQ(AUDIO_S16, audio_manager::get_audio_format());
    }

    TEST_F(audio_manager_Test, set_audio_channels_Default) {
        audio_manager::set_audio_channels();

        EXPECT_EQ(2, audio_manager::get_audio_channels());
    }

    TEST_F(audio_manager_Test, set_audio_buffers_Default) {
        audio_manager::set_audio_buffers();

        EXPECT_EQ(4096, audio_manager::get_audio_buffers());
    }

    TEST_F(audio_manager_Test, set_audio_mixchannels_Default) {
        audio_manager::set_audio_mixchannels();

        EXPECT_EQ(16, audio_manager::get_audio_mixchannels());
    }

    TEST_F(audio_manager_Test, set_audio_rate) {
        audio_manager::set_audio_rate(27);

        EXPECT_EQ(27, audio_manager::get_audio_rate());
    }

    TEST_F(audio_manager_Test, set_audio_format) {
        audio_manager::set_audio_format(27);

        EXPECT_EQ(27, audio_manager::get_audio_format());
    }

    TEST_F(audio_manager_Test, set_audio_channels) {
        audio_manager::set_audio_channels(27);

        EXPECT_EQ(27, audio_manager::get_audio_channels());
    }

    TEST_F(audio_manager_Test, set_audio_buffers) {
        audio_manager::set_audio_buffers(27);

        EXPECT_EQ(27, audio_manager::get_audio_buffers());
    }

    TEST_F(audio_manager_Test, set_audio_mixchannels) {
        audio_manager::set_audio_mixchannels(27);

        EXPECT_EQ(27, audio_manager::get_audio_mixchannels());
    }

} // namespace{}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
