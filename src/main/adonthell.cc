/*
   $Id: adonthell.cc,v 1.1 2003/11/22 09:37:39 ksterker Exp $

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
 * @file   main/adonthell.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  The main application class for programs using the Adonthell framework.
 * 
 * 
 */
 
#include <iostream>

#include "base/paths.h"
#include "main/adonthell.h"

using namespace adonthell;
using std::cerr;
using std::endl;

bool (*app::init_p)(app*) = NULL;
lt_dlhandle app::dlhandle = 0;
string app::backend = "sdl";

void app::parse_args (int & ac, char *av[])
{
    argc = ac;
    argv = av;

    int c;
    
    // Check for options
    while ((c = getopt (argc, argv, "b:")) != -1)
    {        
        switch (c)
        { 
            // backend:
            case 'b':
                backend = optarg;
                break;
            default:
                break;
        }
    }
}

// initialize the Adonthell framework
bool app::init ()
{
    // init libltdl
    if (lt_dlinit ())
    {
        cerr << lt_dlerror() << endl;
        cerr << "Error initializing liblt!" << endl; 
        return false;
    }
    
    // load backend init module
    dlhandle = base::get_module (string ("/main/") + backend);
    if (!dlhandle)
    {
        cerr << lt_dlerror() << endl;
        return false;
    }
    
    // get pointer to init method
    init_p = (bool (*)(app*)) lt_dlsym (dlhandle, "main_init");
    if (!init_p)
    {
        cerr << lt_dlerror() << endl;
        return false;
    }
    
    return init_p (this);
}

// shutdown the Adonthell framework
void app::cleanup ()
{
    if (dlhandle) lt_dlclose (dlhandle);
    lt_dlexit ();
}
