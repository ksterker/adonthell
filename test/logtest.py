from adonthell import rpg
from string import count
import sys

class indextest (object):
    # -- Keys to add to index
    Keys = ["Adonthell", "Aerengist", "Albrecht", "Alenvar", "Andare", "Andomiel", \
        "Belerinath", "Bjarn Fingolson", "Cirades Rymon", "Cirdanth", "Elenstroem", \
        "Elgilad", "Elissathal", "Erinsford", "Farandale", "Foroduin", "Gemthain", \
        "Gemthain Mountain", "Gildhammer", "Guild of Arts", "Helmforge", "Hillgirth", \
        "Isgil", "Lhuzdu'rok", "Marglir Evenstar", "Menagond", "Mina Turvale", \
        "Mountain in the Sea", "Glittering Caves", "Mountains of Dawn", "Mountains of Making", \
        "Oreth-Arinyae", "Ringforge", "Risartus Solmanus", "Riverthain", "Southhome", \
        "Stonetorch", "Sulin Turvale", "Szhum'lhuzdu", "Szhum'szhalrok", "Telimae", \
        "Thalin", "Throatgird", "Thukhuzdun'szhalrok", "Uzdun'kal", "Uzthut'szhum", \
        "Venag Danir", "Vertas", "Waste's Edge", "Whetstone", "Wyvernesse"]
    
    def __init__ (self):
        self.Idx = rpg.log_index ()

    # -- add above keys to index
    def add_keys (self):
        for key in self.Keys:
            self.Idx.add_key (key)

    # -- load a file from the library
    def load (self, name):
        print "Loading", name, "...",
        path = sys.path[0] + "/data/library/"
        try:
            f = file (path + name, "r")
            text = f.read ()
            words = count (text, " ") + 1
            self.LogEntry = rpg.log_entry (name, text, "uid")
            f.close ()
            print "done"
            print "Read ~", words, "words with a total of", len (text), "bytes"
        except:
            print "failed"
            self.LogEntry = rpg.log_entry ("error", "file not found", "uid")
            
    # -- add the file just loaded to the index
    def index (self):
        self.Idx.add (self.LogEntry)

if __name__=='__main__':
    from timeit import Timer
    test = indextest ()
    total = 0.0
    
    print "51 Keys added in",
    t = Timer ("test.add_keys ()", "from __main__ import test")
    total += t.timeit (1)
    print total, "seconds"

    library = ["prologue.txt", "races.txt", "making.txt", "marring.txt", \
        "origins.txt", "alenvar.txt", "uzdunkal.txt", "riots.txt", \
        "falling.txt", "wastesedge.txt", "fang.txt"]
    
    for text in library:
        print
        test.load (text)
        print text, "indexed in",
        t = Timer ("test.index ()", "from __main__ import test")
        time = t.timeit (1)
        total += time
        print time, "seconds"
    
    t = Timer ("test.Idx.keys ()", "from __main__ import test")
    print "\nCalculating keys in", t.timeit (1), "seconds"
    print "Retrieving keys from cache in", t.timeit (1), "seconds"
    keys = test.Idx.keys ()
    print keys
    print "Total time:", total, "seconds\nNot indexed:",
    for key in test.Keys:
        if key not in keys:
            print key, ",",
    print