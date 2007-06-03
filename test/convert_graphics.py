from adonthell import base, gfx, main
import os.path
import sys

""" convert v0.3.x raw image to png image """
def convert_raw_image (img, name):

    # -- get length and height
    length = img.get_uint16 ()
    height = img.get_uint16 ()

    print "Length = %i, Height = %i" % (length, height)
    
    png = gfx.create_surface ()
    png.resize (length, height)
    png.lock ()

    for y in range (height):
        for x in range (length):
            r = img.get_uint8 ()
            g = img.get_uint8 ()
            b = img.get_uint8 ()
            col = png.map_color (r, g, b)
            png.put_pix (x, y, col)
        
    png.unlock ()
    
    # -- for debugging
    # png.draw (320 - length/2, 240 - height/2)
    # gfx.screen.update()
    # base.Timer.sleep (5000)
    
    # -- save image
    png.save_png (name + ".png")
    print "Saved %s.png" % name

""" convert v0.3.x image to png image """
def convert_image (img, name):
    
    # -- read mask and alpha value
    mask = img.get_uint8 ()
    alpha = img.get_uint8 ()
    
    convert_raw_image (img, name)


""" convert v0.3.x animation to png image """
def convert_animation (anim, name):
    # -- skip offset/length/height
    anim.get_uint32()
    anim.get_uint32()
    
    # -- read number of images
    nbr = anim.get_uint16()
    for i in range(nbr):
        convert_raw_image (anim, name + "_" + str(i))

    # -- skip frames
    nbr = anim.get_uint16()
    for i in range(nbr):
        anim.get_uint32()
        anim.get_uint32()
        anim.get_uint32()

""" convert v0.3.x mapobject to png image """
def convert_mapobject (mobj, name):
    # -- skip file version info
    mobj.get_uint8()
    mobj.get_uint16()
    
    # -- read number of animations
    nbr = mobj.get_uint16()
    for i in range(nbr):
        convert_animation (mobj, name + "_" + str(i))

""" convert either animation or mapobject """
def convert():
    
    file = sys.argv[1]
    
    old_gfx = base.igzstream ()
    old_gfx.open (file)
    
    if not old_gfx.is_open ():
        print "Cannot open file", file
        sys.exit (1)

    gfx.init ("sdl")
    gfx.screen.set_video_mode (640, 480)
    
    filename, ext = os.path.splitext (file)
    if ext == ".mobj":
        print "Mapobject found"
        convert_mapobject (old_gfx, filename)
        
    elif ext == ".anim":
        print "Animation found"
        convert_anim (old_gfx, filename)
    
    elif ext == ".img":
        print "Image found"
        convert_raw_image (old_gfx, filename)
    
    elif ext == ".mchar":
        print "Character sprites not supported (yet)"
        
    else:
        print "File extension '%s' not recognized" % ext
        print "Valid extensions are .anim, .img, .mobj and .mchar"
        
    return 0
    
# -- entry point to the script
if __name__ == '__main__':
    myApp = main.AdonthellApp ()
    myApp.init (convert)
