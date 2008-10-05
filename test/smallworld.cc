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

class world_test : public adonthell::app
{
public:
	int main () 
	{
        // Initialize the gfx and input systems
        // # -- need gfx backend for graphics
        // self.init_modules (self.GFX)
    	init_modules (GFX | INPUT | PYTHON);
    
        // # -- add data directory to python search path
        // sys.path.insert (0, "data")
        // we need to update the python search path to find our map view schedule 
        python::add_search_path (base::Paths.user_data_dir() + "/data/");
        

    	// Set video mode
	gfx::screen::set_fullscreen(false);
    	gfx::screen::set_video_mode(320, 240);
        
        // # -- create world
        world::area wrld;

        // # -- create character
        // chr = wrld.add_character ("Player")
	world::character *chr = dynamic_cast<world::character*> (wrld.add_entity(world::CHARACTER, "Player"));
        
        // # -- load character model from file
        chr->load ("data/models/char/npc/ng.xml");

        // # -- set character attributes
        chr->set_position (160, 160);
        chr->set_altitude (5);
        chr->set_speed (1.0);

        // # -- set character location in world
        wrld.put_entity (0, *chr);

        // # -- create a ground tile
	world::object *tile = dynamic_cast<world::object*> (wrld.add_entity(world::OBJECT));
        
        // # -- load tile data
        tile->load ("data/models/map/ground/outside/wood-1.xml");
        
        // # -- place tile in world
        world::coordinates pos(160, 160, 0);
        wrld.put_entity (1, pos);

        // # -- create mapview
        world::mapview view(320, 240);
        
        // # -- set map to show
        view.set_map (&wrld);

        // # -- set schedule of view
        // view.set_schedule ("focus_on_character", ("Player",))


        // we need to load the world module before we can pass the character object to python
        if (python::import_module ("adonthell.world") == NULL) return 1;        

        // arguments to map view schedule
        PyObject *args = PyTuple_New (1);
        PyTuple_SetItem (args, 0, python::pass_instance ("Player"));
        
        // The renderer ...
        view.set_schedule ("focus_on_character", args);

	int point[] = { 0, 20, 60, 100, 140 };
	world::character::direction dirs[] = {
		world::character::EAST,
		world::character::SOUTH,
		world::character::WEST,
		world::character::NORTH,
		world::character::EAST
	};

        world::character::direction cur_mov = world::character::NONE;
	int index = 0;

	for( int i = 0; i < 160; i = (i+1) % 160 ) {

		if( i == point[index] ) {
			chr->remove_direction(cur_mov);
			cur_mov = dirs[index];
			chr->add_direction(cur_mov);
			index = (index + 1) % 5;
		}

           // # -- update game clock
           events::date::update ();
        
           // # -- process world
           wrld.update ();
        
           // # -- process map view
           view.update ();
           view.draw (0, 0);
           
           // # -- debugging
           chr->debug_collision(0, 0);
//           print chr.x(), chr.y(), chr.z()
           
           // # -- process gfx output
           gfx::screen::update();
           gfx::screen::clear();
           
           // # -- keep framerate stable
           base::Timer.update();
        
	}

	    return 0; 
	}
};

world_test myApp;

