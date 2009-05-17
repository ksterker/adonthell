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
}

%}


void log_py_objects ();