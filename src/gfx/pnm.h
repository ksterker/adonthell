/*
   $Id: pnm.h,v 1.2 2003/11/22 09:35:21 ksterker Exp $

   Copyright (C) 1999/2000/2001/2002 Alexandre Courbot <alexandrecourbot@linuxgames.com>
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
 * @file   pnm.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the pnm static class.
 * 
 * 
 */

#ifndef PNM_H_
#define PNM_H_

#include "base/types.h"
#include <fstream>

namespace gfx
{

    /**
     * Allow reading and writing of PNM files.
     * 
     */
    class pnm
    {
    public:
        /** 
         * Reads a PNM %image from an opened file.
         *
         * @warning
         * The returned pointer is allocated by this function with calloc ().
         * Don't forget to free () it when you don't need it anymore!
         * 
         * @param file opened file from which to read.
         * @param length pointer to the integer which will contain the %image's length.
         * @param height pointer to the integer which will contain the %image's height.
         * 
         * @return allocated pointer containing the PNM %image.
         */
        static void *get (std::ifstream & file, u_int16 & length, u_int16 & height);

        /** 
         * Saves a PNM %image into an opened file.
         * 
         * @param file the opened file to write the PNM %image to.
         * @param image the PNM %image data.
         * @param length the length of the %image to write.
         * @param height the height of the %image to write.
         */
        static void put (std::ofstream & file, const char *image, const u_int16 length, const u_int16 height);

    private:
    
        /// Go to the next file's line.
        static void pnm_gotonextline (std::ifstream & file);

        /// Skip PNM comments.
        static int pnm_checkforcomment (std::ifstream & file);
    }; 
}

#endif
