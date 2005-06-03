from adonthell import rpg
from string import split
import sys

class searchtest (object):
    def __init__ (self):
        self.Idx = rpg.log_index ()

    # -- add above keys to index
    def add_key (self, key):
        self.Idx.add_key (key)

    # -- load a file from the library
    def load (self, name):
        path = sys.path[0] + "/data/library/"
        try:
            f = file (path + name, "r")
            text = f.read ()
            self.LogEntry = rpg.log_entry (name, text, name)
            f.close ()
        except:
            self.LogEntry = rpg.log_entry ("error", "file not found", "uid")
            
    # -- add the file just loaded to the index
    def index (self):
        self.Idx.add (self.LogEntry)

if __name__=='__main__':
    from timeit import Timer
    test = searchtest ()

    # -- ask for list of search terms
    terms = raw_input ("Specify list of search terms: ")
    num_terms = 0
    for term in split (terms):
        test.add_key (term)
        num_terms += 1
    
    # -- the texts for our fulltext search
    library = ["prologue.txt", "races.txt", "making.txt", "marring.txt", \
        "origins.txt", "alenvar.txt", "uzdunkal.txt", "riots.txt", \
        "falling.txt", "wastesedge.txt", "fang.txt"]
    
    # -- process contents of library
    for text in library:
        test.load (text)
        test.index ();
            
    # -- retrieve the keywords with entries attached
    keys = test.Idx.keys ()

    # -- make sure that all search terms have been found
    if len (keys) == num_terms:
        # -- find those texts containing all terms
        texts = []
        for key in keys: 
            texts.append (test.Idx.refs (key))
        print keys, "\n", texts
