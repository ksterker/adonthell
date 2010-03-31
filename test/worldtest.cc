 /*
   $Id: worldtest.cc,v 1.41 2009/05/06 19:15:12 ksterker Exp $

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
#include "base/savegame.h"
#include "event/date.h"
#include "gfx/sprite.h"
#include "gfx/screen.h"
#include "input/manager.h"
#include "main/adonthell.h"
#include "rpg/character.h"
#include "rpg/specie.h"
#include "rpg/faction.h"
#include "world/character.h"
#include "world/object.h"
#include "world/area_manager.h"
#include "gui/window_manager.h"
#include "base/logging.h"

static world::debug_renderer DEBUG_RENDERER;

class game_client
{
public:
    bool letsexit;
    bool draw_grid;
	bool screenshot;
    bool draw_bounding_box;
    
    game_client()
    {
        letsexit = false;
        draw_grid = false;
        screenshot = false;
        draw_bounding_box = false;
        
        // prepare gamedata handling
        // TODO: should probably be integrated directly into the engine,
        // i.e. into module initialization (adonthell::init_modules),
        // in which case we need to make sure to init modules in the proper
        // order too.
        
        // note: order is important; load EVENT before RPG before WORLD
        base::savegame::add (new base::serializer<events::date> ());
        // todo: load species
        // todo: load factions
        base::savegame::add (new base::serializer<rpg::character> ());
        base::savegame::add (new base::serializer<world::area_manager> ());
    }

    // callback for key event listener
    bool callback_func(input::keyboard_event * kev)
    {
        if (kev->type() == input::keyboard_event::KEY_PUSHED)
        {
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
                DEBUG_RENDERER.set_draw_bbox (draw_bounding_box);
            }
            // render tile after tile
            if (kev->key() == input::keyboard_event::D_KEY)
            {
            	DEBUG_RENDERER.set_delay (150);
            }
            // save snapshot
            if (kev->key() == input::keyboard_event::S_KEY)
            {
                screenshot = true;
            }
            // print queue
            if (kev->key() == input::keyboard_event::Q_KEY)
            {
            	 DEBUG_RENDERER.print_queue (true);
            }
        }

        // do not consume event
        return false;
    }
};

class world_test : public adonthell::app
{
public:
	int main ()
	{
        LOG(INFO) << "worldtest starting up!";

        // Initialize the gfx and input systems
        LOG(INFO) << "Initialising game modules... ";
    	init_modules (GFX | INPUT | PYTHON | WORLD);
        LOG(INFO) << "  done!";

    	// Set video mode
        LOG(INFO) << "Setting video mode... ";
    	gfx::screen::set_video_mode(640, 480);
        LOG(INFO) << "  done!";

		// Contains map and player controls
	    game_client gc;

        LOG(INFO) << "Adding keyboard manager and listener... ";

		// Add keyboard controls
	    input::listener il;
    	input::manager::add(&il);

		// Listen for keyboard events
    	il.connect_keyboard_function (base::make_functor_ret(gc, &game_client::callback_func));

        LOG(INFO) << "  done!";

        // load initial game data
        LOG(INFO) << "Loading game data... ";
        base::savegame game_mgr;
        game_mgr.load (base::savegame::INITIAL_SAVE);
        LOG(INFO) << "  done!";

        // create a specie
        LOG(INFO) << "Creating 'Human' specie... ";
        rpg::specie human("Human");
        human.get_state("groups/human.specie");
        LOG(INFO) << "  done!";
        
        // create a faction
        LOG(INFO) << "Creating 'Noble' faction... ";
        rpg::faction noble("Noble");
        noble.get_state("groups/noble.faction");
        LOG(INFO) << "  done!";
        
        // rpg character instance
        LOG(INFO) << "Creating player character... ";
        rpg::character *player = rpg::character::get_player();
        player->set_specie ("Human");
        LOG(INFO) << "  done!";
        
        // Add faction to character
        LOG(INFO) << "Adding 'Noble' faction to player character... ";
        player->add_faction("Noble");
        LOG(INFO) << "  done!";

        LOG(INFO) << "Setting 'Human' specie in player character... ";
        rpg::character *npc = rpg::character::get_character("NPC");
        npc->set_specie ("Human");
        LOG(INFO) << "  done!";
        
        // arguments to map view schedule
        LOG(INFO) << "Adding player character to mapview schedule... ";
        PyObject *args = PyTuple_New (1);
        PyTuple_SetItem (args, 0, python::pass_instance ("Player"));
        LOG(INFO) << "  done!";
        
	    while (!gc.letsexit)
    	{
        	u_int16 i;

        	// FIXME frames_missed is probably not what we want here
	        // for (int i = 0; i < base::Timer.frames_missed (); i++)
	        // {
            input::manager::update();
            events::date::update();
            world::area_manager::update();
	        //}

            // whether to draw bbox or not
            world::mapview *mv = world::area_manager::get_mapview();
            
            if (gc.draw_bounding_box)
            {
                mv->set_renderer(&DEBUG_RENDERER);
            }
            else
            {
                mv->set_renderer(NULL);
            }            

            // render mapview on screen
            mv->draw (0, 0);

            // stop printing queue contents
            DEBUG_RENDERER.print_queue (false);
            DEBUG_RENDERER.set_delay (0);

            // whether to render grid
	        if (gc.draw_grid)
	        {
	            for (i = 0; i < gfx::screen::length (); i += 40)
	                gfx::screen::get_surface()->fillrect (i, 0, 1, gfx::screen::height (), 0xFFFF00);
	            for (i = 0; i < gfx::screen::height (); i += 40)
	                gfx::screen::get_surface()->fillrect (0, i, gfx::screen::length (), 1, 0xFFFF00);
	        }

            // take a screenshot
			if (gc.screenshot)
			{
				gfx::surface *screen = gfx::screen::get_surface();
                screen->save_png("screenshot.png");
                gc.screenshot = false;
			}

#if DEBUG_COLLISION
            world::character *mchar = (world::character *) (gc.world.get_entity ("Player"));
            mchar->debug_collision(160 + (320 - 160)/2, 120 + (240 - 240)/2);
            // mchar->add_direction(gc.mchar->NORTH);
#endif
	        base::Timer.update ();
            gui::window_manager::update();
	        gfx::screen::update ();
	        gfx::screen::clear ();
	    } // while (main loop)

        // gc.world.save ("test-world-new.xml");
        
        LOG(INFO) << "Cleaning up specie... ";
        rpg::specie::cleanup();
        LOG(INFO) << "  done!";

        LOG(INFO) << "Cleaning up faction... ";
        rpg::faction::cleanup();
        LOG(INFO) << "  done!";
        
	    return 0;
	}
};

world_test myApp;
