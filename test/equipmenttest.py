from adonthell import base, main, rpg
from os.path import basename
import time, sys


class equipmenttest (object):
    def __init__ (self, path):
        self.Data = path + "/eqp.data"
        
    # -- test behaviour of equipment class
    def equipment_test (self):

        eqp = rpg.equipment
        eqp.define_set ("Humanoid", ["Head", "Left Hand", "Right Hand", "Quiver"])

        eqp.define_slot ("Quiver", ["Projectile"], 0.0)
        eqp.define_slot ("Left Hand", ["Weapon"], 0.0)
        eqp.define_slot ("Right Hand", ["Shield"], 0.5)
        eqp.define_slot ("Head", ["Helmet"], 0.2)

        print "Saving ..."
        file = base.diskio (base.diskio.GZ_FILE)
        eqp.put_state (file)
        file.put_record (self.Data)
        
        print "Loading ..."
        file = base.diskio (base.diskio.GZ_FILE)
        file.get_record (self.Data)
        eqp.get_state (file)
        
        print "Humanoid Inventory:", eqp.get_set_modifier ("Humanoid")
        inv = eqp.create_inventory ("Humanoid")
        slt = inv.first ()
        while slt != None:
            print "*", slt.id (), ":", eqp.get_modifier (slt.id())
            slt = inv.next()
        
def equipmenttest_func ():
    # -- set path to test data directory
    path = sys.path[0] + "/data"
    sys.path.insert (0, path)
    
    et = equipmenttest (path)
    et.equipment_test ()
    
    return 0

if __name__ == '__main__':
    myApp = main.AdonthellApp ()
    myApp.init (equipmenttest_func)
