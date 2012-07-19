#audio event and input test
from adonthell import base, main, event, audio
import random
import sys

def event_script_error():
    print
    print "Unable to connect to game_events.audiotest"
    print "You need to create an game_events directory in the path."
    print "and copy this file into it."
    print
    raise RuntimeError("game_events")
            
#a class for containing all the background music
class bgmusic:
    def __init__(self):
        self.sound = None
        self.sounds = [self.loadfile('at-demo-%x.ogg'%x) for x in range(2,10)]
  
        self.fty = event.factory ()
        for s in self.sounds:
            # -- end of sound event
            ev2 = audio.audio_event (s)
            lst = self.fty.add (ev2)
            if not lst.connect_callback ("audio_events", "event_script", "endsong", (self, )):
                event_script_error()
    
    def loadfile(self, filename):
        print "Loading:", filename
        return audio.sound(filename)

# -- class containing a couple of sounds
class App (main.AdonthellApp):
    def __init__ (self):
        main.AdonthellApp.__init__ (self)

    def eventtest (self):
        #Add data to the path
        path = "data"
        sys.path.insert (0, path)
        
        self.init_modules (main.app.AUDIO)

        self.sound = audio.sound('at-demo-1.ogg')
        self.sound.play()

        fty = event.factory ()
        svd = 0
        
        music = bgmusic()
  
        # -- first event
        ev1 = event.time_event ("0m30s")
        ev1.set_repeat ("10m00s", -1)
        
        lst = fty.add (ev1)
        if not lst.connect_callback ("audio_events", "event_script", "timer", (music, )):
            event_script_error()
        
        # run for a while
        while True:
            base.Timer.update ()
            event.date.update ()
            audio.update ()
    
if __name__ == '__main__':
    theApp = App ()
    theApp.init (theApp.eventtest)
    
