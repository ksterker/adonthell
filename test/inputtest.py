from adonthell import gfx, input, main
import sys, time
    
## Callback fonction to handle keyboard events.
## It will be passed a keyboard_event as a parameter
## and is supposed to return nonzero if it took the
## event.
def handle_keys (ev):
    global letsexit
    
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
            letsexit = 1
            print "Escape pressed, leaving..."

    return 1

def inputtest ():
    ## Initialize the gfx and input systems
    if not gfx.init("sdl"): raise "Can't load gfx backend!"
    if not input.init("sdl"): raise "Can't load input backend!"

    ## Set us a nice window
    gfx.screen.set_video_mode(640, 480)

    ## Create our input_listener and connect the callback
    ## to handle keyboard events
    il = input.listener()
    il.connect_keyboard_function(handle_keys)

    ## Add the listener to the manager
    input.manager.add(il)

    ## Run this loop until letsexit is set to 1 by the
    ## callback function. Every time a key event is raised,
    ## the input manager will send it to the listeners it handles
    ## (that is, only the one we've connected) which will call the
    ## callback function that has been connected to handle keyboard events.
    while not letsexit:
        input.manager.update()

    ## Do some cleanup, and we're ready to exit!
    input.cleanup()
    gfx.cleanup()

    sys.exit(0)

if __name__ == '__main__':
        ## Our exit variable
        letsexit = 0

        myApp = main.AdonthellApp ()
        myApp.init (inputtest)

