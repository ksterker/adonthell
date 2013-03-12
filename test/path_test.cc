 /*
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

#include <adonthell/base/base.h>
#include <adonthell/base/savegame.h>
#include <adonthell/event/date.h>
#include <adonthell/gfx/sprite.h>
#include <adonthell/gfx/screen.h>
#include <adonthell/input/manager.h>
#include <adonthell/main/adonthell.h>
#include <adonthell/rpg/character.h>
#include <adonthell/rpg/faction.h>
#include <adonthell/world/character.h>
#include <adonthell/world/object.h>
#include <adonthell/world/area_manager.h>
#include <adonthell/gui/window_manager.h>

class game_client
{
public:
    world::character * path_char; // Character used for pathfinding
    bool draw_grid;
	bool draw_bounding_box;
    bool draw_delay;
    bool print_queue;
	bool screenshot;
	s_int32 path_task;

    game_client()
    {
        draw_grid = false;
        draw_bounding_box = false;
        draw_delay = false;
        print_queue = false;
        screenshot = false;
        path_task = -1;
        path_char = NULL;

        // prepare gamedata handling
        // TODO: should probably be integrated directly into the engine,
        // i.e. into module initialization (adonthell::init_modules),
        // in which case we need to make sure to init modules in the proper
        // order too.

        // note: order is important; load EVENT before RPG before WORLD
        base::savegame::add (new base::serializer<events::date> ());
        base::savegame::add (new base::serializer<rpg::faction> ());
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
                adonthell::app::theApp->stop();
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
            // render tile after tile
            if (kev->key() == input::keyboard_event::D_KEY)
            {
                draw_delay = true;
            }
            // save snapshot
            if (kev->key() == input::keyboard_event::S_KEY)
            {
            	screenshot = true;
            }
            // print queue
            if (kev->key() == input::keyboard_event::Q_KEY)
            {
            	print_queue = true;
            }
            // start simple pathfinding search
            if (kev->key() == input::keyboard_event::P_KEY)
            {
                const char* zones[] = { "stair-1", "stair-2", "air-1", "air-2", "air-3",
                        "ground-1", "ground-2", "ground-3",
                        "cellar-1", "cellar-3", "cellar-3" };

                s_int32 idx = rand() % 11;
                path_task = world::area_manager::get_pathfinder()->add_task(path_char, zones[idx]);
                // path_task = world::area_manager::get_pathfinder()->add_task(path_char, "test");
                if (path_task >= 0)
                {
                    printf("Goal is %s\n", zones[idx]);
                }
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
	    srand(time(NULL));

        // Initialize the gfx and input systems
    	init_modules (GFX | INPUT | PYTHON | WORLD | GUI);

    	// Set video mode
    	gfx::screen::set_fullscreen(false);
    	gfx::screen::set_video_mode(640, 480);

		// Contains map and player controls
	    game_client gc;

		// Add keyboard controls
	    input::listener il;
    	input::manager::add(&il);

		// Listen for keyboard events
    	il.connect_keyboard_function (base::make_functor_ret(gc, &game_client::callback_func));

        // load initial game data
        base::savegame game_mgr;
        game_mgr.load (base::savegame::INITIAL_SAVE);

        // rpg character instance
        rpg::character *player = rpg::character::get_player();

        // Add faction to character
        player->add_faction("Noble");

        // get NPC ...
        gc.path_char = (world::character *) (world::area_manager::get_map()->get_entity ("NPC"));
        // ... and remove its schedule so we get direct control over its pathfinding
        gc.path_char->get_schedule()->clear_schedule();
        // set its pathfinding type so that the character will only walk over Wood
        gc.path_char->mind()->set_pathfinding_type("Only_on_Wood");

        rpg::character *npc = rpg::character::get_character("NPC");

        // arguments to map view schedule
        PyObject *args = PyTuple_New (1);
        PyTuple_SetItem (args, 0, python::pass_instance ("Player"));

        // The renderer ...
        world::debug_renderer rndr;

        world::mapview *mv = world::area_manager::get_mapview();
        mv->set_renderer (&rndr);

        // add mapview to window stack
        gui::window_manager::add(0, 0, world::area_manager::get_mapview());

	    while (IsRunning)
    	{
        	u_int16 i;

        	// FIXME frames_missed is probably not what we want here
	        // for (int i = 0; i < base::Timer.frames_missed (); i++)
	        // {
            input::manager::update();
            events::date::update();
	        //}

            // whether to draw bbox or not
            rndr.set_draw_bbox (gc.draw_bounding_box);

            // print queue contents
            if (gc.print_queue)
            {
                rndr.print_queue (true);
                gc.print_queue = false;
            }

            // draw with delay
            if (gc.draw_delay)
            {
                rndr.set_delay (150);
                gc.draw_delay = false;
            }

            // render mapview on screen
            base::Timer.update ();
            gui::window_manager::update();
            world::area_manager::update();

            // stop printing queue contents
            rndr.print_queue (false);
            rndr.set_delay (0);

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

			if (gc.path_task != -1 &&
			    world::area_manager::get_pathfinder()->return_state(gc.path_task) == world::pathfinding_manager::ACTIVE)
			{
			    const world::pathfinding_task *task = world::area_manager::get_pathfinder()->get_task(gc.path_task);
                for (std::vector<world::coordinates>::const_iterator i = task->path.begin(); i != task->path.end(); i++)
                {
                    s_int16 x = i->x()*20 - mv->get_view_start_x();
                    s_int16 y = i->y()*20 - mv->get_view_start_y() - i->z();

                    if (i == task->path.begin() + task->actualNode)
                        gfx::screen::get_surface()->fillrect (x, y, 20, 20, 0xFF880088);
                    else
                        gfx::screen::get_surface()->fillrect (x, y, 20, 20, 0x88FF8888);
                }
			}

	        gfx::screen::update ();
	        gfx::screen::clear ();
	    }

        rpg::faction::cleanup();

	    return 0;
	}
};

world_test myApp;
