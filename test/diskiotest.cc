#include "base/diskio.h"
#include <iostream>

using std::cout;
using std::endl;

float  f_PI = 3.14159265358979323846;
double d_PI = 3.14159265358979323846;

int main (int argc, char* argv[]) {

    base::diskio test;
    base::flat fl;
    
    // adding all different kind of data
    cout << "Packing ..." << endl;
    test.put_bool ("b", false);
    test.put_char ("c", 'a');
    test.put_uint8 ("u8", 255);
    test.put_sint8 ("s8", -127);
    test.put_uint16 ("u16", 65535);
    test.put_sint16 ("s16", -32767);
    test.put_uint32 ("u32", 4294836225);
    test.put_sint32 ("s32", -2147418112);
    test.put_string ("s", "abc ... xyz");
    test.put_float ("f", f_PI);
    test.put_double ("d", d_PI);
    test.put_block ("block", new char[256], 256);
    
    // a flat can contain other flats too
    fl.put_string ("string", "Another flat");
    fl.put_flat ("test", test);
    test.put_flat ("flat", fl);
    
    // get size and checksum
    cout << "Everything packed ... " << test.size () << " bytes used" << endl;
    cout << "Checksum: " << (std::hex) << test.checksum () << (std::dec) << endl;
    
    // write record to disk
    cout << "Writing data to disk ..." << endl;
    base::ogzstream out ("/tmp/diskio.test");
    test.put_record (out);
    out.close ();
    
    // read record from disk
    cout << "Reading data from disk" << endl;
    base::igzstream in ("/tmp/diskio.test");
    bool b = test.get_record (in);
    in.close ();
    if (b == true) cout << "Reading successful" << endl;
    
    // unpack all kind of data using get_*
    // this may happen in any order, although using the original
    // order is much more efficient.
    cout << "Unpacking ..." << endl;
    cout << test.get_bool ("b") << endl;
    cout << test.get_char ("c") << endl;
    cout << test.get_uint16 ("u16") << endl;
    cout << test.get_string ("s") << endl;
    cout << test.get_uint32 ("u32") << endl;
    cout << test.get_sint32 ("s32") << endl;
    cout << (int) test.get_sint8 ("s8") << endl;
    cout << (int) test.get_uint8 ("u8") << endl;
    cout << test.get_sint16 ("s16") << endl;
    printf ("%.24f\n", test.get_float ("f"));
    printf ("%.48f\n", test.get_double ("d"));
    delete[] (char *) test.get_block ("block");

    // get included flat using get_flat; will have to delete it later
    base::flat *f = test.get_flat ("flat");
    cout << f->get_string ("string") << endl;
    
    void *value;
    int type, size;
    
    // the next() method is another way to extract included flats
    if (f->next (&value, &size) == base::flat::T_FLAT) {
        base::flat f2 ((const char *) value, size);
        
        // next fetches values in the order they where added, 
        // until it reaches the end; you'll have to cast them
        // manually to the type it returns
        while ((type = f2.next (&value)) != -1)
            cout << type << " ";
    }
    cout << "\nEverything unpacked" << endl;
    
    delete f;
    return 0;
}
