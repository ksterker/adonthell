 /*
   $Id: worldtest.cc,v 1.21 2008/07/10 20:19:45 ksterker Exp $

   Copyright (C) 2003/2004 Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Copyright (C) 2007/2008 Kai Sterker <kaisterker@linuxgames.com>
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

// #define DEBUG_COLLISION 1

#include "base/base.h"
#include "event/date.h"
#include "gfx/sprite.h"
#include "gfx/screen.h"
#include "input/manager.h"
#include "main/adonthell.h"
#include "python/python.h"
#include "world/area.h"
#include "world/character.h"
#include "world/object.h"
#include "world/mapview.h"

class game_client
{
public:
    world::area world;
    world::character * mchar;
    bool letsexit;
    bool draw_grid;
	bool draw_walkable;
	bool draw_bounding_box;
	bool screenshot;
	bool always_run;
	
    game_client()
    {
        letsexit = false;
        draw_grid = false;
        draw_walkable = false;
        draw_bounding_box = false;
        screenshot = false;
        always_run = false;
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
	            	if (!always_run) mchar->run ();
	            	else mchar->walk ();
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
	            	if (!always_run) mchar->walk();
	            	else mchar->run ();
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
        	if (kev->key() == input::keyboard_event::CAPSLOCK_KEY)
        	{
        		always_run = true;
				mchar->run ();
        	}
        	// quit
            if (kev->key() == input::keyboard_event::ESCAPE_KEY)
            {
                letsexit = true;
            }
            // toggle grid on|off
            if (kev->key() == input::keyboard_event::G_KEY)
            {
                draw_grid = !draw_grid;
            }
            // toggle object borders on|off
            if (kev->key() == input::keyboard_event::B_KEY)
            {
                draw_bounding_box = !draw_bounding_box;
            }
            // toggle collision area on|off
            if (kev->key() == input::keyboard_event::W_KEY)
            {
                draw_walkable = !draw_walkable;
            }
            // save snapshot
            if (kev->key() == input::keyboard_event::S_KEY)
            {
            	screenshot = true;
            }
        }
        else
        {
        	if (kev->key() == input::keyboard_event::CAPSLOCK_KEY)
        	{
        		always_run = false;
        		mchar->walk ();
        	}
        }

        return true;
    }

    void create_map ()
    {
        // that's 640x480
        world.resize (16, 12);
        
        // Adding map objects
        world::object * mobj;

		// short grass, 1x1 at index 0
        mobj = (world::object *) world.add_entity(world::OBJECT);
        mobj->load("data/models/map/ground/outside/short-grass-tile.xml");
        
        // long grass, 1.5x1.5 at index 1
        mobj = (world::object *) world.add_entity(world::OBJECT);
        mobj->load("data/models/map/ground/outside/long-grass-tile.xml");

        // wooden planks, 1x1 at index 2
        mobj = (world::object *) world.add_entity(world::OBJECT);
        mobj->load("data/models/map/ground/outside/wood-1.xml");

        // wooden pole, left, height 40 at index 3
        mobj = (world::object *) world.add_entity(world::OBJECT);
        mobj->load("data/models/map/ground/outside/wood-pole-l.xml");

        // wooden pole, right, height 40 at index 4 
        mobj = (world::object *) world.add_entity(world::OBJECT);
        mobj->load("data/models/map/ground/outside/wood-pole-r.xml");

        // diagonal cliff, 40x45x75 at index 5
        mobj = (world::object *) world.add_entity(world::OBJECT);
        mobj->load("data/models/map/wall/outside/cliff-se.xml");

        // diagonal cliff top, 40x45x5 at index 6
        mobj = (world::object *) world.add_entity(world::OBJECT);
        mobj->load("data/models/map/wall/outside/cliff-se-top.xml");
        
        // cliff facing south, 80x5x80 at index 7
        mobj = (world::object *) world.add_entity(world::OBJECT);
        mobj->load("data/models/map/wall/outside/cliff-s.xml");
     
        // Adding the map character
        mchar = (world::character *) world.add_entity(world::CHARACTER, "Player");
        mchar->load ("data/models/char/npc/ng.xml");
        mchar->set_speed (1.5);
        mchar->set_position (4, 5);
        mchar->set_offset (5, 0);
        mchar->set_altitude (0);
        mchar->set_limits (16, 12);
                
        world::coordinates mc;
        
        /*
        mc.set_position (4, 5);
        world.put_entity (0, mc); 

        mc.set_altitude (80);
        world.put_entity (2, mc);
         */
        
        // create ground (grass tiles are 40x40)
        for (u_int16 i = 0; i < world.length(); i++)
            for (u_int16 j = 0; j < 3; j++)
            {
                world::coordinates mc (i, j, 80, 0, 0);
                world.put_entity (0, mc);
            }
                
        for (u_int16 j = 0; j < 3; j++)
            for (u_int16 i = 0; i < 3-j; i++)
            {
                world::coordinates mc (i, j+3, 80, 0, 0);
                world.put_entity (0, mc);
            }
                        
        
		// 4 poles (left side)
        mc.set_position(10, 4);
        world.put_entity(3, mc);  // that one is actually invisible 
        mc.set_position(10, 6);
        world.put_entity(3, mc);
        // (right side)
        mc.set_offset(30, 0);
        mc.set_position(11, 4);  // that one is actually invisible
        world.put_entity(4, mc); 
        mc.set_position(11, 6);
        world.put_entity(4, mc); 

		// wooden platform
        for (int i = 10; i < 12; i++)
        {
            for (int j = 4; j < 6; j++)
            {
                world::coordinates mc (i, j, 40);
                world.put_entity (2, mc); 
            }
        }

		// 4 wooden poles
        mc.set_offset(0, 0);
        mc.set_position(7, 7);
        world.put_entity(3, mc); 
        mc.set_altitude(40);
        world.put_entity(3, mc); 
        mc.set_position(7, 4);
        mc.set_altitude(0);
        world.put_entity(3, mc); 
        mc.set_altitude(40);
        world.put_entity(3, mc); 
        
        mc.set_offset(30, 0);
        mc.set_altitude(0);
        mc.set_position(8, 7);
        world.put_entity(4, mc); 
        mc.set_altitude(40);
        world.put_entity(4, mc); 
        mc.set_position(8, 4);
        mc.set_altitude(0);
        world.put_entity(4, mc); 
        mc.set_altitude(40);
        world.put_entity(4, mc); 

		// wooden platform
        for (int i = 7; i < 9; i++)
        {
            for (int j = 4; j < 7; j++)
            {
                world::coordinates mc (i, j, 80);
                world.put_entity (2, mc); 
            }
        } 

		// "stair"
        for (int i = 4; i < 17; i++)
        {
            world::coordinates mc (i/2, 9, 5 * (i-4), (i%2)*20, 0);
            world.put_entity (2, mc); 
        }

        world::coordinates mc2 (3, 5, 0);
        world.put_entity (2, mc2);
        
        // create ground (grass tiles are 60x60, but grid is 40x40)
        for (float i = 0; i < world.length(); i += 1.5)
            for (float j = 2; j < world.height(); j += 1.5)
            {
                int x = world::SQUARE_SIZE * i;
                int y = world::SQUARE_SIZE * j;
                
                world::coordinates mc (x / world::SQUARE_SIZE, y / world::SQUARE_SIZE, 0, 
                                       x % world::SQUARE_SIZE, y % world::SQUARE_SIZE);
                world.put_entity (1, mc); 
            }
        
        
        // create diagonal wall
        for (int i = 0; i < 4; i++)
        {
            world::coordinates mc (i, 6-i, 0, 0, 0);
            world.put_entity (5, mc); 
            mc.set_altitude (75);
            world.put_entity (6, mc);
        }
        
        // create straight wall
        for (int i = 4; i < 16; i+=2)
        {
            world::coordinates mc (i, 3, 0, 0, 0);
            world.put_entity (7, mc); 
        }
    }
};

class world_test : public adonthell::app
{
public:
	int main () 
	{
        // Initialize the gfx and input systems
    	init_modules (GFX | INPUT | PYTHON);
    
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

        // we need to update the python search path to find our map view schedule 
        python::add_search_path (base::Paths.user_data_dir() + "/data/");
        
        // we need to load the world module before we can pass the character object to python
        python::import_module ("adonthell.world");        

        // arguments to map view schedule
        PyObject *args = PyTuple_New (1);
        PyTuple_SetItem (args, 0, python::pass_instance ((world::character*) gc.mchar));
        
        // The renderer ...
        world::mapview mv (320, 240);
        mv.set_map (&gc.world.Chunk);
        mv.set_schedule ("focus_on_character", args);
        
	    while (!gc.letsexit) 
    	{
        	u_int16 i;
        
        	// FIXME frames_missed is probably not what we want here 
	        // for (int i = 0; i < base::Timer.frames_missed (); i++) 
	        // {
            input::manager::update();
            events::date::update();
            gc.world.update();
            mv.update();
	        //}
	            
            mv.draw (160, 120);
            
	        if (gc.draw_grid)
	        {
	            for (i = 0; i < gfx::screen::length (); i += world::SQUARE_SIZE) 
	                gfx::screen::get_surface()->fillrect (i, 0, 1, gfx::screen::height (), 0xFFFF00); 
	            for (i = 0; i < gfx::screen::height (); i += world::SQUARE_SIZE) 
	                gfx::screen::get_surface()->fillrect (0, i, gfx::screen::length (), 1, 0xFFFF00); 
	        }
			
			if (gc.screenshot)
			{
				gfx::surface *screen = gfx::screen::get_surface();
                screen->save_png("screenshot.png");
			}
            
#if DEBUG_COLLISION
            gc.mchar->debug_collision();
            // gc.mchar->add_direction(gc.mchar->NORTH);
#endif
            // rectangle that should be filled with the mapview
            gfx::screen::get_surface()->fillrect (160, 120, 320, 1, 0xFF8888); 
            gfx::screen::get_surface()->fillrect (160, 240+120, 320, 1, 0xFF8888); 
            gfx::screen::get_surface()->fillrect (160, 120, 1, 240, 0xFF8888); 
            gfx::screen::get_surface()->fillrect (320+160, 120, 1, 240, 0xFF8888); 
                        
	        base::Timer.update (); 
	        gfx::screen::update ();
	        gfx::screen::clear (); 
	    }
	    
	    return 0; 
	}
};

world_test myApp;
