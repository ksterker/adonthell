from adonthell import base, main, event
import sys

# -- class containing a couple of test-callbacks
class event_script (object):
    def __init__ (self):
        self.counter = 0
        
    def callback_1 (self, lst, evt, str, num):
        print str, num, evt.time (), base.Timer.uptime (), lst.get_event ().repeat ()
        lst.connect_callback ("eventtest", "event_script", "callback_1", (str, num+1))
        self.counter += 1
        
    def callback_2 (self, lst, evt):
        print "#2", self.counter, evt.time (), base.Timer.uptime (), lst.get_event ().repeat ()

        # -- switch callback
        if self.counter == 4:
            print "#2 Switching to callback_3 / Increasing repeat count to 4"
            lst.connect_callback ("eventtest", "event_script", "callback_3", ("#2",))
            lst.get_event ().set_repeat (4)
            
    def callback_3 (self, lst, evt, num):
        print num, evt.time (), base.Timer.uptime (), lst.get_event ().repeat ()
        if lst.get_event ().repeat () == 2:
            # -- destroy
            print "#2 Destroying at repeat count '2'"
            lst.destroy ()

class App (main.AdonthellApp):
    def __init__ (self):
        main.AdonthellApp.__init__ (self)

    def eventtest (self):
        # -- init
        self.init_modules (self.EVENT)

        fty = event.factory ()
        svd = 0
        
        # -- first event
        ev1 = event.time_event ("0m30s")
        ev1.set_repeat ("30s", 6)
        
        lst = fty.add (ev1)
        lst.connect_callback ("eventtest", "event_script", "callback_1", ("#1", 1))
        
        # -- second event
        ev2 = event.time_event ("1m05s")
        ev2.set_repeat ("30s", 4)
        
        lst = fty.add (ev2)
        lst.connect_callback ("eventtest", "event_script", "callback_2")
        
        # -- run for 3:20 gametime minutes
        while event.date.time () < 200:
            # -- save factory to disk
            if event.date.time () == 100 and svd == 0:
                print "   Saving state"
                file = base.ogzstream ()
                file.open ("/tmp/eventtest.tmp")
                record = base.diskio ()
                fty.put_state (record)
                record.put_record (file)
                file.close ()
                svd = 1
                
            base.Timer.update ()
            event.date.update ()
    
        # -- resume with saved state
        print "   Returning to previously saved state"
        file = base.igzstream ()
        file.open ("/tmp/eventtest.tmp")
        fty.clear ()
        
        record = base.diskio ()
        record.get_record (file)
        success = fty.get_state (record)
        if success == 1: print "OK"
        file.close ()
        
        # -- run for another 1:40 gametime minutes
        while event.date.time () < 300:
            base.Timer.update ()
            event.date.update ()
    
if __name__ == '__main__':
    theApp = App ()
    theApp.init (theApp.eventtest)
    
