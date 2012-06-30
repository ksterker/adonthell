%module debug

%{

extern "C" 
{
	/**
	 * Print list of types known to SWIG. This can be used
	 * for debugging purpose to find out which modules and
	 * classes are available. 
	 */
	void log_py_objects ();

    void check_module_version (const char *name, const unsigned int & module_ver);
}
%}

%init %{
    check_module_version (SWIG_name, SWIGVERSION);
%}


void log_py_objects ();