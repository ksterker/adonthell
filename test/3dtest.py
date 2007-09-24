from adonthell import base, main, gfx, world

class App (main.AdonthellApp):
    
    def test_cube_io (self):
        """Test saving and loading of a cube"""
        
        # -- create cube
        cube = world.cube3 (100, 100, 40)
        
        # -- save cube
        record = base.diskio (base.diskio.XML_FILE)
        cube.put_state (record)
        record.put_record ("cube.xml")

        # -- load cube
        record.get_record ("cube.xml")
        cube.get_state (record);
        
        return cube
    
    def draw_mesh (self, cube):
        """Test whether mesh for cube is created correctly, i.e with all faces pointing outwards"""
        
        cube.create_mesh ()
        cube.draw_mesh (220, 140)
        
        gfx.screen_update()
        base.Timer.sleep (25000)
        
    def run (self):
        """Main test method"""
        
        self.init_modules (self.GFX)
        gfx.screen_set_video_mode (640, 480)
        
        cube = self.test_cube_io ()
        self.draw_mesh (cube)
        
        return 0

if __name__ == '__main__':
    myApp = App ()
    myApp.init (myApp.run)
