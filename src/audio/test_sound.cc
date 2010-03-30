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
 * @file   test/audio/sound.cc
 * @author Josh Glover <jmglov@jmglov.net>
 * 
 * @brief  Unit tests for the sound class.
 * 
 * 
 */


#include "audio/sound.h"

#include <gtest/gtest.h>

// No-op to allow constructor to succeed
void *audio_open(const char *filename) {
    return NULL;
}

namespace audio
{
    class sound_noop : public sound {
    public:
        static void init() {
            sound::m_open = audio_open;
        }

        sound_noop(const std::string &filename) : sound(filename) {}
        sound_noop() {}

        void * get_sample()     { return m_sample;     }
        int    get_channel()    { return m_channel;    }
        bool   get_forcedhalt() { return m_forcedhalt; }
    };

    class sound_Test : public ::testing::Test {

    protected:
        sound_Test() {
            sound_noop::init();

            s = new sound_noop("foo.ogg");
        }

        virtual ~sound_Test() {
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

        sound_noop *s;
    }; // class{}

    TEST_F(sound_Test, constructor_Default) {
        EXPECT_EQ("game_sounds/foo.ogg", s->getfilename());
    }

    TEST_F(sound_Test, put_state_Filename) {
        EXPECT_EQ("game_sounds/foo.ogg", s->getfilename());

        base::flat f;
        s->put_state(f);

        sound_noop *s2 = new sound_noop();
        s2->get_state(f);

        EXPECT_EQ("game_sounds/foo.ogg", s2->getfilename());
        EXPECT_EQ(-1,                    s2->get_channel());

        EXPECT_FALSE(s2->get_forcedhalt());
    }

} // namespace{}


int main(int argc, char **argv) {
    google::InitGoogleLogging(argv[0]);

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
