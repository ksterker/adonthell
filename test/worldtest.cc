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

#include <iomanip>

#include <adonthell/audio/sound.h>
#include <adonthell/base/base.h>
#include <adonthell/base/savegame.h>
#include <adonthell/event/date.h>
#include <adonthell/gfx/sprite.h>
#include <adonthell/gfx/screen.h>
#include <adonthell/input/manager.h>
#include <adonthell/main/adonthell.h>
#include <adonthell/rpg/character.h>
#include <adonthell/rpg/faction.h>
#include <adonthell/rpg/quest.h>
#include <adonthell/world/character.h>
#include <adonthell/world/object.h>
#include <adonthell/world/area_manager.h>
#include <adonthell/gui/ui_event.h>
#include <adonthell/gui/window_manager.h>
#include <adonthell/base/logging.h>

static world::debug_renderer DEBUG_RENDERER;

class game_client
{
public:
    bool delay;
    bool draw_grid;
	bool screenshot;
    bool draw_bounding_box;
    
    game_client()
    {
        delay = false;
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
        base::savegame::add (new base::serializer<rpg::quest> ());
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
                LOG(INFO) << "worldtest is stopping!";
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
                DEBUG_RENDERER.set_draw_bbox (draw_bounding_box);
            }
            // render tile after tile
            if (kev->key() == input::keyboard_event::D_KEY)
            {
                delay = !delay;
            	DEBUG_RENDERER.set_delay (delay ? 150 : 0);
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

    void play_sound(const events::event *event) {
        const gui::ui_event *e = (const gui::ui_event *) event;
        audio::sound *sound = (audio::sound *) e->user_data();
        sound->play(0);
    }
};

class world_test : public adonthell::app
{
public:
	int main ()
	{
        LOG(INFO) << "worldtest starting up!";

        LOG(INFO) << "Initialising game modules... ";
    	init_modules (AUDIO | GFX | INPUT | PYTHON | WORLD);
        LOG(INFO) << "  done!";

        LOG(INFO) << "Setting video mode... ";
    	gfx::screen::set_native_mode ();
        LOG(INFO) << "  done!";

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

        if (world::area_manager::get_map() == NULL)
        {
            LOG(ERROR) << "No map loaded, exiting";
            return 1;
        }

        // set mapview to proper size
        world::area_manager::get_mapview()->resize(gfx::screen::length(), gfx::screen::height());

        LOG(INFO) << "Fading sound in...";
        load_sound("worldtest.ogg")->fadein(3, -1);
        LOG(INFO) << "  done!";

        register_ui_sound("activate", "select.ogg");
        register_ui_sound("layout_switch", "switch.ogg");

        u_int32 totalFrames = 0;
        u_int32 totalTime = 0;
        u_int32 currentTime;

        // add mapview to window stack
        gui::window_manager::add(0, 0, world::area_manager::get_mapview());

	    while (IsRunning)
    	{
	        currentTime = base::Timer.current_time();

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
            
            if (gc.draw_bounding_box || gc.delay)
            {
                mv->set_renderer(&DEBUG_RENDERER);
            }
            else
            {
                mv->set_renderer(NULL);
            }            

            // render everything on screen
            gui::window_manager::update();

            // stop printing queue contents
            DEBUG_RENDERER.print_queue (false);

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

	        gfx::screen::update ();
	        gfx::screen::clear ();

            totalTime += base::Timer.current_time() - currentTime;
            totalFrames++;

	        base::Timer.update ();
	    } // while (main loop)

        // gc.world.save ("test-world-new.xml");

        LOG(INFO) << "Cleaning up faction... ";
        rpg::faction::cleanup();
        LOG(INFO) << "  done!";
        
        LOG(ERROR) << "Rendered " << totalFrames << " frames in " << totalTime << " ms.";
        LOG(ERROR) << "Average time required per frame is " << std::setprecision(4) << (double) totalTime / totalFrames  << " ms.";

	    return 0;
	}

private:
    // Contains map and player controls
    game_client gc;

    events::factory event_factory;

	audio::sound * load_sound(const std::string filename) {
	    LOG(INFO) << "Loading sound file '" << filename << "'";
	    audio::sound *sound = new audio::sound(filename);
	    LOG(INFO) << "  done!";

	    return sound;
	}

	void register_ui_sound(const std::string action, const std::string filename) {
        gui::ui_event *activate_event = new gui::ui_event((gui::widget*)gui::ui_event::ANY_SOURCE, action, load_sound(filename));
        event_factory.register_event(activate_event, ::base::make_functor(gc, &game_client::play_sound));
	}
};

world_test myApp;
