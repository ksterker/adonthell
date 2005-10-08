#audio event and input test
from adonthell import base, main, event, audio
import random
import sys

# -- class containing a couple of test-callbacks
class event_script (object):
    def __init__ (self):
	self.sound = None
	self.sounds = [self.loadfile('at-demo-%x.ogg'%x) for x in range(2,10)]
	
    def loadfile(self, filename):
        print "Loading:", filename
        return audio.sound(filename)
	
    def callback (self, lst, evt):
        print evt.time (), base.Timer.uptime (), lst.get_event ().repeat ()
        if(self.sound):
	    self.sound.fadeout(5)
	self.sound = random.choice(self.sounds)
	print "Playing:", self.sound.getfilename()
	self.sound.fadein(5)

    def endsong (self, lst, evt):
        print "End of the song"

        
# -- class containing a couple of sounds
class App (main.AdonthellApp):
    def __init__ (self):
        main.AdonthellApp.__init__ (self)

    def eventtest (self):
        self.init_modules (main.app.AUDIO)

	self.sound = audio.sound('at-demo-1.ogg')
	self.sound.play()

        fty = event.factory ()
        svd = 0
        
        # -- first event
        ev1 = event.time_event ("0m30s")
        ev1.set_repeat ("2m00s", 9)
        
        lst = fty.add (ev1)
        lst.connect_callback ("audiotest", "event_script", "callback")

	# -- end of sound event
	ev2 = audio.audio_event (self.sound)
	lst = fty.add (ev2)
	lst.connect_callback ("audiotest", "event_script", "endsong")
        
        # run for a while
	while event.date.time () < 1000:
                
            base.Timer.update ()
            event.date.update ()
	    audio.update ()
    
if __name__ == '__main__':
    theApp = App ()
    theApp.init (theApp.eventtest)
    
