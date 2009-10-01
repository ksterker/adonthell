from adonthell import base, main

class testserializer(main.AdonthellApp):
        
    def save (self, name):
        print "saving state to", name, "..."
        return 1

    def progress (self, current):
        print "+",
        
    def main (self):
        # -- register serializer
        base.savegame.add (base.py_serializer(self))
        
        # -- create savegame instance
        sg = base.savegame (self.progress)
        
        # -- save
        sg.load (base.savegame.INITIAL_SAVE)
        sg.save (base.savegame.QUICK_SAVE, "Test", 0)
        
        # -- list existing save games
        for index in range (0, sg.count()):
            data = sg.get (index)
            print "*", data.directory (), data.last_modified()
        
        return 0

if __name__ == '__main__':
    app = testserializer()
    app.init (app.main)

