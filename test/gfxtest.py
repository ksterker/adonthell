from adonthell import gfx, input, main
import time

letsexit = 0
walkdir = None
screenshot = False

def gfxtest ():
    global screenshot, walkdir
    
    #setup key handler
    def key_callback(ev):
        global letsexit, walkdir, screenshot
        if ev.type() == input.keyboard_event.KEY_PUSHED:
            if ev.key() == input.keyboard_event.ESCAPE_KEY: letsexit = 1
            elif ev.key() == input.keyboard_event.LEFT_KEY: walkdir = 'walk_west'
            elif ev.key() == input.keyboard_event.RIGHT_KEY: walkdir = 'walk_east'
            elif ev.key() == input.keyboard_event.UP_KEY: walkdir = 'walk_north'
            elif ev.key() == input.keyboard_event.DOWN_KEY: walkdir = 'walk_south'
            elif ev.key() == input.keyboard_event.F8_KEY: screenshot = True
        elif ev.type() == input.keyboard_event.KEY_RELEASED:
            if ev.key() == input.keyboard_event.LEFT_KEY and walkdir == 'walk_west': walkdir = "stand" + walkdir[4:]
            elif ev.key() == input.keyboard_event.RIGHT_KEY and walkdir == 'walk_east': walkdir = "stand" + walkdir[4:]
            elif ev.key() == input.keyboard_event.UP_KEY and walkdir == 'walk_north': walkdir = "stand" + walkdir[4:]
            elif ev.key() == input.keyboard_event.DOWN_KEY and walkdir == 'walk_south': walkdir = "stand" + walkdir[4:]
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
    background.thisown = 0
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
    image = gfx.animation()
    image.load_animation("gfx/character/npc/naked_guy/naked_guy.xml")
    count = 0
    oldwalkdir = None
    while not letsexit:
        count += 1
        input.manager.update()
        if(walkdir != oldwalkdir):
            oldwalkdir = walkdir
            image.change_animation(walkdir)
        background.draw(0,0)
        image.draw(xloc,yloc)
        if count == 10: image.update(); count = 0
        
        if screenshot:
            print "saving screenshot to 'screenshot.png'"
            screenshot = False
            ssurface = gfx.screen.get_surface()
            ssurface.save_png("screenshot.png")
        
        gfx.screen.update()

    input.cleanup()
    gfx.cleanup()
    return 0

if __name__ == '__main__':
    myApp = main.AdonthellApp ()
    myApp.init (gfxtest)
    
