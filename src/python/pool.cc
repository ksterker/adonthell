/*
   $Id: pool.cc,v 1.4 2005/06/03 17:29:13 ksterker Exp $

   Copyright (C) 2003 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file   python/pool.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Allow sharing of python class instances between multiple objects.
 */

#include "pool.h"

using python::method;
using python::pool;

std::map<std::string, python::script*> pool::Pool;

void pool::init () 
{
}

void pool::cleanup () 
{
    std::map<std::string, python::script*>::iterator i;

    for (i = Pool.begin (); i != Pool.end (); i++)
        delete (*i).second;
	
	Pool.clear ();
}

method *pool::connect (const std::string & file, const std::string & classname, const std::string & callback)
{
    python::script *scrpt = reconnect (file, classname);
    if (scrpt != NULL) return new method (scrpt, callback);
    
    return NULL;
}

python::script *pool::reconnect (const std::string & file, const std::string & classname)
{
    std::map<std::string, python::script*>::const_iterator i;
    std::string name = file + classname;
    
    // script not in pool yet
    if ((i = Pool.find (name)) == Pool.end ())
    {
        python::script *scrpt = new python::script ();
        
        // instanciate
        if (!scrpt->create_instance (file, classname, NULL))
        {
            fprintf (stderr, "*** pool::reconnect: failed instanciating class %s in file %s!\n", 
                classname.c_str (), file.c_str ());
            
            delete scrpt;
            return NULL;
        }
        
        // add script to pool
        Pool[name] = scrpt;
        return scrpt;
    }
    // script already in pool
    else
    {
        return (*i).second;
    }
}
