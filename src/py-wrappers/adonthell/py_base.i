%module base
%{

#include <string>
#include "base/types.h"
#include "base/timer.h"
#include "base/diskio.h"
%}

%include "std_string.i"

%include "base/types.h"
%include "base/timer.h"
%include "base/file.h"
%include "base/flat.h"
%include "base/diskio.h"


/* implement friend operators of igzstream */
%extend base::igzstream {
    /// Reads a boolean.
    bool get_bool ()
    {
        u_int8 b;
        b << *self;
        return b;
    }
        
    /// Reads a u_int8.
    u_int8 get_uint8 ()
    {
        u_int8 n;
        n << *self;
        return n;
    }
    
    /// Reads a s_int8.
    s_int8 get_sint8 ()
    {
        s_int8 n;
        n << *self;
        return n;
    }
    
    /// Reads a u_int16.
    u_int16 get_uint16 ()
    {
        u_int16 n;
        n << *self;
        return n;
    }
    
    /// Reads a s_int16.
    s_int16 get_sint16 ()
    {
        s_int16 n;
        n << *self;
        return n;
    }
    
    /// Reads a u_int32.
    u_int32 get_uint32 ()
    {
        u_int32 n;
        n << *self;
        return n;
    }
    
    /// Reads a s_int32.
    s_int32 get_sint32 ()
    {
        s_int32 n;
        n << *self;
        return n;
    }
    
    /// Reads a string.
    string get_string ()
    {
        string s;
        s << *self;
        return s;
    }
    
    /// Reads a float.
    float get_float ()
    {
        float f;
        f << *self;
        return f;
    }
}


/* implement friend operators of ogzstream */
%extend base::ogzstream 
{
    void put_bool (const bool &b)         { b >> *self; }
    void put_uint8 (const u_int8 &n)      { n >> *self; }
    void put_sint8 (const s_int8 &n)      { n >> *self; }
    void put_uint16 (const u_int16 &n)    { n >> *self; }
    void put_sint16 (const s_int16 &n)    { n >> *self; }
    void put_uint32 (const u_int32 &n)    { n >> *self; }
    void put_sint32 (const s_int32 &n)    { n >> *self; }
    void put_string (const string& s)     { s >> *self; }
    void put_float (const float &f)       { f >> *self; }
}        
