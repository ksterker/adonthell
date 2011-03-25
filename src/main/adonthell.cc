/*
   Copyright (C) 2003/2004/2005 Kai Sterker <kaisterker@linuxgames.com>
   Copyright (C) 2010           Josh Glover <jmglov@jmglov.net>
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
 * @author Josh Glover <jmglov@jmglov.net>
 *
 * @brief  The main application class for programs using the Adonthell framework.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "gfx/gfx.h"
#include "base/nls.h"
#include "base/base.h"
#include "base/savegame.h"
#include "input/input.h"
#include "audio/audio.h"
#include "audio/audio_manager.h"
#include "python/python.h"
#include "world/world.h"
#include "main/adonthell.h"

using adonthell::app;

/// The handler of our library file.
static lt_dlhandle dlhandle = 0;

/// pointer to method with backend/platform specific intialization code.
static bool (*init_p)(adonthell::app* application) = 0;

/// pointer to main application class
app* app::theApp = NULL;

// ctor
app::app ()
{
    google::InstallFailureSignalHandler();
	theApp = this;
}

// dtor
app::~app ()
{
	theApp = NULL;
    if (dlhandle) lt_dlclose (dlhandle);
    lt_dlexit ();
}

// initialize engine subsystems
bool app::init_modules (const u_int16 & modules)
{
    LOG(INFO) << logging::indent() << "app::init_modules() called";
    logging::increment_log_indent_level();

    // don't initialize previously loaded modules
    u_int16 m = Modules ^ modules;
    Modules |= m;

    // startup python
    if (m & PYTHON)
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
        
        // update the python search path
        python::add_search_path (base::Paths().game_data_dir());
        python::add_search_path (base::Paths().user_data_dir());
    }

    // startup graphics
    if (m & GFX)
    {
        gfx::setup (Cfg);
        if (!gfx::init (Backend)) return false;
    }

    // startup input
    if (m & INPUT)
    {
        input::setup (Cfg);
        if (!input::init (Backend)) return false;
    }

    // startup audio
    if (m & AUDIO)
    {
        audio::setup (Cfg);
        if (!audio::init (Backend)) {
            LOG(ERROR) << logging::indent() << "audio::init() failed";

            logging::decrement_log_indent_level();
            return false;
        }
    }

    // init map stuff
    if (m & WORLD)
    {
        world::init (Cfg);
    }
    
    logging::decrement_log_indent_level();

    return true;
}

// read command line arguments
void app::parse_args (int & argc, char *argv[])
{
    google::InitGoogleLogging(argv[0]);

    LOG(INFO) << logging::indent() << "app::parse_args() called";
    logging::increment_log_indent_level();

    LOG(INFO) << logging::indent()
              << "Program '" << argv[0] << "' has "
              << argc << " args:"
        ;
    for (int i = 1; i < argc; i++)
        LOG(INFO) << logging::indent() << "[" << i << "]: '" << argv[i] << "'";

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
                LOG(INFO) << "found option '" << c << "': setting Backend to '"
                          << optarg << "'; was '" << Backend << "'";
                Backend = optarg;
                break;
            // configuration file:
            case 'c':
                LOG(INFO) << "found option '" << c << "': setting Config to '"
                          << optarg << "'; was '" << Config << "'";
                Config = optarg;
                break;
            // user supplied data directory:
            case 'g':
                LOG(INFO) << "found option '" << c << "': setting Userdatadir to '"
                          << optarg << "'; was '" << Userdatadir << "'";
                Userdatadir = optarg;
                break;
            // help message:
            case 'h':
                LOG(INFO) << "found option '" << c << "': printing help";
                print_help ();
                exit (0);
                break;
            // version number:
            case 'v':
                LOG(INFO) << "found option '" << c << "': printing version";
                std::cout << VERSION << std::endl;
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
        LOG(INFO) << "found GAME parameter: '" << Game << "'";
    }

    LOG(INFO) << logging::indent() << "Game: '"        << Game        << "'";
    LOG(INFO) << logging::indent() << "Backend: '"     << Backend     << "'";
    LOG(INFO) << logging::indent() << "Userdatadir: '" << Userdatadir << "'";
    LOG(INFO) << logging::indent() << "Config: '"      << Config      << "'";

    logging::decrement_log_indent_level();
}

// initialize the Adonthell framework
bool app::init ()
{
    Modules = 0;
    IsRunning = true;
    dlhandle = NULL;

    // init libltdl
    if (lt_dlinit ())
    {
        LOG(ERROR) << logging::indent() << lt_dlerror();
        LOG(ERROR) << logging::indent() << "Error initializing liblt!";

        return false;
    }

    // init base module (required for reading config file)
    if (!base::init (Game, Userdatadir))
    {
        LOG(ERROR) << logging::indent() << "No valid data directory found (use -g switch)";
        return false;
    }

    // read available saved games
    if (Game != "")
    {
        base::savegame::init ();
    }
    
    // load engine configuration file
    if (!Cfg.read (Config))
    {
        // print message if that fails, but don't panic yet ...
        LOG(ERROR) << logging::indent()
                   << "Error reading engine configuration '"
                   << Config << ".xml'"
            ;
    }

	/*
	 * FIXME: This requires some more thoughts ... how can the individual options
	 * be saved to correct file again? How can config options added to the proper
	 * file at runtime? etc ...
	 *
	// merge with game specific config file
	if (Game != "" && !Cfg.read (Game))
	{
        // no big deal if that fails; being verbose just in case ...
        LOG(ERROR) << logging::indent() << "Cannot read application specific configuration '" << Game << ".xml'";
	}
	 */
	
    // if we have been given no backend on the command line,
    // try to get it from config file; fallback to sdl if that fails
    if (Backend == "")
    {
        Backend = Cfg.get_string ("General", "Backend", "sdl");
        // set type of config option (free form text entry)
        Cfg.option ("General", "Backend", base::cfg_option::FREE);
    }

    // load backend init module
    dlhandle = base::get_module (string ("/main/_") + Backend);
    if (!dlhandle)
    {
        LOG(ERROR) << logging::indent() << lt_dlerror();
        return false;
    }

    // get pointer to init method
    init_p = (bool (*)(app*)) lt_dlsym (dlhandle, "main_init");
    if (!init_p)
    {
        LOG(ERROR) << logging::indent() << lt_dlerror();
        return false;
    }
    
    // init national language support
    base::nls::init (Cfg);

    // platform / backend specific initialization
    return init_p (this);
}

// shutdown the Adonthell framework
void app::cleanup () const
{
    // save configuration to disk
    Cfg.write (Config);

    // cleanup modules
    if (Modules & WORLD) world::cleanup ();
    if (Modules & AUDIO) audio::cleanup ();
    if (Modules & INPUT) input::cleanup ();
    if (Modules & GFX) gfx::cleanup ();
    if (Modules & PYTHON) python::cleanup ();
}

// display a help message
void app::print_help () const
{
    std::cout
        << "Usage: " << Argv[0] << " [OPTIONS] [GAME]" << std::endl
        << std::endl
        << "Where [OPTIONS] can be:"                                      << std::endl
        << "-b <backend>     specifiy the backend to use (default 'sdl')" << std::endl
        << "-c <config>      use given config file (default 'adonthell')" << std::endl
        << "-g <directory>   specify user game directory"                 << std::endl
        << "-h               print this message and exit"                 << std::endl
        << "-v               print engine version number and exit"        << std::endl
        ;
}
