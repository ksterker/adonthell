/*
   $Id: adonthell.h,v 1.2 2004/08/02 07:35:28 ksterker Exp $

   Copyright (C) 2003/2004 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file   main/adonthell.h 
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  The main application class for programs using the Adonthell framework.
 * 
 * 
 */

#ifndef ADONTHELL_APP_H
#define ADONTHELL_APP_H

#include "ltdl.h"
#include "base/configuration.h"

using std::string;

namespace adonthell {
    /**
     * Subclass adonthell::app to create the entry point for any application
     * using the Adonthell framework. The class you create must be named theApp
     * in order to be found by the linker. Override the main() method. It will
     * be called after all operating system dependend intialization has been
     * completed. You will find the command line args passed to your application
     * in the member variables argc and argv.
     */
    class app {
        public:
            /**
             * Destructor.
             */
            virtual ~app () { }
            
            /**
             * This method is called after the engine has been initialized.
             * Overwrite it to add your game's main loop. Once this method
             * returns, the engine will quit.
             */
            virtual int main () const = 0;
        
            /**
             * Called by the engine before main(). Used to do platform specific
             * initialization work. What needs to be done depends mainly on the
             * backend and OS used. For example, SDL on both MacOS X and Win32
             * needs to 'connect' to the windowing system before it can display
             * any windows or recieve input.
             * This method does not initialize any subsystems of the Adonthell
             * engine itself. Do that in your main() method.  
             */
            bool init ();
            
            /**
             * Parse the given command line arguments for those required by
             * the engine. The following arguments are currently handled:
             * -b <backend> 	specifiy the backend to use (default 'sdl')
             * -c <config>		use given config file (default 'adonthell')
             * -g <directory> 	specify user game directory
             * -h				print help message and exit
             * -v 				print version number and exit
             *
             * @param argc argument count
             * @param argv argument vector
             */
            void parse_args (int & argc, char *argv[]);
            
            /**
             * Called automatically on shutdown of the Adonthell engine. Frees
             * any resources aquired by init(). Don't call it manually. 
             */
            void cleanup () const;

        protected:
            /// Commandline argument count.
            int argc;
            
            /// Commandline argument vector.
            char **argv;

            /// Engine configuration
            base::configuration cfg;
            
        private:
            /**
             * Print short usage message to cout.
             */
            void print_help () const;
        
            /// The handler of our library file.
            lt_dlhandle dlhandle;

            /// pointer to method with backend/platform specific intialization code.
            bool (*init_p)(app* application);
            
            /// the backend we're using
            string backend;
            
            /// the config file we're loading
            string config;
            
            /// user supplied data directory
            string userdatadir;
            
            /// game to launch
            string game;
    };
}

#endif // ADONTHELL_APP_H
