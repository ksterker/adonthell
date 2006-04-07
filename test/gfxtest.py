from adonthell import gfx, input, main
import time

letsexit = 0
xmove = 0
ymove = 0
screenshot = False

def gfxtest ():
    global screenshot
    
    #setup key handler
    def key_callback(ev):
        global letsexit, xmove, ymove, screenshot
        if ev.type() == input.keyboard_event.KEY_PUSHED:
            if ev.key() == input.keyboard_event.ESCAPE_KEY: letsexit = 1
            elif ev.key() == input.keyboard_event.LEFT_KEY: xmove = -1
            elif ev.key() == input.keyboard_event.RIGHT_KEY: xmove = 1
            elif ev.key() == input.keyboard_event.UP_KEY: ymove = -1
            elif ev.key() == input.keyboard_event.DOWN_KEY: ymove = 1
            elif ev.key() == input.keyboard_event.F8_KEY: screenshot = True
        elif ev.type() == input.keyboard_event.KEY_RELEASED:
            if ev.key() == input.keyboard_event.LEFT_KEY and xmove == -1: xmove = 0
            elif ev.key() == input.keyboard_event.RIGHT_KEY and xmove == 1: xmove = 0
            elif ev.key() == input.keyboard_event.UP_KEY and ymove == -1: ymove = 0
            elif ev.key() == input.keyboard_event.DOWN_KEY and ymove == 1: ymove = 0
        return 1

    if not gfx.init("sdl"): raise "Can't load gfx backend!"
    if not input.init("sdl"): raise "Can't load input backend!"

    gfx.screen.set_video_mode(640, 480)

    li = input.listener()
    li.connect_keyboard_function(key_callback)
    input.manager.add(li)

    xloc = 10
    yloc = 10
    
    #setup a 'fake' background
    background = gfx.create_surface()
    background.resize(640,480)
    backgroundcolor = [
    0x000000,0xFF0000,0x00FF00,0x0000FF,
    0xFFFF00,0xFF00FF,0x00FFFF,0xFFFFFF,
    0x7FFF00,0x007FFF,0xFF007F,0xFF7FFF]
    for x in xrange(4):
        for y in xrange(3):
            background.fillrect(160*x,160*y,160,160,backgroundcolor[0])
            backgroundcolor = backgroundcolor[1:]
            
    #load an image... gfx must be checked out and linked
    image = gfx.create_surface()
    image.load_png("gfx/item/treasure/catseye.png")
    image.set_mask(True)
    while not letsexit:
        input.manager.update()
        xloc += xmove; yloc += ymove
        background.draw(0,0)
        image.draw(xloc,yloc)
        
        if screenshot:
            print "saving screenshot to 'screenshot.png'"
            screenshot = False
            ssurface = gfx.screen.get_surface()
            ssurface.save_png("screenshot.png")
        
        gfx.screen.update()

    input.cleanup()
    #if this is called it causes a segfault... This will free the c++ pointer in image, 
    #when image goes out of scope it will be called again
    #gfx.cleanup()
    return 0

if __name__ == '__main__':
    myApp = main.AdonthellApp ()
    myApp.init (gfxtest)
    
