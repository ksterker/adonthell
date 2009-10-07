from adonthell import base, main, event, rpg
import sys

class grouptest (object):
    def __init__ (self, path):
        self.path = path + "/groups/"
    
    # -- Creates the necessary faction
    def create_faction (self):
        
        # -- Create noble
        noble = rpg.faction("Noble")
        noble.create_instance("faction")
        
        pynoble = noble.get_instance(noble)
        
        print "+++ Creating Noble"
        pynoble.Name = "Noble"
        pynoble.Dic = { "Wood": 5, "Swamp": -30 }
        
        noble.put_state (self.path + "noble.faction")
        
        # -- Create human
        human = rpg.specie("Human")
        human.create_instance("specie")
        
        pyhuman = human.get_instance(human)
        
        print "+++ Creating Human"
        pyhuman.Name = "Human"
        pyhuman.Dic = { "Wood": 10, "Swamp": -10 }
        
        human.put_state (self.path + "human.specie")
     
    def test_loading (self):
        print "*** Starting loading test"
        print "+++ Loading Noble"
        
        noble = rpg.faction("Noble")
        ret = noble.get_state(self.path + "noble.faction")
        
        if ret == True:
            print "   Success"    
            pynoble = noble.get_instance(noble)
        
            print "   Noble(C++) Name:", noble.name()
            print "   Noble(PY) Name:", pynoble.Name
            print "   Noble(PY) Dic:", pynoble.Dic
            
        print "+++ Loading Human"
        
        human = rpg.specie("Human")
        ret = human.get_state(self.path + "human.specie")
        
        if ret == True:
            print "   Success"    
            pyhuman = human.get_instance(human)
        
            print "   Human(C++) Name:", human.name()
            print "   Human(PY) Name:", pyhuman.Name
            print "   Human(PY) Dic:", pyhuman.Dic


        

# -- set path to test data directory
path = sys.path[0] + "/data"
sys.path.insert (0, path)

fac = grouptest(path)
fac.create_faction()
fac.test_loading()
