/*
 $Id: imagetest.cc,v 1.2 2007/12/29 22:21:38 ksterker Exp $
 
 Copyright (C) 2007 Kai Sterker <kaisterker@linuxgames.com>
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

#include "base/base.h"
#include "gfx/gfx.h"
#include "gfx/screen.h"
#include "main/adonthell.h"

class image_test : public adonthell::app
{
public:
	int main () 
    {
        // Initialize the gfx and input systems
    	init_modules (GFX);
        
    	// Set video mode
    	gfx::screen::set_video_mode (640, 480);
        
        gfx::surface* images[4];
        u_int8 psa[] = { 255, 127, 255, 127 };
        u_int8 ppa[] = { false, false, true, true };
        u_int8 col[] = { 0xff, 0x88, 0x00 };
        
        // test images
        for (int i = 0; i < 4; i++)
        {
            images[i] = gfx::create_surface();
            images[i]->set_alpha (psa[i], ppa[i]);
            images[i]->resize (80, 80);
            images[i]->fillrect (0, 0, i%2?40:80, i%2?80:40, images[i]->map_color(col[i%3], col[(i+1)%3], col[(i+2)%3]));
            images[i]->fillrect (i%2?40:0, i%2?0:40, i%2?40:80, i%2?80:40, images[i]->map_color(col[(i+2)%3], col[(i+1)%3], col[i%3], 0x88));
        }
        
        for (int i = 0; i < 1000; i++)
        {
            printf ("Iteration %i\n", i);
            for (int j = 0; j < 4; j++)
            {
                images[j]->draw ((j+1)*80, 0);
                images[j]->draw (0, (j+1)*80);
            
                for (int k = 0; k < 4; k++)
                {
                    gfx::surface *tmp = gfx::create_surface();
                    tmp->set_alpha (psa[j], ppa[j]);
                    tmp->resize (80, 80);
                    tmp->fillrect (0, 0, j%2?40:80, j%2?80:40, tmp->map_color(col[j%3], col[(j+1)%3], col[(j+2)%3]));
                    tmp->fillrect (j%2?40:0, j%2?0:40, j%2?40:80, j%2?80:40, tmp->map_color(col[(j+2)%3], col[(j+1)%3], col[j%3], 0x88));

                    images[k]->draw (0, 0, NULL, tmp);
                    tmp->draw ((k+1)*80, (j+1)*80);
                    // images[k]->draw ((k+1)*80, (j+1)*80);
                    delete tmp;
                }
            }
            
            base::Timer.update (); 
	        gfx::screen::update ();
	        gfx::screen::clear ();
	    }
	    
	    return 0;         
    }
};

image_test myApp;

