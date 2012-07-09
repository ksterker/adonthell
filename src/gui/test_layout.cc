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
 * @file   src/gui/test_conversation.cc
 * @author Josh Glover <jmglov@jmglov.net>
 * 
 * @brief  Unit tests for the gui::conversation class.
 * 
 * 
 */


#include "event/manager.h"
#include "input/keyboard_event.h"
#include "gui/layout.h"
#include "gui/ui_event.h"
#include "gui/ui_event_manager.h"
#include "gui/widget.h"

#include <gtest/gtest.h>
#include "../input/keyboard_event.h"

namespace gui
{
    class test_widget : public widget {
    protected:
        bool focus() {
            return true;
        }
    };

    class layout_Test : public ::testing::Test {

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

    void press(input::keyboard_event::key_type key, layout *l, events::manager_base *manager) {
        input::keyboard_event *left = new input::keyboard_event(input::keyboard_event::KEY_PUSHED, key, "");
        l->on_keyboard_event(left);
        ((ui_event_manager *)manager)->update();
    }

    TEST_F(layout_Test, switching_conversation_options_fires_switch_event) {
        layout *l = new layout(2, 2);

        test_widget w;
        l->add_child(w, 0, 0);
        l->add_child(w, 0, 0);
        l->add_child(w, 0, 0);
        l->add_child(w, 0, 0);

        int test_value = 0;
        ui_event* event = new ui_event( (widget*)ui_event::ANY_SOURCE, "layout_switch", &test_value);
        events::factory event_factory;

        event_factory.register_event(event, ::base::make_functor(*this, &layout_Test::increment_int));
        events::manager_base *manager = events::event_type::get_manager (((events::event *) event)->type ());

        press(input::keyboard_event::LEFT_KEY, l, manager);
        EXPECT_EQ(1, test_value);

        press(input::keyboard_event::RIGHT_KEY, l, manager);
        EXPECT_EQ(2, test_value);

        press(input::keyboard_event::DOWN_KEY, l, manager);
        EXPECT_EQ(3, test_value);

        press(input::keyboard_event::UP_KEY, l, manager);
        EXPECT_EQ(4, test_value);
    }

} // namespace{}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
