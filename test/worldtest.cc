/*
   $Id: worldtest.cc,v 1.2 2007/06/11 06:45:01 ksterker Exp $

   Copyright (C) 2003/2004 Alexandre Courbot <alexandrecourbot@linuxgames.com>
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
#include "gfx/animation.h"
#include "input/manager.h"
#include "main/adonthell.h"
#include "world/area.h"

class game_client
{
public:
    world::area world;
    world::character_with_gfx * mchar;
    bool letsexit;
    bool draw_grid;

    game_client()
    {
        letsexit = false;
        draw_grid = false;
    }

	// callback for control event listener
    bool control_func(input::control_event * cev)
    {
        if (cev->type() == input::control_event::BUTTON_PUSHED)
        {
        	switch (cev->button())
        	{       		
	            case input::control_event::A_BUTTON:
	            {
	                mchar->run();
	                break;
	            }            
	            case input::control_event::B_BUTTON:
	            {
	                mchar->jump();
	                break;
	            }            
	            case input::control_event::LEFT_BUTTON:
	            {
	                mchar->add_direction(mchar->WEST);
	                break;
	            }
	            case input::control_event::RIGHT_BUTTON:
	            {
	                mchar->add_direction(mchar->EAST);
	                break;
	            }
	            case input::control_event::UP_BUTTON:
	            {
	                mchar->add_direction(mchar->NORTH);
	                break;
	            }
	            case input::control_event::DOWN_BUTTON:
	            {
	                mchar->add_direction(mchar->SOUTH);
	                break;
	            }
	            default: break;   
        	}
        }
        else
        {
        	switch (cev->button())
        	{       		
	            case input::control_event::A_BUTTON:
	            {
                	mchar->walk();
	                break;
	            }            
	            case input::control_event::LEFT_BUTTON:
	            {
	                mchar->remove_direction(mchar->WEST);
	                break;
	            }
	            case input::control_event::RIGHT_BUTTON:
	            {
	                mchar->remove_direction(mchar->EAST);
	                break;
	            }
	            case input::control_event::UP_BUTTON:
	            {
	                mchar->remove_direction(mchar->NORTH);
	                break;
	            }
	            case input::control_event::DOWN_BUTTON:
	            {
	                mchar->remove_direction(mchar->SOUTH);
	                break;
	            }
	            default: break;   
        	}
        }

        return true;
    }
    
    // callback for key event listener
    bool callback_func(input::keyboard_event * kev)
    {
        if (kev->type() == input::keyboard_event::KEY_PUSHED)
        {
            if (kev->key() == input::keyboard_event::ESCAPE_KEY)
            {
                letsexit = true;
            }            
            if (kev->key() == input::keyboard_event::G_KEY)
            {
                draw_grid = !draw_grid;
            }            
            if (kev->key() == input::keyboard_event::O_KEY)
            {
                world.output_occupation();
            }            
        }

        return true;
    }

    void create_map ()
    {
        // that's 640x480
        world.resize (16, 12);
        
        // Adding the map character
        mchar = (world::character_with_gfx *) world.add_character();
        mchar->load ("data/models/char/npc/ng.xml");
        mchar->set_speed (1.0);
        mchar->set_position (4, 4);
        mchar->set_limits (16, 12);
        
        // Adding map objects
        world::object_with_gfx * mobj;
        
        mobj = (world::object_with_gfx *) world.add_object();
        mobj->load("data/models/map/ground/outside/long-grass-tile.xml");
        
        /*
        mobj = (world::object_with_gfx *)world.add_object();
        mobj->load("data/gfx/sandy.xml");

        mobj = (world::object_with_gfx *)world.add_object();
        mobj->load("data/gfx/rug.xml");
        
        mobj = (world::object_with_gfx *)world.add_object();
        mobj->load("data/gfx/platform.xml");
        
        mobj = (world::object_with_gfx *)world.add_object();
        mobj->load("data/gfx/pillar_l.xml");
        
        mobj = (world::object_with_gfx *)world.add_object();
        mobj->load("data/gfx/pillar_r.xml");
        */
        
        world::coordinates mc;
        
        // create ground (grass tiles are 60x60, but grid is 40x40)
        for (float i = 0; i < world.length(); i += 1.5)
            for (float j = 0; j < world.height(); j += 1.5)
            {
                int x = world::SQUARE_SIZE * i;
                int y = world::SQUARE_SIZE * j;
                
                world::coordinates mc (x / world::SQUARE_SIZE, y / world::SQUARE_SIZE, 0, 
                                       x % world::SQUARE_SIZE, y % world::SQUARE_SIZE);
                world.put_object (0, mc); 
            }
        
        /*
        mc.set_position(10, 5);
        world.put_object(4, mc); 
        mc.set_position(11, 5);
        world.put_object(5, mc); 
        mc.set_position(10, 3);
        world.put_object(4, mc); 
        mc.set_position(11, 3);
        world.put_object(5, mc); 

        for (int i = 10; i < 12; i++)
            for (int j = 4; j < 6; j++)
            {
                world::coordinates mc(i, j, 40);
                world.put_object(3, mc); 
            }
        
        mc.set_position(7, 6);
        world.put_object(4, mc); 
        mc.set_altitude(40);
        world.put_object(4, mc); 
        mc.set_position(7, 3);
        mc.set_altitude(0);
        world.put_object(4, mc); 
        mc.set_altitude(40);
        world.put_object(4, mc); 
        
        mc.set_altitude(0);
        mc.set_position(8, 6);
        world.put_object(5, mc); 
        mc.set_altitude(40);
        world.put_object(5, mc); 
        mc.set_position(8, 3);
        mc.set_altitude(0);
        world.put_object(5, mc); 
        mc.set_altitude(40);
        world.put_object(5, mc); 
        
        for (int i = 7; i < 9; i++)
            for (int j = 4; j < 7; j++)
            {
                world::coordinates mc(i, j, 80);
                world.put_object(3, mc); 
            }
        
        for (int i = 0; i < 13; i++)
        {
            world::coordinates mc(i + 1, 9, 5 * (i + 1));
            world.put_object(3, mc); 
        }
        */
    }
};

class world_test : public adonthell::app
{
public:
	int main () 
	{
        // Initialize the gfx and input systems
    	init_modules (GFX | INPUT);
    
    	// Set video mode
    	gfx::screen::set_video_mode(640, 480);

		// Contains map and player controls
	    game_client gc;

		// Add keyboard controls
	    input::listener il;
    	input::manager::add(&il);

		// Listen for control events
    	il.connect_control_function (base::make_functor_ret(gc, &game_client::control_func));

		// Listen for keyboard events
    	il.connect_keyboard_function (base::make_functor_ret(gc, &game_client::callback_func));

		// Create game world
	    gc.create_map();

		// The renderer ...
	    while (!gc.letsexit) 
    	{
        	u_int16 i, j;
        
        	// FIXME frames_missed is probably not what we want here 
	        // for (int i = 0; i < base::Timer.frames_missed (); i++) 
	        // {
            input::manager::update();
            gc.world.update();
	        //}
	
	        std::list <world::square_info> drawqueue; 
	
	        // Rendering phase        
	        for (j = 0; j < gc.world.height (); j++)
	        {
	            for (i = 0; i < gc.world.length (); i++) 
	            {
	                world::square * sq = gc.world.get (i, j); 
	                world::square::iterator it = sq->end();
	                while (it != sq->begin())
	                {
	                    --it;
	                    if (!it->oy())
	                    {
	                        if (it->x () == i && it->y () == j) 
	                            drawqueue.push_back(*it);
	                    }
	                    else
	                    {
	                        if (it->x () == i && it->y () == j - 1) 
	                            drawqueue.push_back(*it);
	                    }	
	                }
	            }

		        drawqueue.sort();
		            
		        for (std::list <world::square_info>::iterator it = drawqueue.begin ();
		             it != drawqueue.end (); it++)
		        {
		            switch ((*it).obj->type ()) 
		            {
		                case world::CHARACTER:
		                    ((world::character_with_gfx *)
		                     (*it).obj)->draw ((*it).x () * world::SQUARE_SIZE + (*it).ox (),
		                                       (*it).y () * world::SQUARE_SIZE + (*it).oy ());
		                    break; 
		                    
		                case world::OBJECT:
		                    ((world::object_with_gfx *)
		                     (*it).obj)->draw ((*it).x () * world::SQUARE_SIZE + (*it).ox (),
		                                       (*it).y () * world::SQUARE_SIZE + (*it).oy () - (*it).z());
		                    break;
		                    
		                default:
		                    break; 
		            }
		        }
	        
	        	drawqueue.clear ();
	    	}

	        if (gc.draw_grid)
	        {
	            for (i = 0; i < gfx::screen::length (); i += world::SQUARE_SIZE) 
	                gfx::screen::get_surface()->fillrect (i, 0, 1, gfx::screen::height (), 0xFFFF00); 
	            for (i = 0; i < gfx::screen::height (); i += world::SQUARE_SIZE) 
	                gfx::screen::get_surface()->fillrect (0, i, gfx::screen::length (), 1, 0xFFFF00); 
	        }
	
	        base::Timer.update (); 
	        gfx::screen::update ();
	        gfx::screen::clear (); 
	    }
	    
	    return 0; 
	}
};

world_test myApp;
