/*
   $Id: adonthell.h,v 1.1 2003/11/22 09:37:39 ksterker Exp $

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
 * @file   main/adonthell.h 
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  The main application class for programs using the Adonthell framework.
 * 
 * 
 */

#ifndef ADONTHELL_APP
#define ADONTHELL_APP

#include <string>
#include "ltdl.h"

using std::string;

namespace adonthell {
    /**
     *
     */
    class app {
        public:
            /**
             *
             */
            virtual int main () const = 0;
        
            /**
             *
             */
            bool init ();
            
            /**
             *
             */
            void parse_args (int & argc, char *argv[]);
            
            /**
             *
             */
            void cleanup ();

        protected:
            /**
             *
             */
            int argc;
            
            /**
             *
             */
            char **argv;

        private:
            /**
             * The handler of our library file.
             */
            static lt_dlhandle dlhandle;

            /**
             *
             */
            static bool (*init_p)(app* application);
            
            /**
             * the backend we're using
             */
            static string backend;
    };
}

#endif // ADONTHELL_APP
