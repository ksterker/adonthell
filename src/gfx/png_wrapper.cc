/*
   Copyright (C) 2006   Tyler Nielsen <tyler.nielsen@gmail.com>
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
 * @file   gfx/png_wrapper.cc
 * @author Tyler Nielsen <tyler.nielsen@gmail.com>
 *
 * @brief  Defines the png static class.
 *
 *
 */

#include "png_wrapper.h"
#include "base/logging.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <png.h>

using namespace std;

namespace gfx
{
    // Functions for overriding reading/writing png using iostreams
    void user_read_data(png_structp png_ptr, png_bytep data, png_size_t length)
    {
        ifstream &file = *(ifstream *)(png_get_io_ptr(png_ptr));
        file.read((char *)data,length);
    }

    void user_write_data(png_structp png_ptr, png_bytep data, png_size_t length)
    {
        ofstream &file = *(ofstream*)(png_get_io_ptr(png_ptr));
        file.write((char *)data, length);
    }

    void user_flush_data(png_structp png_ptr)
    {
        ofstream &file = *(ofstream*)(png_get_io_ptr(png_ptr));
        file.flush();
    }

    void * png::get (ifstream & file, u_int16 & length, u_int16 & height, bool * alpha)
    {
        const int headerbytes = 8;  //This is used to read the file and make sure its a png... can be 1-8
        png_byte header[headerbytes];
        png_structp png_ptr;
        png_infop info_ptr;

        int number_of_passes;
        png_bytep * row_pointers;

        /* open file and test for it being a png */
        file.read((char *)header, headerbytes);
        if (png_sig_cmp(header, 0, headerbytes))
        {
            LOG(ERROR) << logging::indent() << "[read_png_file] File is not recognized as a PNG file";
            return NULL;
        }

        /* initialize stuff */
        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png_ptr)
        {
            LOG(ERROR) << logging::indent() << "[read_png_file] png_create_read_struct failed";
            return NULL;
        }

        info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr)
        {
            LOG(ERROR) << logging::indent() << "[read_png_file] png_create_info_struct failed";
            png_destroy_read_struct(&png_ptr, NULL, NULL);
            return NULL;
        }

        if (setjmp(png_jmpbuf(png_ptr)))
        {
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            LOG(ERROR) << logging::indent() << "[read_png_file] Error during init_io";
            return NULL;
        }

        png_set_read_fn(png_ptr, &file, user_read_data);
        png_set_sig_bytes(png_ptr, headerbytes);

        png_read_info(png_ptr, info_ptr);

        length = png_get_image_width(png_ptr, info_ptr);
        height = png_get_image_height(png_ptr, info_ptr);

        number_of_passes = png_set_interlace_handling(png_ptr);
        png_read_update_info(png_ptr, info_ptr);

        /* read file */
        if (setjmp(png_jmpbuf(png_ptr)))
        {
            LOG(ERROR) << logging::indent() << "[read_png_file] Error during read_image";
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            return NULL;
        }

        *alpha = (png_get_color_type(png_ptr, info_ptr) == PNG_COLOR_TYPE_RGBA);
        const int bytes_per_pixel = (*alpha) ? 4 : 3;
        char *image = (char *)calloc (length * height, bytes_per_pixel);
        int idx = 0;

        switch (png_get_color_type(png_ptr, info_ptr))
        {
        case PNG_COLOR_TYPE_RGBA:
        {
#ifdef __BIG_ENDIAN__
            // read pixels in ARGB format instead of RGBA
            png_set_swap_alpha(png_ptr);
#endif
            // fall through
        }
        case PNG_COLOR_TYPE_RGB: 
        {
            row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
            for (int y=0; y<height; y++)
                row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr, info_ptr));
            
            png_read_image(png_ptr, row_pointers);
                        
            const int line_buf_length = bytes_per_pixel * length;

            for (int y=0; y<height; y++)
            {
                const png_byte* row = row_pointers[y];

                memcpy(image + idx, row, line_buf_length);
                idx += line_buf_length;

                free(row_pointers[y]);
            }
            free(row_pointers);
            break; 
        }
        default:
            LOG(ERROR) << logging::indent() << "[read_png_file] color_type of input file must be PNG_COLOR_TYPE_RGBA (is " << png_get_color_type(png_ptr, info_ptr) << ")";
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            return NULL;
        }

        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return image;
    }

    void png::put (ofstream & file, const char *image, u_int16 length, u_int16 height, bool alpha)
    {
        png_structp png_ptr;
        png_infop info_ptr;
        png_bytep * row_pointers;

        /* initialize stuff */
        png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png_ptr)
        {
            LOG(ERROR) << logging::indent() << "[write_png_file] png_create_write_struct failed";
            return;
        }

        info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr)
        {
            LOG(ERROR) << logging::indent() << "[write_png_file] png_create_info_struct failed";
            png_destroy_write_struct(&png_ptr, NULL);
            return;
        }

        if (setjmp(png_jmpbuf(png_ptr)))
        {
            LOG(ERROR) << logging::indent() << "[write_png_file] Error during init_io";
            png_destroy_write_struct(&png_ptr, &info_ptr);
            return;
        }

        png_set_write_fn(png_ptr, &file, user_write_data, user_flush_data);

        /* write header */
        if (setjmp(png_jmpbuf(png_ptr)))
        {
            LOG(ERROR) << logging::indent() << "[write_png_file] Error during writing header";
            png_destroy_write_struct(&png_ptr, &info_ptr);
            return;
        }

        int color_type = alpha ? PNG_COLOR_TYPE_RGBA : PNG_COLOR_TYPE_RGB;
        png_set_IHDR(png_ptr, info_ptr, length, height, 8, color_type, 
                     PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, 
                     PNG_FILTER_TYPE_BASE);
        
        png_write_info(png_ptr, info_ptr);

        /* write bytes */
        if (setjmp(png_jmpbuf(png_ptr)))
        {
            LOG(ERROR) << logging::indent() << "[write_png_file] Error during writing bytes";
            png_destroy_write_struct(&png_ptr, &info_ptr);
            return;
        }

#ifdef __BIG_ENDIAN__
        if (alpha)
        {
            // write pixels in ARGB format instead of RGBA
            png_set_swap_alpha(png_ptr);
        }
#endif
        
        int idx = 0;
        row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
        for (int y=0; y<height; y++)
        {
            row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr, info_ptr));
            png_byte* row = row_pointers[y];
            for (int x=0; x<length; x++)
            {
                *row++ = image[idx++];
                *row++ = image[idx++];
                *row++ = image[idx++];
                if (alpha)
                    *row++ = image[idx++];
            }
        }

        png_write_image(png_ptr, row_pointers);
        png_write_end(png_ptr, NULL);

        for (int y=0; y<height; y++)
            free(row_pointers[y]);
        free(row_pointers);

        png_destroy_write_struct(&png_ptr, &info_ptr);
    }
}
