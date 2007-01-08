/*
   $Id: main.cc,v 1.3 2007/01/08 07:51:23 ksterker Exp $

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
 * @file   main/main.cc 
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Main function for programs using the Adonthell framework.
 * 
 * 
 */

#include "adonthell.h"

// class AdonthellApp;
// extern "C" AdonthellApp theApp;

// initialize the engine and call the user's main method
int main (int argc, char *argv[]) 
{
	int retval = 1;
	
	// does a main class exist?
	if (adonthell::app::theApp != NULL)
	{
		// pointer to our main class
    	adonthell::app *application = adonthell::app::theApp;
    
	    // parse command line arguments for framework related arguments
    	application->parse_args (argc, argv);
    
	    // initialize and start the application
    	// NOTE: some backends, like SDL/OSX, never return from that call
    	retval = application->init ();
    
    	// do cleanup, if required
    	application->cleanup ();
    
    	// thanks for using Adonthell ...
    	return retval;
	}
	else
	{
		fprintf (stderr, "*** main: no application class found! Exitting ...");
	}
	
	return retval;
}
