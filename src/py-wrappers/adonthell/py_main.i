%module main
%feature("autodoc", "1");   // enable docstrings for python wrappers

%{
#include <string>
#include "py_main.h"

extern "C" {
    void check_module_version (const char *name, const unsigned int & module_ver);
}
%}

%init %{
    check_module_version (SWIG_name, SWIGVERSION);
%}

%include "stdint.i"
%include "std_string.i"
%include "base/types.h"
%include "main/adonthell.h"
%include "py_main.h"