from adonthell import base, main, world

class App (main.AdonthellApp):
    def test_cube (self):
        # -- create cube
        cube = world.cube3 (10, 10, 10)
        
        # -- save cube
        record = base.diskio (base.diskio.XML_FILE)
        cube.put_state (record)
        record.put_record ("cube.xml")

        # -- load cube
        record.get_record ("cube.xml")
        cube.get_state (record);
        
    def run (self):
       self.test_cube ()
       return 0

if __name__ == '__main__':
    myApp = App ()
    myApp.init (myApp.run)
