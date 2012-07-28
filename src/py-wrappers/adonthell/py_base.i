%module base
%feature("autodoc", "1");   // enable docstrings for python wrappers

%{
#include <string>
#include <adonthell/base/nls.h>
#include <adonthell/base/base.h>
#include <adonthell/base/endians.h>
#include <adonthell/base/types.h>
#include <adonthell/base/diskio.h>
#include <adonthell/base/configuration.h>
#include <adonthell/base/savegame.h>

#include <adonthell/python/callback.h>

// partial specialization of base::serializer so it can be used from Python 
namespace base
{
    template<>
    serializer<PyObject>::serializer() : serializer_base()
    {
    }

    template<>
    serializer<PyObject>::serializer(PyObject *instance) : serializer_base()
    {
        PyObject *save = PyObject_GetAttrString (instance, "save");
        if (PyCallable_Check (save))
        {
            Save = new python::functor_1ret<const std::string &, bool>(save);
        }
        else
        {
            fprintf (stderr, "*** serializer: '%s' has no method 'save'!\n", instance->ob_type->tp_name);
        }
        Py_XDECREF (save);

        PyObject *load = PyObject_GetAttrString (instance, "load");
        if (PyCallable_Check (load))
        {
            Load = new python::functor_0ret<bool>(load);
        }
        else
        {
            fprintf (stderr, "*** serializer: '%s' has no method 'load'!\n", instance->ob_type->tp_name);
        }
        Py_XDECREF (load);
    }
}

extern "C" {
    void check_module_version (const char *name, const unsigned int & module_ver);
}
%}

%init %{
    check_module_version (SWIG_name, SWIGVERSION);
%}


%include "stdint.i"
%include "std_string.i"

// typemaps for the flat::next (...) method
namespace base {
    %typemap(in, numinputs = 0) (void **value, u_int32 *size, char **name) "
        void *value;
        char *name;
        u_int32 size;
        
        $1 = &value;
        $2 = &size;
        $3 = &name;
    "
    %typemap(argout) (void **value, u_int32 *size, char **name) {
        Py_XDECREF($result);
        PyObject *py_value;
        switch (result) {
            case base::flat::T_BOOL:
            case base::flat::T_UINT8:
            {
                py_value = PyInt_FromLong (*((u_int8*) *$1));
                break;
            }
            case base::flat::T_SINT8:
            {
                py_value = PyInt_FromLong (*((s_int8*) *$1));
                break;
            }
            case base::flat::T_SINT16:
            {
                py_value = PyInt_FromLong (*((s_int16*) *$1));
                break;
            }
            case base::flat::T_UINT16:
            {
                py_value = PyInt_FromLong (*((u_int16*) *$1));
                break;
            }
            case base::flat::T_SINT32:
            {
                py_value = PyLong_FromLong (*((s_int32*) *$1));
                break;
            }
            case base::flat::T_UINT32:
            {
                py_value = PyLong_FromUnsignedLong (*((u_int32*) *$1));
                break;
            }
            case base::flat::T_FLOAT:
            case base::flat::T_DOUBLE:
            {
                py_value = PyFloat_FromDouble (atof ((char *) *$1));
                break;
            }
            case base::flat::T_CHAR:
            case base::flat::T_STRING:
            case base::flat::T_BLOB:
            {
                py_value = PyString_FromStringAndSize ((char *) *$1, *$2);
                break;
            }
            case base::flat::T_FLAT:
            {
                base::flat *flat = new base::flat ((const char *) *$1, *$2);
                py_value = SWIG_NewPointerObj ((void *) flat, $descriptor(base::flat*), 1);
                break;
            }
            default:
            {
                return Py_BuildValue ("(iOOO)", result, Py_None, Py_None, Py_None);
            }
        }
        
        $result = Py_BuildValue ("(iOis)", result, py_value, *$2, (char *) *$3);
    }

    // typemap to let C++ have ownership of cfg_option* given to configuration::add_option ()
    %typemap(in) cfg_option *value "if ((SWIG_ConvertPtr ($input, (void **) &$1, $1_descriptor, SWIG_POINTER_EXCEPTION | SWIG_POINTER_DISOWN)) == -1) SWIG_fail;"

    %typemap(in) serializer_base *serializer "if ((SWIG_ConvertPtr ($input, (void **) &$1, $1_descriptor, SWIG_POINTER_EXCEPTION | SWIG_POINTER_DISOWN)) == -1) SWIG_fail;"
}

// typemap for passing a FILE* from Python
%typemap(in) FILE * { 
    if (!PyFile_Check ($input)) { 
        PyErr_SetString(PyExc_TypeError, "Need a file!"); 
        goto fail;
    } 
    $1 = PyFile_AsFile($input); 
} 


// typemap for returning a vector<const char*> as python list
%typemap(out) vector<const char*> {
    unsigned int index = 0;
    $result = PyList_New (result.size ());
    for (vector<const char*>::iterator i = result.begin (); i != result.end (); i++)
        PyList_SET_ITEM ($result, index++, PyString_FromString (*i));
}

%include <adonthell/base/base.h>
%include <adonthell/base/types.h>
%include <adonthell/base/timer.h>
%include <adonthell/base/file.h>
%include <adonthell/base/flat.h>
%include <adonthell/base/diskio.h>
%include <adonthell/base/configuration.h>
%include <adonthell/base/paths.h>
typedef long time_t;
%include <adonthell/base/savegame.h>
%include <adonthell/base/serializer.h>
%include <adonthell/base/logging.h>

%template(py_serializer) base::serializer<PyObject>;

%extend base::savegame {
    savegame (PyObject* callback)
    {
        return new base::savegame (new python::functor_1<const s_int32>(callback));
    }
}

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

%pythoncode %{
    Timer = cvar.Timer
%}
