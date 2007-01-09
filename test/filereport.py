from adonthell import base
import sys

str_type = ["bool", "char", "u_int8", "s_int8", "u_int16", "s_int16", "u_int32", \
            "s_int32", "string", "float", "double", "blob", "flat"]

hex_vals = "0123456789ABCDEF"

class file_report (object):
    def string_to_hex (self, str):
        result = ""
        for letter in str[:11]:
            val = ord (letter)
            result += hex_vals[val / 16]
            result += hex_vals[val % 16]
            result += " "
        return result

    # -- check type of file being converted
    def file_type (self, filename):
        try:
            data = file (filename, "rb")
            line = data.readline ()
            if line[0] == '<': 
                print "Data file in XML format found"
                filetype = base.diskio.XML_FILE
            else: 
                print "Data file in Binary format found"
                filetype = base.diskio.GZ_FILE
        except IOError:
            print "Error reading file", filename
            sys.exit(1)
            
        return filetype

    def print_record (self, flt, efn):
        efn = efn + 1
        type, value, length, field = flt.next ()
        while type != -1:
            if type == base.flat.T_FLAT: 
                value_str = "<Embedded flat '" + field + "'>"
                field = "-----> %3i" % efn
            elif type == base.flat.T_BLOB: value_str = self.string_to_hex (value)
            else: value_str = str(value)
            
            if len (value_str) > 30: value_str = value_str[0:27] + "..."
            print "%-10s  %-8s  %-5i  %-30s" % (field, str_type[type], length, value_str)
    
            if type == base.flat.T_FLAT:
                self.print_record (value, efn)
                print "<----- %3i  --------  -----  ------------------------------" % efn
                efn = efn + 1
            type, value, length, field = flt.next ()
        
    def run (self):
        # -- check that we've got a file to report on
        if len (sys.argv) != 2:
            print "usage: python filereport.py <filename>"
            return
        
        # -- get filename
        filename = sys.argv[1]
        
        # -- open file of detected type
        record = base.diskio (self.file_type (filename))
        
        # -- load contents of file
        record.get_record (filename)
        print "Checksum =", hex (record.checksum())
        print "Buffer =", record.to_string ()
 
        print "Field       Type      Len    Value"
        print "----------  --------  -----  ------------------------------"
        
        # -- print contents recursively to stdout
        self.print_record (record, 0)
                
        print "==========  ========  =====  =============================="

if __name__ == '__main__':
    report = file_report ()
    report.run ()
