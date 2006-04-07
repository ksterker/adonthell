/*
   $Id: png_wrapper.h,v 1.1 2006/04/07 16:12:59 Mithander Exp $

   Copyright (C) 2006 Tyler Nielsen <tyler.nielsen@gmail.com>
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
 * @file   png.h
 * @author Tyler Nielsen <tyler.nielsen@gmail.com>
 *
 * @brief  Declares the png static class.
 *
 *
 */

#ifndef PNG_H_
#define PNG_H_

#include "base/types.h"
#include <fstream>

namespace gfx
{

    /**
     * Allow reading and writing of PNG files.
     *
     */
    class png
    {
    public:
        /**
         * Reads a PNG %image from an opened file.
         *
         * @warning
         * The returned pointer is allocated by this function with calloc ().
         * Don't forget to free () it when you don't need it anymore!
         *
         * @param file opened file from which to read.
         * @param length pointer to the integer which will contain the %image's length.
         * @param height pointer to the integer which will contain the %image's height.
         *
         * @return allocated pointer containing the PNG %image.
         */
        static void *get (std::ifstream & file, u_int16 & length, u_int16 & height);

        /**
         * Saves a PNG %image into an opened file.
         *
         * @param file the opened file to write the PNG %image to.
         * @param image the PNG %image data.
         * @param length the length of the %image to write.
         * @param height the height of the %image to write.
         */
        static void put (std::ofstream & file, const char *image, const u_int16 length, const u_int16 height);
    };
}

#endif
