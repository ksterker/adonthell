/*
   $Id: script.h,v 1.11 2009/04/08 19:36:02 ksterker Exp $

   Copyright (C) 1999/2000/2001/2003/2006 Kai Sterker <kaisterker@linuxgames.com>
   Copyright (C) 2001 Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   Adonthell is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Adonthell is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Adonthell; if not, write to the Free Software 
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/


/**
 * @file   python/script.h
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Makes a python script available to C++ code.
 */


#ifndef PYTHON_SCRIPT_H
#define PYTHON_SCRIPT_H
                   
#include "python.h"

namespace python 
{
    /**
    * Python script class.
    *
    * Use this class to create instances of Python classes contained in Python
    * modules, then control their execution. You can pass an argument tuple to
    * the class constructor and to any method you want to run. It is further
    * possible to access and change attributes of the Python instance.
    * 
    */ 
    class script
    {
    public:
        /** 
         * Default constructor.
         * 
         */
        script ();
        
        /** 
         * Destructor.
         * 
         */
        virtual ~script (); 
    
        /** 
         * Resets the script to its post-constructor state.
         * 
         */
        void clear (); 
    
        /**
         * @name PyObject creation
         */
        //@{
        /** 
         * Creates an instance of a Python class.
         * 
         * @param file file name of the module to use.
         * @param classname name of the class to import.
         * @param args Python tuple containing the arguments to pass to the
         *             Python class constructor.
         */
        bool create_instance (const string & file, const string & classname, PyObject * args = NULL);
    
        /**
         * Similar to create_instance, except that it will reload the module
         * from disk, in case it has been changed in the meantime. Mainly interesting
         * for script development or tools like dlgedit.
         *
         * @param file file name of the module to use.
         * @param classname name of the class to import.
         * @param args Python tuple containing the arguments to pass to the
         *             Python class constructor.
         */
        bool reload_instance (const string & file, const string & classname, PyObject * args = NULL);
        //@}
    
        /**
         * @name PyObject method calling
         */
        //@{
        /** 
         * Call a method of this object.
         * 
         * @param name name of the method to call.
         * @param args Python tuple containing the arguments to pass to the method.
         * @return the return value of the method as PyObject. Needs to be 
         *     Py_DECREF'd when no longer needed.
         */
        PyObject *call_method_ret (const string &name, PyObject *args = NULL) const;
        
        /** 
         * Call a method of this object.
         * 
         * @param name name of the method to call.
         * @param args Python tuple containing the arguments to pass to the method.
         */
        void call_method (const string & name, PyObject * args = NULL) const
        {
            PyObject *result = call_method_ret (name, args);
            Py_XDECREF (result);
        }
        //@}
    
        /**
         * @name PyObject member access
         */
        //@{
        /**
         * Tests whether the object contains a certain attribute (i.e. method
         * or variable).
         *
         * @param name Name of the attribute to test for
         * @return <b>true</b> if the attribute exists, <b>false</b> otherwise.
         */
        bool has_attribute (const string & name) const;
    
        /**
         * Returns a new reference to an attribute of this object.
         *
         * @param name Name of the attribute to access
         * @return New reference to the attribute or NULL on error
         */
        PyObject* get_attribute (const string & name) const;
    
        /**
         * Returns the given attribute as integer value.
         *
         * @param name Name of the attribute to access
         * @return An integer.
         */
        s_int32 get_attribute_int (const string & name) const;
    
        /**
         * Returns the given attribute as string value.
         *
         * @param name Name of the attribute to access
         * @return A string.
         */
        std::string get_attribute_string (const string & name) const;
    
        /**
         * Assign a new attribute to the module, overriding an existing
         * attribute of the same name.
         *
         * @param name The attribute's name
         * @param value The attribute's value
         */
        void set_attribute (const string & name, PyObject *value);
    
        /**
         * Assign a new integer attribute to the module, overriding an
         * existing attribute of the same name.
         *
         * @param name The attribute's name
         * @param value The attribute's value
         */
        void set_attribute_int (const string & name, s_int32 value);
    
        /**
         * Assign a new string attribute to the module, overriding an
         * existing attribute of the same name.
         *
         * @param name The attribute's name
         * @param value The attribute's value
         */
        void set_attribute_string (const string & name, const string & value);
        //@}
        
        /**
         * @name Member access
         */
        //@{
        /**
         * Direct access to the instance object. The default behaviour is to
         * increase the instance's reference count, so that this method can
         * be safely called from Python scripts.
         *
         * @param incref whether to increase the reference count.
         * @return the Python class instance.
         */
        PyObject *get_instance (const bool & incref = true) const
        {
            if (incref) Py_XINCREF (Instance);
            return Instance;
        }
    
        /**
         * Direct access to the constructor arguments. Does not increase
         * the reference count and thus should not be called from Python.
         * @return the arguments passed to the constructor.
         */
        PyObject *get_args () const
        {
            return Args;
        }
        
        /**
         * Returns the class name of this object. This is the name of the
         * wrapped Python class.
         *
         * @return class name of this object.
         */
        std::string class_name () const
        {
            return Classname;
        }
    
        /**
         * Returns the file name of this object. This is the name of the
         * Python module containing the wrapped class.
         *
         * @return fiöe name of this object.
         */
        std::string file_name () const
        {
            return Filename;
        }
        //@}
        
        /**
         * @name Loading / Saving
         */
        //@{
        /** 
         * Save the %script to a stream.
         * @param record stream where to save the %script.
         */ 
        void put_state (base::flat& record) const;
        
        /** 
         * Loads the %script from a stream.
         * @param record stream to load the %script from.
         * @return \e true if the %script was loaded successfully, \e false otherwise.
         */
        bool get_state (base::flat& record);
        //@}
                
#ifndef SWIG
        /// allow script to be passed through SWIG
        GET_TYPE_NAME_VIRTUAL(python::script);
#endif // SWIG
    
    protected:
        /// The python class instance wrapped by %script
        PyObject *Instance;
    
    private:
        /// Helper for create_instance and reload_instance
        bool instanciate (PyObject*, const std::string &, const std::string &, PyObject*);
    
        /// The class name of the current script
        std::string Classname;
    
        /// The file name of the current script
        std::string Filename;
        
        /// The arguments passed to script ctor
        PyObject *Args;
    };
}
#endif // PYTHON_SCRIPT_H
