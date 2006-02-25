from adonthell import base, main, rpg
import sys


# -- test class
class App (main.AdonthellApp):
    # -- call super ctor
    def __init__ (self):
        main.AdonthellApp.__init__ (self)
    
        # -- set path to test data directory
        path = sys.path[0] + "/data"
        sys.path.insert (0, path)
    
    # -- test method (callback)
    def dialogtest (self):
        index = -1
        
        # -- character whose dialog is to be tested
        npc = rpg.character ("NPC", "Id", rpg.NPC)
        npc.set_dialogue ("demo_intro_1")
        
        # -- we also need a player character
        player = rpg.character ("Player", "Player", rpg.PLAYER)
        
        # -- start dialogue
        dlg = rpg.dialog (npc)
        
        # -- loop through dialogue
        while 1:
            line = dlg.run (index)
            if line == None: break
            
            # -- print NPC text
            print line.speaker(), ":", line.text()
            
            # -- print possible Player answers
            for i in range (0, line.num_answers()):
                print i+1, ")", line.answer (i)
            
            index = line.num_answers() - 1

# -- entry point to the script
if __name__ == '__main__':
    theApp = App ()
    theApp.init (theApp.dialogtest)