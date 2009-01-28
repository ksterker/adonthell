%module main
%feature("autodoc", "1");   // enable docstrings for python wrappers

%{
#include <string>
#include "py_main.h"
%}

%include "stdint.i"
%include "std_string.i"
%include "base/types.h"
%include "main/adonthell.h"
%include "py_main.h"