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

    def print_record (self, flt):
        type, value, length, field = flt.next ()
        while type != -1:
            if type == base.flat.T_FLAT: value_str = "<Embedded flat>"
            elif type == base.flat.T_BLOB: value_str = self.string_to_hex (value)
            else: value_str = str(value)
            
            if len (value_str) > 30: value_str = value_str[0:27] + "..."
            print "%-10s  %-8s  %-5i  %-30s" % (field, str_type[type], length, value_str)
    
            if type == base.flat.T_FLAT: 
                self.print_record (value)
                print "----------  --------  -----  ------------------------------"
            type, value, length, field = flt.next ()
        
    def run (self):
        if len (sys.argv) != 2:
            print "usage: python filereport.py <filename>"
            return
            
        filename = sys.argv[1]
        file = base.igzstream ()
        if not file.open (filename):
            print "Error opening file '%s'" % filename
            return
        
        record_num = 1
        record = base.diskio ()
    
        while not file.eof ():
            if not record.get_record (file):
                print "Error opening record #%i" % record_num
            else:
                print "Field       Type      Len    Value"
                print "----------  --------  -----  ------------------------------"
                self.print_record (record)
                record_num += 1
                
        print "==========  ========  =====  =============================="

if __name__ == '__main__':
    report = file_report ()
    report.run ()
