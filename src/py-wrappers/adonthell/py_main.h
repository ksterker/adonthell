
#include "main/adonthell.h"
#include "python/callback.h"

class AdonthellApp : public adonthell::app 
{
public:
    AdonthellApp ()
    {
        MainFunc = NULL;
    }
    
    virtual ~AdonthellApp ()
    {
        delete MainFunc;
    }
    
    int main () const 
    { 
        if (!MainFunc) 
        {
            printf ("*** py_main: no callback to python script given! Exiting ...");
            return 1;
        }
        
        return (*MainFunc)();
    }
    
    void init (PyObject *main_func) 
    {
        MainFunc = new python::functor_0ret<int> (main_func);
        adonthell::app::init ();
    }
    
private:
    python::functor_0ret<int> *MainFunc;
    
} theApp;
