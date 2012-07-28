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

#include <adonthell/base/logging.h>
#include <gtest/gtest.h>

#include "python.h"

namespace python
{
    class python_Test : public ::testing::Test
    {
    protected:
        python_Test() 
        {
            python::init();
        }

        virtual ~python_Test() 
        {
            python::cleanup();
        }

        // If the constructor and destructor are not enough for setting up
        // and cleaning up each test, you can define the following methods:

        virtual void SetUp() 
        {
        }

        virtual void TearDown() 
        {
        }
    }; // class{}

    TEST_F(python_Test, pad_tuple_1)
    {
        PyObject *no_tuple = PyList_New(1);
        EXPECT_EQ(NULL, python::pad_tuple (no_tuple, 0));
    }
    
    TEST_F(python_Test, pad_tuple_2)
    {
        PyObject *null_tuple = python::pad_tuple (NULL, 1);
        EXPECT_EQ(true, PyTuple_Check(null_tuple));
        EXPECT_EQ(1, PyTuple_GET_SIZE(null_tuple));
        EXPECT_EQ(Py_None, PyTuple_GET_ITEM(null_tuple, 0));
    }

    TEST_F(python_Test, pad_tuple_3)
    {
        PyObject *arg1 = PyString_FromString ("a");
        PyObject *arg2 = PyInt_FromLong (1);
        
        PyObject *a_tuple = PyTuple_New (2);
        PyTuple_SetItem (a_tuple, 0, arg1);
        PyTuple_SetItem (a_tuple, 1, arg2);
        
        PyObject *res = python::pad_tuple (a_tuple, 2);
        EXPECT_EQ(true, PyTuple_Check(res));
        EXPECT_EQ(4, PyTuple_GET_SIZE(res));
        EXPECT_EQ(Py_None, PyTuple_GET_ITEM(res, 0));
        EXPECT_EQ(Py_None, PyTuple_GET_ITEM(res, 1));
        EXPECT_EQ(arg1, PyTuple_GET_ITEM(res, 2));
        EXPECT_EQ(arg2, PyTuple_GET_ITEM(res, 3));
    }
    
} // namespace{}


int main(int argc, char **argv) 
{
    ::google::InitGoogleLogging(argv[0]);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
