%{
extern "C" {

// print types known to SWIG for debugging
static void print_type_info (swig_type_info **typelist)
{
	for (swig_type_info *ti = *typelist; ti != NULL; ti = ti->prev)
	{
		const char *name = SWIG_TypePrettyName (ti);
		fprintf (stderr, "  - %s\n", name);
	}
}

// call this to log all classes known to SWIG 
void log_py_objects ()
{
	// get global typelist
	swig_type_info ** typelist = SWIG_Python_GetTypeListHandle();
	if (*typelist != NULL)
	{
		fprintf (stderr, "These types are known to SWIG:\n");
		print_type_info (typelist);
	}
}

// pass a C++ object to Python
PyObject *cxx_to_py (void *instance, const char *name, const bool & ownership)
{
	// get global typelist
	swig_type_info ** typelist = SWIG_Python_GetTypeListHandle();
	
    swig_type_info * tt = SWIG_TypeQueryTL (*typelist, name);
    if (tt) return SWIG_NewPointerObj (instance, tt, ownership);
        
    fprintf (stderr, "*** cxx_to_py: '%s' not found in SWIGs typelist:\n", name);
	print_type_info (typelist);
    return NULL;
}

// pass a Python object to C++
void py_to_cxx (PyObject *instance, const char *name, void **retval)
{
	// get global typelist
	swig_type_info ** typelist = SWIG_Python_GetTypeListHandle ();
	
    swig_type_info * tt = SWIG_TypeQueryTL (*typelist, name);
    if (tt == NULL || SWIG_ConvertPtr (instance, retval, tt, 0) == -1)
    {
        fprintf (stderr, "*** py_to_cxx: '%s' not found in SWIGs typelist:", name);
		print_type_info (typelist);
    }
}

}

%}