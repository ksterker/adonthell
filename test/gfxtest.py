from adonthell import gfx, input
import time


if __name__ == '__main__':
    def key_callback(ev):
        global letsexit
        if ev.type() == input.keyboard_event.KEY_PUSHED:
            if ev.key() == input.keyboard_event.ESCAPE_KEY: letsexit = 1
        return 1

    if not gfx.init("sdl"): raise "Can't load gfx backend!"
    if not input.init("sdl"): raise "Can't load input backend!"

    gfx.screen.set_video_mode(640, 480)

    li = input.listener()
    li.connect_keyboard_function(key_callback)
    input.manager.add(li)

    letsexit = 0

    while not letsexit:
        ssurface = gfx.screen.get_surface()
        ssurface.lock()
        input.manager.update()
        ssurface.fillrect(10, 10, 50, 50, 0xffffff)
        ssurface.unlock()
        
        gfx.screen.update()

    input.cleanup()
    gfx.cleanup()
