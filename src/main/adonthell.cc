/*
   $Id: adonthell.cc,v 1.3 2004/08/02 07:35:28 ksterker Exp $

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
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <unistd.h>

#include "base/base.h"
#include "main/adonthell.h"

using namespace adonthell;
using std::cerr;
using std::endl;

// read command line arguments
void app::parse_args (int & ac, char *av[])
{
    int c;
    
    argc = ac;
    argv = av;

    backend = "";
    userdatadir = "";
    config = "adonthell";

    // Check for options
    while ((c = getopt (argc, argv, "b:c:g:hv")) != -1)
    {        
        switch (c)
        { 
            // backend:
            case 'b':
                backend = optarg;
                break;
            // configuration file:
            case 'c':
                config = optarg;
                break;
            // user supplied data directory:
            case 'g':
                userdatadir = optarg;
                break;
            // help message:
            case 'h':
                print_help ();
                exit (0);
                break;
            // version number:
            case 'v':
                std::cout << VERSION << endl;
                exit (0);
                break;
            default:
                break;
        }
    }
    
    // check whether the GAME parameter is given
    if (argc - optind == 1)
    {
        game = argv[argc-1];
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
    
    // load configuration file
    if (!cfg.read (config))
    {
        // print message if that fails, but don't panic yet ...
        cerr << "Error reading configuration '" << config << ".xml'" << endl;
    }
    
    // if we have been given no backend on the command line, 
    // try to get it from config file; fallback to sdl if that fails
    if (backend == "")
    {
        backend = cfg.get_string ("General", "Backend", "sdl");
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
    
    // init base module
    base::init (userdatadir, game);
    
    // platform / backend specific initialization
    return init_p (this);
}

// shutdown the Adonthell framework
void app::cleanup () const
{
    // save configuration to disk
    cfg.write (config);
    
    if (dlhandle) lt_dlclose (dlhandle);
    lt_dlexit ();
}

// display a help message
void app::print_help () const
{
    std::cout << "Usage: " << argv[0] << " [OPTIONS] GAME" << endl;
    std::cout << endl;
    std::cout << "Where [OPTIONS] can be:\n";
    std::cout << "-b <backend>     specifiy the backend to use (default 'sdl')\n";
    std::cout << "-c <config>      use given config file (default 'adonthell')\n";
    std::cout << "-g <directory>   specify user game directory\n";
    std::cout << "-h               print this message and exit\n";
    std::cout << "-v               print engine version number and exit" << endl;
}
