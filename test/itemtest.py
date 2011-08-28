from adonthell import base, main, event, rpg
from os.path import basename
import time, sys


class itemtest (object):
    def __init__ (self, path):
        self.path = path + "/items/"
    
    # -- helper method to print a list of slots
    def print_slots (self, slots):
        for s in slots:
            if s.count () > 0:
                print "Slot %s count/max %4i/%4i, item %s" % \
                    (s.id (), s.count (), s.get_item ().max_stack (), s.get_item ().name ())
            else:
                print "Slot %s is empty" % s.id ()
    
    # -- helper method to print a list of inventories
    def print_inventories (self, inventories):
        for i in inventories:
            s = i.first ()
            slots = []
            while s != None:
                slots.append (s)
                s = i.next ()
            self.print_slots (slots)
            print "-------------------"

    # -- helper method to move items between inventories
    def move (self, i1, i2, pos, count):
        slot = i1.first ()
        while pos > 2:
            pos = pos - 1
            slot = i1.next ()

        item = slot.get_item ()
        if item != None:
            print "Moving %s (%i) ..." % (item.name (), count)
            i2.add (item, count)

    # -- create all the items required by the various tests
    def create_items (self):
        print "Creating items ..."
        
        # -- create mutable items
        item = rpg.item (1)
        item.set_max_stack (5)
        item.create_instance ("lightsource")
        pyitem = item.get_instance ()
        
        # -- small torch
        print "* Small Torch"
        pyitem.Charge = 10
        pyitem.Duration = "5m"
        pyitem.Name = "Small Torch"
        pyitem.Value = 5
        
        item.put_state (self.path + "small_torch.item")
        
        # -- large torch
        print "* Large Torch"
        pyitem.Charge = 15
        pyitem.Duration = "8m"
        pyitem.Name = "Large Torch"
        pyitem.Value = 8
        
        item.put_state (self.path + "large_torch.item")
        
        # -- candle
        print "* Candle"
        pyitem.Charge = 20
        pyitem.Duration = "10m"
        pyitem.Name = "Candle"
        pyitem.Value = 10
        
        item.set_max_stack (10)
        item.put_state (self.path + "candle.item")
        
        # -- lamp
        print "* Oil Lamp"
        # pyitem = None
        item.create_instance ("rechargable_lightsource")
        item.set_max_stack (1)
        
        pyitem = item.get_instance ()
        pyitem.Charge = 6
        pyitem.MaxCharge = 12
        pyitem.Duration = "5m"
        pyitem.Name = "Oil Lamp"
        pyitem.Value = 15
        
        item.put_state (self.path + "lamp.item")

        # -- lamp oil
        print "* Oil Flask"

        item.create_instance ("item")
        item.set_max_stack (5)

        pyitem = item.get_instance ()
        pyitem.Charge = 24
        pyitem.MaxCharge = 24
        pyitem.Name = "Oil Flask"
        pyitem.Value = 6
        pyitem.add_category ("Lampfuel")

        item.put_state (self.path + "oil_flask.item")
        
        # -- poison
        print "* Nightshade Syrup"
        
        pyitem.Charge = 30
        pyitem.MaxCharge = 30
        pyitem.Name = "Nightshade Syrup"
        pyitem.Value = 80
        pyitem.add_category ("Poison")
        
        item.put_state (self.path + "nightshade_syrup.item")

        # -- create immutable items
        # -- coin
        print "* Coin"

        item = rpg.item (0)
        item.set_max_stack (1000)
        item.create_instance ("item")
        
        pyitem = item.get_instance ()
        pyitem.Name = "Coin"
        pyitem.Value = 0
        
        item.put_state (self.path + "coin.item")        
        
        # -- knife
        print "* Knife"
        item.set_max_stack (1)
        
        pyitem.Name = "Knife"
        pyitem.Value = 4
        
        item.put_state (self.path + "knife.item")
        
        # -- stick
        print "* Stick"

        item.create_instance ("transformable_item")
        item.set_max_stack (40)
 
        pyitem = item.get_instance ()
        pyitem.Name = "Stick"
        pyitem.Agent = "Knife"
        pyitem.Result = "Arrow"
        pyitem.Value = 0
        
        item.put_state (self.path + "stick.item")
        
        # -- arrow
        print "* Arrow"
        item.set_max_stack (50)
        item.create_instance ("projectile")
        
        pyitem = item.get_instance ()
        pyitem.Name = "Arrow"
        pyitem.Value = 1
        
        item.put_state (self.path + "arrow.item")
    
    # -- test behaviour of the slot class
    def slot_test (self):
        Torches = []
        Slots = []
        
        # -- create some mutable items
        print "Creating torches ...",
        while len (Torches) < 7:
            Torches.append (rpg.item (1))
            Torches[-1].get_state (self.path + "small_torch.item")
            print len(Torches),
        
        # -- create immutable items
        print "done\nCreating coins ...",
        Coin = rpg.item (0)
        Coin.get_state (self.path + "coin.item")

        # -- create some slots
        print "done\nCreating slots ...",
        while len (Slots) < 6:
            Slots.append (rpg.slot (None, str (len (Slots))))
            print len(Slots),
        
        # -- populate slots
        print "done\nPopulating slots ...",
        Slots[0].add (Coin, 500)
        Slots[1].add (Coin, 1000)
        for torch in Torches[0:2]: (Slots[2]).add (torch)
        for torch in Torches[2:]: (Slots[3]).add (torch)
        
        # -- print slots
        print "done"
        self.print_slots (Slots)
        
        # -- actual tests (note that only as much items will be moved as
        #    reside in source and fit into target, despite the number given)
        print "\nMoving 900 items from 1 to 0:"
        print "-----------------------------"
        Slots[0].add (Slots[1].get_item (), 900)
        self.print_slots (Slots[0:2])
        
        print "\nMoving 55 items from 0 to 1:"
        print "-----------------------------"
        Slots[1].add (Slots[0].get_item (), 55)
        self.print_slots (Slots[0:2])
        
        print "\nMoving 2000 items from 0 to 5:"
        print "------------------------------"
        Slots[5].add (Slots[0].get_item (), 2000)
        self.print_slots ((Slots[0], Slots[5]))
        
        print "\nMoving 10 items from 3 to 2:"
        print "----------------------------"
        Slots[2].add (Slots[3].get_item (), 10)
        self.print_slots (Slots[2:4])
        
        print "\nMoving 1 item from 2 to 3:"
        print "--------------------------"
        Slots[3].add (Slots[2].get_item (), 1)
        self.print_slots (Slots[2:4])
        
        print "\nMoving 6 items from 2 to 4:"
        print "---------------------------"
        Slots[4].add (Slots[2].get_item (), 6)
        self.print_slots ((Slots[2], Slots[4]))

    # -- test behaviour of inventory class
    def inventory_test (self):
        Torches = []
        Inventories = []
        
        # -- create some mutable items
        print "Creating torches ...",
        while len (Torches) < 7:
            Torches.append (rpg.item (1))
            Torches[-1].get_state (self.path + "small_torch.item")
            print len(Torches),
        
        # -- create immutable items
        print "done\nCreating coins ...",
        Coin = rpg.item (0)
        Coin.get_state (self.path + "coin.item")
        rpg.item_storage.add (Coin)

        # -- create some inventories
        print "done\nCreating inventories ...",
        Inventories.append (rpg.inventory (5, 1))
        Inventories.append (rpg.inventory (5, 1))
        Inventories.append (rpg.inventory (0, 0))

        # -- populate inventories
        print "done\nPopulating inventories ...",
        Inventories[0].add (Coin, 3500)
        Inventories[0].add (Torches[0])

        Inventories[1].add (Torches[1])
        Inventories[1].add (Torches[2])
        Inventories[1].add (Coin, 2100)
        Inventories[1].add (Torches[3])
        Inventories[1].add (Torches[4])

        Inventories[2].add (Coin, 2100)
        Inventories[2].add (Torches[5])
        Inventories[2].add (Torches[6])

        # -- actual tests
        print "done"
        self.print_inventories (Inventories)
        
        print "Saving ..."
        out = base.diskio (base.diskio.GZ_FILE)
        for i in Inventories: 
            i.put_state (out)
        out.put_record ("/tmp/itemtest.tmp")
        
        print "Cleaning up ..."
        Inventories = []
        
        Inventories.append (rpg.inventory ())
        Inventories.append (rpg.inventory ())
        Inventories.append (rpg.inventory ())
        
        print "Loading ..."
        rin = base.diskio (base.diskio.GZ_FILE)
        rin.get_record ("/tmp/itemtest.tmp")
        for i in Inventories:
            i.get_state (rin)
        
        self.print_inventories (Inventories)
        
        print "Moving some stuff around ..."
        self.move (Inventories[2], Inventories[0], 5, 2)
        self.move (Inventories[2], Inventories[0], 1, 55)
        self.move (Inventories[2], Inventories[2], 5, 100)
        self.move (Inventories[0], Inventories[0], 5, 4)
        self.print_inventories([Inventories[0], Inventories[2]])
        
        print "Saving ..."
        out = base.diskio (base.diskio.GZ_FILE)
        for i in Inventories: 
            i.put_state (out)
        out.put_record ("/tmp/itemtest.tmp")
        
        print "Cleaning up ..."
        Inventories = []
        
        Inventories.append (rpg.inventory ())
        Inventories.append (rpg.inventory ())
        Inventories.append (rpg.inventory ())
        
        print "Loading ..."
        rin = base.diskio (base.diskio.GZ_FILE)
        rin.get_record ("/tmp/itemtest.tmp")
        for i in Inventories:
            i.get_state (rin)
        
        self.print_inventories (Inventories)
        
        print "Moving some stuff around ..."
        self.move (Inventories[2], Inventories[1], 1, 1000)
        
        self.print_inventories ([Inventories[1], Inventories[2]])
        
        # -- cleanup
        rpg.item_storage.cleanup ()
        
def itemtest_func ():
    # -- set path to test data directory
    path = sys.path[0] + "/data"
    sys.path.insert (0, path)
    
    it = itemtest (path)
    it.create_items ()
    it.slot_test ()
    it.inventory_test ()

    return 0
    
if __name__ == '__main__':
    myApp = main.AdonthellApp ()
    myApp.init (itemtest_func)
