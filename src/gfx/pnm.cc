/*
   $Id: pnm.cc,v 1.3 2003/11/22 09:35:21 ksterker Exp $

   Copyright (C) 1999/2000/2001/2002   Alexandre Courbot <alexandrecourbot@linuxgames.com>
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
 * @file   pnm.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the pnm static class.
 * 
 * 
 */

#include "pnm.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace std;

namespace gfx
{
    
    void * pnm::get (ifstream & file, u_int16 & length, u_int16 & height)
    {
        char *image;
        char sign[10];
        u_int16 l, h;
        u_int32 i = 0;
        
        file.read(sign, 2);
        if ((sign[0] != 'P') || (sign[1] != '6'))
        {
            printf ("Invalid format.\n");
            return (NULL);
        }
        pnm_gotonextline (file);
        /* Getting height and length */
        while (pnm_checkforcomment (file));
        do
        {
            file.read(&sign[i], 1);
            i++;
        }
        while (sign[i - 1] != ' ');
        sign[i - 1] = 0;
        l = atoi (sign);
        i = 0;
        do
        {
            file.read(&sign[i], 1);
            i++;
        }
        while (sign[i - 1] != '\n');
        sign[i - 1] = 0;
        h = atoi (sign);
        /* Going to next line */
        pnm_gotonextline (file);
        /* Reading the image */
        image = (char *)calloc (l * h, 3);
        file.read(image, 3 *l * h);
        if (length)
            length = l;
        if (height)
            height = h;
        return (image);
    }
    
    void pnm::put (ofstream & file, const char *image, u_int16 length, u_int16 height)
    {
        char s[30];
        
        sprintf (s, "P6\n%d %d\n255\n", length, height);
        file.write(s, sizeof (char) * strlen (s));
        
        file.write(image, 3 * length * height);
    }
    
    void pnm::pnm_gotonextline (ifstream & file)
    {
        char buff;
        
        do file.read(&buff, 1);
        while (buff != '\n');
    }
    
    int pnm::pnm_checkforcomment (ifstream & file)
    {
        char buff;
        
        file.read(&buff, 1);
        if (buff == '#')
        {
            pnm_gotonextline (file);
            return (1);
        }
        else
        {
            file.seekg(-1, ios::cur);
            return (0);
        }
    }
}
