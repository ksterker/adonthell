from adonthell import gfx, main
import time

def gfxtest ():
    if not gfx.init("sdl"): raise "Can't load gfx backend!"
    gfx.screen.set_video_mode(640, 480)
    image = gfx.animation()
    image.load_animation("foo")
    image.save_animation("naked_guy.xml")
    return 0

if __name__ == '__main__':
    myApp = main.AdonthellApp ()
    myApp.init (gfxtest)
    
