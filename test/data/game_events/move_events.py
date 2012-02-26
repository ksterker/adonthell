import sys

#
# This class implements test callbacks for time events. 
#
class move_test:
    def moving (self, lst, evt):
        sys.stdout.write(".")
        sys.stdout.flush()
