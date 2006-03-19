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
        file = base.ogzstream ()
        file.open (self.Data)
        io = base.diskio ()
        eqp.put_state (io)
        io.put_record (file)
        file.close ()
        
        print "Loading ..."
        file = base.igzstream ()
        file.open (self.Data)
        io = base.diskio ()
        io.get_record (file)
        eqp.get_state (io)
        file.close ()
        
        print "Humanoid Inventory:"
        inv = eqp.create_inventory ("Humanoid")
        slt = inv.first ()
        while slt != None:
            print "*", slt.id ()
            slt = inv.next()
        
def equipmenttest_func ():
    # -- set path to test data directory
    path = sys.path[0] + "/data"
    sys.path.insert (0, path)
    
    et = equipmenttest (path)
    et.equipment_test ()
    
if __name__ == '__main__':
    myApp = main.AdonthellApp ()
    myApp.init (equipmenttest_func)
