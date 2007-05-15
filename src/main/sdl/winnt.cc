/*
   $Id: winnt.cc,v 1.1 2007/05/15 02:41:23 ksterker Exp $

   Copyright (C) 2007 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file   main/sdl/winnt.cc 
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  SDL initialization for Windows NT target.
 * 
 * 
 */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "SDL.h"
#include "SDL_main.h"

#include "main/adonthell.h"

#ifdef USE_LIBTOOL
/* exported names for libltdl */
#define main_init sdl_LTX_main_init
#endif

extern "C" {

/* SDL_Quit() shouldn't be used with atexit() directly because
   calling conventions may differ... */
static void cleanup(void)
{
	SDL_Quit();
}

/* Init */
int main_init (const adonthell::app *theApp)
{
	/* Load SDL dynamic link library */
	if ( SDL_Init(SDL_INIT_NOPARACHUTE) < 0 ) {
		return 0;
	}

	atexit(cleanup);

	/* Sam:
	   We still need to pass in the application handle so that
	   DirectInput will initialize properly when SDL_RegisterApp()
	   is called later in the video initialization.
	 */
	SDL_SetModuleHandle(GetModuleHandle(NULL));
	
    // no need to initialize anything, so just start the application ...
    int retval = ((adonthell::app *) theApp)->main ();
    
    // Exit cleanly, calling atexit() functions
    exit (retval);
    
    return 0;
}

}
