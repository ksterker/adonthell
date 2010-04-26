from adonthell import world, gfx, input, event, base
import adonthell.main
import sys

## Our exit variable
letsexit = 0

def handle_keys (ev):
    global letsexit
    print("in handle keys")
    if ev.type() == input.keyboard_event.KEY_PUSHED:
        if ev.key() == input.keyboard_event.ESCAPE_KEY:
            letsexit = 1
            print "Escape pressed, leaving..."
    return 1

class App (adonthell.main.AdonthellApp):

    def worldtest (self):
        global handle_keys
        # -- need gfx backend for graphics
        self.init_modules (self.GFX| self.INPUT)
        
        # -- add data directory to python search path
        sys.path.insert (0, "data")
        
        # -- need this for SDL backend to work
        #gfx.screen.set_video_mode (640, 480)
        #gfx.screen.set_video_mode (1024, 768)
        gfx.screen.set_video_mode (1280, 1024)

    
        ## Create our input_listener and connect the callback
        ## to handle keyboard events
        il = input.listener()
        il.connect_keyboard_function(handle_keys)

        ## Add the listener to the manager
        input.manager.add(il)


        # -- create world
        wrld = world.area ()
        
        # -- create character
        chr = wrld.add_character ("Player")
        
        # -- load character model from file
        chr.load ("data/models/char/npc/ng.xml")

        # -- set character attributes
        chr.set_position (160, 160)
        chr.set_altitude (5)
        chr.set_speed (1.0);

        # -- set character location in world
        wrld.place_entity (0, chr)

        # -- create a ground tile
        tile = wrld.add_object ()
        
        # -- load tile data
        tile.load ("data/models/map/ground/outside/wood-1.xml")
        
        # -- place tile in world
        pos = world.coordinates (160, 160, 0)
        wrld.place_entity (1, pos)

        # -- create mapview
        view = world.mapview (640, 480)
        
        # -- set map to show
        view.set_map (wrld)

        # -- set schedule of view
        view.set_schedule ("focus_on_character", ("Player",))

        mov = { 0:chr.EAST, 20:chr.SOUTH, 60:chr.WEST, 100:chr.NORTH, 140:chr.EAST }
        i = 0
        cur_mov = 0        

        # -- main loop
        while not letsexit:
           if i in mov:
               # -- let character walk
               chr.remove_direction(cur_mov)
               cur_mov = mov[i]
               chr.add_direction(cur_mov)

           # -- update game clock
           event.date.update ()
        
           # -- process world
           wrld.update ()
        
           # -- process map view
           view.update ()
           view.draw (0, 0)

           # update keys
           input.manager.update() 

           # -- debugging
           chr.debug_collision(0, 0)
           print chr.x(), chr.y(), chr.z()
           
           # -- process gfx output
           gfx.screen.update()
           gfx.screen.clear()
           
           # -- keep framerate stable
           base.Timer.update()
        
           i = (i + 1) % 160

        return 0
    
if __name__ == '__main__':
    myApp = App ()
    myApp.init (myApp.worldtest)
