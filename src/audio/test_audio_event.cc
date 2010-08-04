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
 * @file   test/audio/audio_event.cc
 * @author Josh Glover <jmglov@jmglov.net>
 * 
 * @brief  Unit tests for the audio_event class.
 * 
 * 
 */


#include "audio/audio_event.h"
#include "audio/test_sound.h"

#include <gtest/gtest.h>

namespace audio
{
    class audio_event_Test : public ::testing::Test {

    protected:
        audio_event_Test() {
            sound_noop::init();

            s  = new sound_noop("foo.ogg");
            ev = new audio_event(s);
        }

        virtual ~audio_event_Test() {
            delete ev;
            delete s;
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

        audio_event *ev;
        sound_noop  *s;
    }; // class{}

    TEST_F(audio_event_Test, constructor_Default) {
        EXPECT_EQ("audio/foo.ogg", ev->sample().getfilename());
    }

    TEST_F(audio_event_Test, put_state_Filename) {
        EXPECT_EQ("audio/foo.ogg", ev->sample().getfilename());

        base::flat f;
        ev->put_state(f);

        sound_noop  *s2  = new sound_noop("bar.ogg");
        audio_event *ev2 = new audio_event(s2);

        EXPECT_EQ("audio/bar.ogg", ev2->sample().getfilename());

        ev2->get_state(f);

        EXPECT_EQ("audio/foo.ogg", ev2->sample().getfilename());

        delete ev2;
        delete s2;
    }

} // namespace{}


int main(int argc, char **argv) {
    google::InitGoogleLogging(argv[0]);

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
