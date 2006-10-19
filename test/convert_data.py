#
#  $Id: convert_data.py,v 1.3 2006/10/19 05:58:38 ksterker Exp $
#
#  (C) Copyright 2005/2006 Kai Sterker <kaisterker@linuxgames.com>
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
    GzWriter = None
    XmlWriter = None
    
    def __init__ (self):
        self.GzWriter = base.diskio (base.diskio.GZ_FILE)
        self.XmlWriter = base.diskio (base.diskio.XML_FILE)
    
    # -- check type of file being converted
    def file_type (self, filename):
        try:
            data = file (filename, "rb")
            line = data.readline ()
            if line[0] == '<': 
                print "Data file in XML format found"
                filetype =  base.diskio.XML_FILE
            else: 
                print "Data file in Binary format found"
                filetype = base.diskio.GZ_FILE
        except IOError:
            print "Error reading file", filename
            sys.exit(1)
            
        return filetype

    def run (self):
        ft = self.file_type (sys.argv[1])
        if ft == base.diskio.GZ_FILE:
            outf = sys.argv[1] + ".xml"
            
            if self.GzWriter.get_record (sys.argv[1]) == 1:
                self.XmlWriter.setBuffer (self.GzWriter.getBuffer(), self.GzWriter.size())
                self.XmlWriter.put_record (outf)
                
        if ft == base.diskio.XML_FILE:
            outf = sys.argv[1] + ".gz"
            
            if self.XmlWriter.get_record (sys.argv[1]) == 1:
                self.GzWriter.setBuffer (self.XmlWriter.getBuffer(), self.XmlWriter.size())
                self.GzWriter.put_record (outf)
    
# -- the 'main program'
if __name__ == '__main__':
    if len (sys.argv) != 2:
        print "usage: python convert_data.py <filename>"
        sys.exit(1)

    theApp = ConvertApp ()
    theApp.run ()
