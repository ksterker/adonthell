/*
 $Id: imagetest.cc,v 1.1 2007/12/23 10:44:38 ksterker Exp $
 
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
        
        // test image
        gfx::surface *Image = gfx::create_surface();
        Image->resize (gfx::screen::length(), gfx::screen::height());
        Image->set_mask (true);
        
        for (int i = 0; i < 1000; i++)
        {
            printf ("Iteration %i\n", i);
            
            Image->fillrect (0, 0, Image->length() - 1, Image->height() - 1, gfx::screen::trans_color ());
            
            Image->draw_line (10, 25, 120, 250, Image->map_color (0, 0, 255));            
            Image->draw_line (120, 250, 55, 300, Image->map_color (255, 0, 0));        
            Image->draw_line (55, 300, 10, 25, Image->map_color (0, 0, 255));        
            
            Image->draw (0, 0);

            base::Timer.update (); 
	        gfx::screen::update ();
	        gfx::screen::clear (); 
	    }
	    
	    return 0;         
    }
};

image_test myApp;

