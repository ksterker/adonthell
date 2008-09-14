from adonthell import world, gfx, event, base
import adonthell.main
import sys

class App (adonthell.main.AdonthellApp):

    def worldtest (self):
        # -- need gfx backend for graphics
        self.init_modules (self.GFX)
        
        # -- add data directory to python search path
        sys.path.insert (0, "data")
        
        # -- need this for SDL backend to work
        gfx.screen.set_video_mode (640, 480)

        # -- create world
        wrld = world.area ()
        
        # -- create character
        chr = wrld.add_character ("Player")
        
        # -- load character model from file
        chr.load ("data/models/char/npc/ng.xml")

        # -- set character location in world
        pos = world.coordinates (4, 4, 0, 0, 0)
        wrld.put_entity (0, pos)

        # -- set character attributes
        chr.set_position (4, 4)
        chr.set_speed (1.0);

        # -- create a ground tile
        tile = wrld.add_object ()
        
        # -- load tile data
        tile.load ("data/models/map/ground/outside/wood-1.xml")
        
        # -- place tile in world
        pos.set_offset (0, 0)
        wrld.put_entity (1, pos)

        # -- create mapview
        view = world.mapview (640, 480)
        
        # -- set map to show
        view.set_map (wrld)

        # -- set schedule of view
        view.set_schedule ("focus_on_character", ("Player",))
        
        # -- main loop
        while 1:
           # -- update game clock
           event.date.update ()
        
           # -- process world
           wrld.update ()
        
           # -- process map view
           view.update ()
           view.draw (0, 0)
           
           # -- debugging
           chr.debug_collision()
           
           # -- process gfx output
           gfx.screen.update()
           gfx.screen.clear()
           
           # -- keep framerate stable
           base.Timer.update()
        
           # -- let character walk
           chr.add_direction (chr.SOUTH)

        return 0
    
if __name__ == '__main__':
    myApp = App ()
    myApp.init (myApp.worldtest)
