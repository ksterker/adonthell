from adonthell import base

class cfgtest (object):
    def __init__ (self):
        self.cfg = base.configuration ()
        self.general ()
        self.audio ()
        self.video ()
        
    def general (self):
        print "Generating section 'General'"
        # -- low level method that will usually be used by the config file loader
        self.cfg.add_option ("General", "Language", base.cfg_option ("En_en"))
        self.cfg.add_option ("General", "Backend", base.cfg_option ("sdl"))
        self.cfg.add_option ("General", "Quick-Load", base.cfg_option ("1"))

    def video (self):
        print "Generating section 'Video'"
        # -- if the option does not exist, it will be created with the given default value
        self.cfg.get_int ("Video", "Fullscreen", 1)
        # -- low level access to the underlying data structure
        opt = self.cfg.option ("Video", "Fullscreen", base.cfg_option.CHOICE)
        # -- each option can have a comment attached to it
        opt.set_comment (" 0 for Window mode, 1 for Fullscreen mode ")
        
        self.cfg.get_int ("Video", "Resolution", 1)
        opt = self.cfg.option ("Video", "Resolution", base.cfg_option.CHOICE)
        opt.set_comment (" 0 for 320x240, 1 for 640x480 ")

    def audio (self):
        print "Generating section 'Audio'"
        # -- option with integer range as value
        opt = base.cfg_range ("175")
        # -- setting the range will automatically adapt value to sane setting
        opt.set_range (0, 100)
        self.cfg.add_option ("Audio", "Volume", opt)
        
        # -- option with a limited list of allowed settings
        opt = base.cfg_choice ("2")
        opt.add ("11025 Hz", "0")
        opt.add ("22050 Hz", "1")
        opt.add ("44100 Hz", "2")
        self.cfg.add_option ("Audio", "Sample-Rate", opt)
        
        opt = base.cfg_choice ("1")
        opt.add ("Mono", "0")
        opt.add ("Stereo", "1")
        self.cfg.add_option ("Audio", "Channels", opt)

        opt = base.cfg_choice ("1")
        opt.add ("8 bit", "0")
        opt.add ("16 bit", "1")
        self.cfg.add_option ("Audio", "Resolution", opt)

    def write (self):
        print "Writing configuration to %sadonthell.xml" % base.Paths.cfg_data_dir ()
        # -- save configuration to disk
        self.cfg.write ("adonthell")
        
    def read (self):
        print "Clearing configuration"
        self.cfg = base.configuration ()
        print "Loading configuration from %sadonthell.xml" % base.Paths.cfg_data_dir ()
        # -- load configuration from disk
        self.cfg.read ("adonthell")
    
    def string (self, print_all):
        print "Configuration contents (%s):" % str (print_all)  
        s = ""
        # -- return list of section names
        for sec in self.cfg.get_sections (print_all):
            # -- return list of option names within a section
            s += sec + ": " + str (self.cfg.get_options (sec, print_all)) + "\n"
        return s;
        
if __name__ == "__main__":
    # -- engine initialization
    base.Paths.init ("", "")

    ct = cfgtest ()
    ct.write ()
    # -- print only valid options (those with type != UNDEF)
    print ct.string (False)
    ct.read ()
    # -- print all options, valid or not (note: directly after loading, none will be valid)
    print ct.string (True)
    
