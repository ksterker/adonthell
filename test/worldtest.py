import adonthell.main
from adonthell import world, gfx, base

class App (adonthell.main.AdonthellApp):

    filename = "data/character.xml"
    
    def worldtest (self):
        # -- need gfx backend for graphics
        self.init_modules (self.GFX)
        
        # -- need this for SDL backend to work
        gfx.screen.set_video_mode (640, 480)

        # -- create world
        wrld = world.area ()
        
        # -- create character
        chr = world.character_with_gfx (wrld)

        # -- set character graphics
        chr.set_sprite ("gfx/character/npc/naked_guy/naked_guy.xml")
        
        # -- set 'physical' character representation
        model = chr.add_state ("stand_south")
        model.set_area_size (1, 2)
        model.base.set_position (0, 1)
        model.base.set_offset (40, 40)
        
        # -- set current state
        chr.set_state ("stand_south")
        
        # -- save to file
        chr.save (self.filename)
        print "Character saved to '%s'" % self.filename
        
        # -- restore from file
        chr2 = world.character_with_gfx (wrld)
        chr2.load (self.filename)

        # -- draw on screen
        chr2.draw (320, 240)
        gfx.screen.update ()

        # -- wait 5 seconde
        base.Timer.sleep (5000)

        return 0
    
if __name__ == '__main__':
    myApp = App ()
    myApp.init (myApp.worldtest)
