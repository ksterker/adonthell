from adonthell import gfx, input, gui, main
import sys, time

## Our exit variable
letsexit = 0

def handle_keys (ev):
    global letsexit
    if ev.type() == input.keyboard_event.KEY_PUSHED:
        if ev.key() == input.keyboard_event.ESCAPE_KEY:
            letsexit = 1
            print "Escape pressed, leaving..."
    return 1

def guitest ():
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

    ## gui stuff
    cont = gui.container ()
    cont.set_size (150, 150)
    cont.set_location (20, 30)

    a = dict ()
    for i in range (0,7):
    	a[i] = gui.base ()
	a[i].set_size (40,20)
	a[i].set_location (30, 10)
	cont.add_child (a[i])
  
    # set the layout
    cont.set_extend_policy (2)
    z = gui.listlayout ()
    cont.set_layout (z)

    
    ## Run this loop until letsexit is set to 1 by the
    ## callback function. Every time a key event is raised,
    ## the input manager will send it to the listeners it handles
    ## (that is, only the one we've connected) which will call the
    ## callback function that has been connected to handle keyboard events.
    while not letsexit:
        gfx.screen.update ()
        input.manager.update()
        #gfx.screen.draw ()
        cont.update ()
        cont.draw ()
        
    ## Do some cleanup, and we're ready to exit!
    input.cleanup()
    gfx.cleanup()

    sys.exit(0)
    
if __name__ == '__main__':
    myApp = main.AdonthellApp ()
    myApp.init (guitest)
