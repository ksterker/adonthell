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
            cube3 *c1 = new cube3 (vector3<s_int16>(0, 0, -5), vector3<s_int16>(128, 96, 0));
            cube3 *c2 = new cube3 (vector3<s_int16>(0, 0, 0), vector3<s_int16>(128, 12, 100));
            
            s1.add_part (c1);
            s2.add_part (c2);
        }

        virtual void TearDown() 
        {
            // Code here will be called immediately after each test (right
            // before the destructor).
        }
        
        placeable_shape s1;
        placeable_shape s2;
    }; // class{}

    TEST_F(renderer_Test, is_object_below_1)
    {
        render_info obj1 (&s1, NULL, vector3<s_int32>(-256, 192, 0), NULL);
        render_info obj2 (&s2, NULL, vector3<s_int32>(-256, 224, -100), NULL);
        
        EXPECT_EQ(false, is_object_below (obj1, obj2));
        EXPECT_EQ(true, is_object_below (obj2, obj1));
    }

    TEST_F(renderer_Test, is_object_below_2)
    {
        render_info obj1 (&s1, NULL, vector3<s_int32>(-256, 192, 0), NULL);
        render_info obj2 (&s2, NULL, vector3<s_int32>(-256, 280, -100), NULL);
        
        EXPECT_EQ(false, is_object_below (obj1, obj2));
        EXPECT_EQ(true, is_object_below (obj2, obj1));
    }

    TEST_F(renderer_Test, is_object_below_3)
    {
        render_info obj1 (&s1, NULL, vector3<s_int32>(-256, 192, 0), NULL);
        render_info obj2 (&s2, NULL, vector3<s_int32>(-256, 196, -100), NULL);
        
        EXPECT_EQ(false, is_object_below (obj1, obj2));
        EXPECT_EQ(true, is_object_below (obj2, obj1));
    }

    TEST_F(renderer_Test, is_object_below_4)
    {
        render_info obj1 (&s1, NULL, vector3<s_int32>(-256, 192, 0), NULL);
        render_info obj2 (&s2, NULL, vector3<s_int32>(-256, 182, -5), NULL);
        
        EXPECT_EQ(true, is_object_below (obj1, obj2));
        EXPECT_EQ(false, is_object_below (obj2, obj1));
    }

    TEST_F(renderer_Test, is_object_below_5)
    {
        render_info obj1 (&s1, NULL, vector3<s_int32>(-256, 192, 0), NULL);
        render_info obj2 (&s2, NULL, vector3<s_int32>(-256, 280, -5), NULL);
        
        EXPECT_EQ(true, is_object_below (obj1, obj2));
        EXPECT_EQ(false, is_object_below (obj2, obj1));
    }

    TEST_F(renderer_Test, is_object_below_6)
    {
        render_info obj1 (&s1, NULL, vector3<s_int32>(-256, 192, 0), NULL);
        render_info obj2 (&s2, NULL, vector3<s_int32>(-256, 224, -5), NULL);
        
        EXPECT_EQ(true, is_object_below (obj1, obj2));
        EXPECT_EQ(false, is_object_below (obj2, obj1));
    }
/*
    TEST_F(renderer_Test, is_object_below_7)
    {
        render_info obj1 (&s1, NULL, vector3<s_int32>(-256, 192, 0), NULL);
        render_info obj2 (&s2, NULL, vector3<s_int32>(-256, 224, -100), NULL);
        
        EXPECT_EQ(true, is_object_below (obj1, obj2));
        EXPECT_EQ(false, is_object_below (obj2, obj1));
    }
*/    
} // namespace{}


int main(int argc, char **argv) 
{
    ::google::InitGoogleLogging(argv[0]);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
