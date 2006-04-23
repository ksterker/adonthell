#
#  $Id: convert_data.py,v 1.1 2006/04/23 17:14:29 ksterker Exp $
#
#  (C) Copyright 2005 Kai Sterker <kaisterker@linuxgames.com>
#  Part of the Adonthell Project http://adonthell.linuxgames.com
#
#  Adonthell is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  Adonthell is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with Adonthell; if not, write to the Free Software 
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#

from adonthell import base
import sys

class ConvertApp (object):
    UNKNOWN = 0
    BINARY = 1
    ASCII = 2
        
    # -- check type of file being converted
    def file_type (self, filename):
        filetype = self.UNKNOWN
        try:
            data = file (filename, "rb")
            line = data.readline ()
            if line[0] == '{': 
                print "Data file in ASCII format found"
                filetype =  self.ASCII
            else: 
                print "Data file in Binary format found"
                filetype = self.BINARY
        except IOError:
            print "Error reading file", filename
            sys.exit(1)
            
        return filetype

    def run (self):
        dkio = base.diskio ()
        ft = self.file_type (sys.argv[1])
        if ft == self.BINARY:
            inf = base.igzstream ()
            inf.open (sys.argv[1])
            outf = file (sys.argv[1] + ".txt", "wb")
            
            while not inf.eof ():
                if dkio.get_record (inf) == 1:
                    dkio.put_ascii (outf)
                    
            outf.close ()
            inf.close ()
    
# -- the 'main program'
if __name__ == '__main__':
    if len (sys.argv) != 2:
        print "usage: python convert_data.py <filename>"
        sys.exit(1)

    theApp = ConvertApp ()
    theApp.run ()
