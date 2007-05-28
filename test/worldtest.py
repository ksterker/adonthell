import adonthell.base
import adonthell.main
from adonthell import world

class App (adonthell.main.AdonthellApp):

    filename = "data/character.xml"
    
    def worldtest (self):
        self.init_modules(self.GFX)

        wrld = world.area ()
        chr = world.character_with_gfx (wrld)
        
        model = chr.add_state ("test")
        model.set_area_size (1, 2)
        model.base.set_position (0, 1)
        model.base.set_offset (40, 40)
        
        chr.save (self.filename)
        print "Character saved to '%s'" % self.filename
        
        chr = world.character_with_gfx (wrld)
        chr.load (self.filename)
        return 0
    
if __name__ == '__main__':
    myApp = App ()
    myApp.init (myApp.worldtest)