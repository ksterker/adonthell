/*
   $Id: adonthell.cc,v 1.5 2004/10/18 07:40:23 ksterker Exp $

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

#include "gfx/gfx.h"
#include "base/base.h"
#include "input/input.h"
#include "python/python.h"
#include "main/adonthell.h"

using namespace adonthell;
using std::cerr;
using std::endl;

/// The handler of our library file.
static lt_dlhandle dlhandle = 0;
            
/// pointer to method with backend/platform specific intialization code.
static bool (*init_p)(adonthell::app* application) = 0;

// dtor
app::~app ()
{
    if (dlhandle) lt_dlclose (dlhandle);
    lt_dlexit ();
}

// initialize engine subsystems 
bool app::init_modules (const u_int16 & modules)
{
    Modules = modules;
    
    // startup python
    if (modules & PYTHON)
    {
        // but not if we're called from a Python script
        if (!Py_IsInitialized ())
        {
            python::init ();
            PySys_SetArgv (Argc, Argv);
        }
        // avoid shutting down Python in that case
        else
        {
            Modules -= PYTHON;
        }
    }

    // startup graphics
    if (modules & GFX)
    {
        if (!gfx::init (Backend)) return false;
    }
    
    // startup input
    if (modules & INPUT)
    {
        if (!input::init (Backend)) return false;
    }
    
    // startup event system
    if (modules & EVENT)
    {
    }
    
    return true;
}

// read command line arguments
void app::parse_args (int & argc, char *argv[])
{
    int c;
    
    Argc = argc;
    Argv = argv;

    Game = "";
    Backend = "";
    Userdatadir = "";
    Config = "adonthell";

    // Check for options
    while ((c = getopt (argc, argv, "b:c:g:hv")) != -1)
    {        
        switch (c)
        { 
            // backend:
            case 'b':
                Backend = optarg;
                break;
            // configuration file:
            case 'c':
                Config = optarg;
                break;
            // user supplied data directory:
            case 'g':
                Userdatadir = optarg;
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
        Game = argv[argc-1];
    }
}

// initialize the Adonthell framework
bool app::init ()
{
    dlhandle = NULL;
    
    // init libltdl
    if (lt_dlinit ())
    {
        cerr << lt_dlerror() << endl;
        cerr << "Error initializing liblt!" << endl; 
        return false;
    }

    // init base module (required for reading config file)
    base::init (Userdatadir, Game);
        
    // load configuration file
    if (!Cfg.read (Config))
    {
        // print message if that fails, but don't panic yet ...
        cerr << "Error reading configuration '" << Config << ".xml'" << endl;
    }
    
    // if we have been given no backend on the command line, 
    // try to get it from config file; fallback to sdl if that fails
    if (Backend == "")
    {
        Backend = Cfg.get_string ("General", "Backend", "sdl");
        // set type of config option (free form text entry)
        Cfg.option ("General", "Backend", base::cfg_option::FREE);
    }
    
    // load backend init module
    dlhandle = base::get_module (string ("/main/") + Backend);
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
    
    // platform / backend specific initialization
    return init_p (this);
}

// shutdown the Adonthell framework
void app::cleanup () const
{
    // save configuration to disk
    Cfg.write (Config);

    // cleanup modules
	if (Modules & INPUT) input::cleanup ();
    if (Modules & GFX) gfx::cleanup ();
    if (Modules & PYTHON) python::cleanup ();
}

// display a help message
void app::print_help () const
{
    std::cout << "Usage: " << Argv[0] << " [OPTIONS] [GAME]" << endl;
    std::cout << endl;
    std::cout << "Where [OPTIONS] can be:\n";
    std::cout << "-b <backend>     specifiy the backend to use (default 'sdl')\n";
    std::cout << "-c <config>      use given config file (default 'adonthell')\n";
    std::cout << "-g <directory>   specify user game directory\n";
    std::cout << "-h               print this message and exit\n";
    std::cout << "-v               print engine version number and exit" << endl;
}
