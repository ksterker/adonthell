from adonthell import gfx, input, main
import sys, time

class InputtestApp (main.AdonthellApp):
    def __init__ (self):
        main.AdonthellApp.__init__(self)
        self.Letsexit = 0
        
    ## Callback fonction to handle keyboard events.
    ## It will be passed a keyboard_event as a parameter
    ## and is supposed to return nonzero if it took the
    ## event.
    def handle_keys (self, ev):
    
        ## If the key is released
        if ev.type() == input.keyboard_event.KEY_RELEASED:
            print "Key", ev.key_symbol(), "has been released."

        ## If the key is pushed
        elif ev.type() == input.keyboard_event.KEY_PUSHED:
            ## Print some info about what happened
            print "Key", ev.key_symbol(), "has been pushed, unichar emitted is",
            if not ev.unikey(): print "<none>"
            else: print "%c (%d)" % (chr(ev.unikey()), ev.unikey())
            ## On escape, the global variable letsexit will be set to 1
            if ev.key() == input.keyboard_event.ESCAPE_KEY:
                self.Letsexit = 1
                print "Escape pressed, leaving..."

        return 1

    def main (self):
        ## Initialize the gfx and input systems
        self.init_modules (main.app.GFX | main.app.INPUT)

        ## Set us a nice window
        gfx.screen.set_video_mode(640, 480)

        ## Create our input_listener and connect the callback
        ## to handle keyboard events
        il = input.listener()
        il.connect_keyboard_function(self.handle_keys)

        ## Add the listener to the manager
        input.manager.add(il)

        ## Run this loop until letsexit is set to 1 by the
        ## callback function. Every time a key event is raised,
        ## the input manager will send it to the listeners it handles
        ## (that is, only the one we've connected) which will call the
        ## callback function that has been connected to handle keyboard events.
        while not self.Letsexit:
            input.manager.update()

        return 0

if __name__ == '__main__':
    app = InputtestApp ()
    app.init (app.main)

