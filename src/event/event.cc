/*
   $Id: event.cc,v 1.4 2004/05/13 06:44:00 ksterker Exp $

   Copyright (C) 2000/2001/2002/2003 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file	event/event.cc
 * @author 	Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  	Defines the base event class.
 */

#include "event/event.h"

using event::event;

// constructor
event::event ()
{
    // repeat forever
    Repeat = -1;
}

// save the state of the script associated with the event
void event::put_state (base::flat & out) const
{
    out.put_uint8 ("etp", Type);
    out.put_sint32 ("erp", Repeat);
}

// load the state of the script associated with the event 
bool event::get_state (base::flat & in) 
{
    // Note that 'Type' is already read by listener::get_state 
    // to determine what event subclass to instanciate
    Repeat = in.get_sint32 ("erp");
    
    return in.success ();
}
