%{
extern "C" {

#ifndef SWIGVERSION
#  ifndef SWIG_Python_GetTypeListHandle
#    define SWIGVERSION 0x010325
#    ifndef SWIG_Python_TypeQuery
#      define SWIG_Python_TypeQuery SWIG_TypeQuery
#    endif // SWIG_Python_TypeQuery
#  endif // SWIG_Python_GetTypeListHandle
#endif // SWIGVERSION

// code below is compatible with SWIG 1.3.24
#if (SWIGVERSION <= 0x010324)
    
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
    exit(1);
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
        exit(1);
    }
}

// code below is compatible with SWIG 1.3.25, up to at least 1.3.31 (or maybe not)
#else

// used to create sorted list of types without duplicates
struct type_collector
{
    type_collector (const char *n, type_collector *t = NULL) : name (n), next (t) { }
    ~type_collector () { delete next; }
    
    // insert new type
    void insert (const char *n)
    {
        type_collector *tc = this;
        while (tc->next != NULL)
        {
            int res = strcmp (tc->next->name, n);
            if (res == 0) return;
            else if (res > 0) break;
            // else continue;
            
            tc = tc->next;
        }
        
        tc->next = new type_collector (n, tc->next);
    }
    
    // print list of types
    void print ()
    {
        printf ("%s\n", name);
        
        for (type_collector *tc = next; tc != NULL; tc = tc->next)
            printf ("  - %s\n", tc->name);
    }
    
    const char *name;
    type_collector *next;
};

// collect types known to SWIG for debugging
static void collect_type_info (swig_module_info *typelist, type_collector & tc)
{
    swig_type_info **ti = typelist->types;
	for (unsigned int i = 0; i < typelist->size; i++)
	{
        tc.insert (SWIG_TypePrettyName (ti[i]));
    }
}

// call this to log all classes known to SWIG 
SWIGEXPORT void log_py_objects ()
{
    type_collector tc ("These types are known to SWIG:");
    
	// get global typelist
	swig_module_info *typelist = SWIG_Python_GetModule ();
	if (typelist != NULL)
	{
        // module info structure is organized as circular list
        swig_module_info *iter = typelist;
        do
        {
            collect_type_info (iter, tc);
            iter = iter->next;
        }
        while (iter != typelist);
    }
    
    tc.print ();
}

// pass a C++ object to Python
SWIGEXPORT PyObject *cxx_to_py (void *instance, const char *name, const bool & ownership)
{
    if (SWIG_Python_GetModule())
    {
        swig_type_info * tt = SWIG_Python_TypeQuery (name);
        if (tt) return SWIG_NewPointerObj (instance, tt, ownership);
    
        fprintf (stderr, "*** cxx_to_py: '%s' not found. ", name);
        log_py_objects ();
    }
    else
    {
        fprintf (stderr, "*** cxx_to_py: no Python module imported!\n");
    }
    
    exit(1);
}

// pass a Python object to C++
SWIGEXPORT void py_to_cxx (PyObject *instance, const char *name, void **retval)
{
    swig_type_info * tt = SWIG_Python_TypeQuery (name);
    if (tt == NULL || SWIG_ConvertPtr (instance, retval, tt, 0) == -1)
    {
        fprintf (stderr, "*** py_to_cxx: '%s' not found. ", name);
        log_py_objects();
        exit(1);
    }
}

#endif // SWIGVERSION <= 0x010324
}

%}
