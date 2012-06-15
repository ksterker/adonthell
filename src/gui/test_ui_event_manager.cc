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


#include "event/manager.h"
#include "gui/button.h"
#include "gui/ui_event.h"

#include <gtest/gtest.h>

namespace gui
{
    class ui_event_manager_Test : public ::testing::Test {

    protected:
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

    public:
        void increment_int(const events::event *event) {
            const ui_event *e = (const ui_event *) event;
            (*((int *)e->user_data()))++;
        }
    }; // class{}

    TEST_F(ui_event_manager_Test, listen_for_events_from_all_sources) {
        int test_value = 0;
        ui_event* event = new ui_event( (widget*)ui_event::ALL_SOURCES, "activate", &test_value);
        events::factory event_factory;

        event_factory.register_event(event, ::base::make_functor(*this, &ui_event_manager_Test::increment_int));

        gui::button* button = new gui::button(0, 0);
        button->activate();

        EXPECT_EQ(1, test_value);
    }

} // namespace{}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
