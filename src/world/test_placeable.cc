/*
   $Id: test_placeable.cc,v 1.7 2009/04/25 13:17:50 ksterker Exp $

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
 * @file   test/world/placeable.cc
 * @author Josh Glover <jmglov@jmglov.net>
 *
 * @brief  Unit tests for the placeable class.
 *
 *
 */


#include "world/placeable.h"
#include "world/area.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Return;

namespace world
{
    class placeable_Test : public ::testing::Test {

    protected:
        placeable_Test() : object(nowhere) {
        }

        virtual ~placeable_Test() {
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

        area nowhere; //Need mock object?
        placeable object;

        //Helper function to make it easy to make cubes
        void AddCube(s_int16 x1, s_int16 y1, s_int16 z1, s_int16 x2, s_int16 y2, s_int16 z2, bool solid = false) {
            placeable_model *model = new placeable_model;
            placeable_shape *shape = model->add_shape("default");
            shape->add_part(new cube3(vector3<s_int16>(x1,y1,z1), vector3<s_int16>(x2,y2,z2)));
            shape->set_solid(solid);
            object.add_model(model);
        }
    }; // class{}

    TEST_F(placeable_Test, blankObject) {
        EXPECT_EQ(vector3<s_int16>(0,0,0), object.entire_min());
        EXPECT_EQ(vector3<s_int16>(0,0,0), object.entire_max());
        EXPECT_EQ(vector3<s_int16>(0,0,0), object.solid_min());
        EXPECT_EQ(vector3<s_int16>(0,0,0), object.solid_max());
    }

    TEST_F(placeable_Test, simpleObject) {
        AddCube(0,0,0,10,10,10);
        EXPECT_EQ(vector3<s_int16>(0,0,0), object.entire_min());
        EXPECT_EQ(vector3<s_int16>(10,10,10), object.entire_max());
        EXPECT_EQ(vector3<s_int16>(0,0,0), object.solid_min());
        EXPECT_EQ(vector3<s_int16>(0,0,0), object.solid_max());
    }

    TEST_F(placeable_Test, oneNegativeObject) {
        AddCube(0,0,-10,10,10,0);
        EXPECT_EQ(vector3<s_int16>(0,0,-10), object.entire_min());
        EXPECT_EQ(vector3<s_int16>(10,10,10), object.entire_max());
        EXPECT_EQ(vector3<s_int16>(0,0,0), object.solid_min());
        EXPECT_EQ(vector3<s_int16>(0,0,0), object.solid_max());
    }

    TEST_F(placeable_Test, threeNegativeObject) {
        AddCube(-10,-10,-10, 0, 0, 0);
        EXPECT_EQ(vector3<s_int16>(-10,-10,-10), object.entire_min());
        EXPECT_EQ(vector3<s_int16>(10,10,10), object.entire_max());
        EXPECT_EQ(vector3<s_int16>(0,0,0), object.solid_min());
        EXPECT_EQ(vector3<s_int16>(0,0,0), object.solid_max());
    }

    TEST_F(placeable_Test, oneOffsetObject) {
        AddCube(0,0,5,10,10,10);
        EXPECT_EQ(vector3<s_int16>(0,0,5), object.entire_min());
        EXPECT_EQ(vector3<s_int16>(10,10,5), object.entire_max());
        EXPECT_EQ(vector3<s_int16>(0,0,0), object.solid_min());
        EXPECT_EQ(vector3<s_int16>(0,0,0), object.solid_max());
    }

    TEST_F(placeable_Test, threeOffsetObject) {
        AddCube(5,5,5,10,10,10);
        EXPECT_EQ(vector3<s_int16>(5,5,5), object.entire_min());
        EXPECT_EQ(vector3<s_int16>(5,5,5), object.entire_max());
        EXPECT_EQ(vector3<s_int16>(0,0,0), object.solid_min());
        EXPECT_EQ(vector3<s_int16>(0,0,0), object.solid_max());
    }

    TEST_F(placeable_Test, oneMoreNegativeObject) {
        AddCube(0,0,-10,10,10,-5);
        EXPECT_EQ(vector3<s_int16>(0,0,-10), object.entire_min());
        EXPECT_EQ(vector3<s_int16>(10,10,5), object.entire_max());
        EXPECT_EQ(vector3<s_int16>(0,0,0), object.solid_min());
        EXPECT_EQ(vector3<s_int16>(0,0,0), object.solid_max());
    }

    TEST_F(placeable_Test, threeMoreNegativeObject) {
        AddCube(-10,-10,-10, -5, -5, -5);
        EXPECT_EQ(vector3<s_int16>(-10,-10,-10), object.entire_min());
        EXPECT_EQ(vector3<s_int16>(5,5,5), object.entire_max());
        EXPECT_EQ(vector3<s_int16>(0,0,0), object.solid_min());
        EXPECT_EQ(vector3<s_int16>(0,0,0), object.solid_max());
    }

    TEST_F(placeable_Test, simpleCompoundObject) {
        AddCube(0,0,0,5,5,5);
        AddCube(5,5,5,10,10,10);
        EXPECT_EQ(vector3<s_int16>(0,0,0), object.entire_min());
        EXPECT_EQ(vector3<s_int16>(10,10,10), object.entire_max());
        EXPECT_EQ(vector3<s_int16>(0,0,0), object.solid_min());
        EXPECT_EQ(vector3<s_int16>(0,0,0), object.solid_max());
    }

    TEST_F(placeable_Test, stairCompoundObject) {
        AddCube(8, 0, 112, 24, 48, 120);
        AddCube(24, 0, 96, 40, 48, 104);
        AddCube(40, 0, 80, 56, 48, 88);
        AddCube(56, 0, 64, 72, 48, 72);
        AddCube(72, 0, 48, 88, 48, 56);
        AddCube(88, 0, 32, 104, 48, 40);
        AddCube(104, 0, 16, 120, 48, 24);
        AddCube(120, 0, 0, 136, 48, 8);
        AddCube(112, 0, 8, 128, 48, 16);
        AddCube(96, 0, 24, 112, 48, 32);
        AddCube(80, 0, 40, 96, 48, 48);
        AddCube(64, 0, 56, 80, 48, 64);
        AddCube(48, 0, 72, 64, 48, 80);
        AddCube(32, 0, 88, 48, 48, 96);
        AddCube(16, 0, 104, 32, 48, 112);
        AddCube(0, 0, 120, 16, 48, 128);
        EXPECT_EQ(vector3<s_int16>(0,0,0), object.entire_min());
        EXPECT_EQ(vector3<s_int16>(136,48,128), object.entire_max());
        EXPECT_EQ(vector3<s_int16>(0,0,0), object.solid_min());
        EXPECT_EQ(vector3<s_int16>(0,0,0), object.solid_max());
    }

    TEST_F(placeable_Test, simpleSolidObject) {
        AddCube(0,0,0,10,10,10,true);
        EXPECT_EQ(vector3<s_int16>(0,0,0), object.entire_min());
        EXPECT_EQ(vector3<s_int16>(10,10,10), object.entire_max());
        EXPECT_EQ(vector3<s_int16>(0,0,0), object.solid_min());
        EXPECT_EQ(vector3<s_int16>(10,10,10), object.solid_max());
    }

    TEST_F(placeable_Test, simpleSolidPartialObject) {
        AddCube(0,0,-5,40,40,5);
        AddCube(10,10,5,20,20,20,true);
        EXPECT_EQ(vector3<s_int16>(0,0,-5), object.entire_min());
        EXPECT_EQ(vector3<s_int16>(40,40,25), object.entire_max());
        EXPECT_EQ(vector3<s_int16>(10,10,5), object.solid_min());
        EXPECT_EQ(vector3<s_int16>(10,10,15), object.solid_max());
    }

} // namespace{}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
