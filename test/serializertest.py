from adonthell import base, main

class testserializer(main.AdonthellApp):
        
    def save (self, name):
        print "saving state to", name, "..."
	return 1

    def main (self):
	s = base.py_serializer(self)
	s.save ("test")
	s.load ()
        
        return 0

if __name__ == '__main__':
    app = testserializer()
    app.init (app.main)

