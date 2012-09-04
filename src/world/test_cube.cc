/*
   Copyright (C) 2012 Kai Sterker <kai.sterker@gmail.com>
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

#include <adonthell/base/logging.h>
#include <gtest/gtest.h>

#include "cube3.h"

namespace world
{
    class cube_Test : public ::testing::Test, public world::cube3
    {
    protected:
        cube_Test() : world::cube3(1, 1, 1)
        {
        }

        virtual ~cube_Test()
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

    TEST_F(cube_Test, checkCreateMesh)
    {
        this->create_mesh();

        EXPECT_EQ(12, Surface.size());

        // top
        EXPECT_EQ(world::vector3<s_int32>(0,0,1), Surface[0]->normal());
        EXPECT_EQ(world::vector3<s_int32>(0,0,1), Surface[1]->normal());

        // bottom
        EXPECT_EQ(world::vector3<s_int32>(0,0,-1), Surface[2]->normal());
        EXPECT_EQ(world::vector3<s_int32>(0,0,-1), Surface[3]->normal());

        // front
        EXPECT_EQ(world::vector3<s_int32>(0,-1,0), Surface[4]->normal());
        EXPECT_EQ(world::vector3<s_int32>(0,-1,0), Surface[5]->normal());

        // back
        EXPECT_EQ(world::vector3<s_int32>(0,1,0), Surface[6]->normal());
        EXPECT_EQ(world::vector3<s_int32>(0,1,0), Surface[7]->normal());

        // left
        EXPECT_EQ(world::vector3<s_int32>(-1,0,0), Surface[8]->normal());
        EXPECT_EQ(world::vector3<s_int32>(-1,0,0), Surface[9]->normal());

        // right
        EXPECT_EQ(world::vector3<s_int32>(1,0,0), Surface[10]->normal());
        EXPECT_EQ(world::vector3<s_int32>(1,0,0), Surface[11]->normal());
    }

} // namespace{}


int main(int argc, char **argv) 
{
    ::google::InitGoogleLogging(argv[0]);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
