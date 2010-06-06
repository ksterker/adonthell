/*
   Copyright (C) 2010 Kai Sterker <kai.sterker@gmail.com>
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

#include "base/logging.h"
#include <gtest/gtest.h>

#include "world/renderer.h"

namespace world
{
    class renderer_Test : public ::testing::Test, public default_renderer
    {
    protected:
        renderer_Test() 
        {
        }

        virtual ~renderer_Test() 
        {
        }

        // If the constructor and destructor are not enough for setting up
        // and cleaning up each test, you can define the following methods:

        virtual void SetUp() 
        {
            // Code here will be called immediately after the constructor (right
            // before each test).
        }

        virtual void TearDown() 
        {
            // Code here will be called immediately after each test (right
            // before the destructor).
        }        
    }; // class{}

    TEST_F(renderer_Test, is_object_below) 
    {
        cube3 c1 (vector3<s_int16>(0, 0, 0), vector3<s_int16>(12, 43, 100));
        cube3 c2 (vector3<s_int16>(0, 0, -5), vector3<s_int16>(128, 96, 0));
        
        placeable_shape s1;
        s1.add_part (&c1);
        
        placeable_shape s2;
        s2.add_part (&c2);
        
        render_info obj1 (&s1, NULL, vector3<s_int32>(250, 341, -100), NULL);
        render_info obj2 (&s2, NULL, vector3<s_int32>(256, 288, 0), NULL);
        
        EXPECT_EQ(true, is_object_below (obj1, obj2));
        EXPECT_EQ(false, is_object_below (obj2, obj1));
    }

} // namespace{}


int main(int argc, char **argv) 
{
    ::google::InitGoogleLogging(argv[0]);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
