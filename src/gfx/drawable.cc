// $Id: drawable.cc,v 1.2 2003/07/24 12:57:58 gnurou Exp $
/*
   Copyright (C) 1999/2000/2001/2002   Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/**
 * @file   gfx/drawable.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief Defines the drawable class. 
 */


#include "drawable.h"

// Public methods.

namespace gfx
{
    
    drawable::drawable () 
        : length_(0), height_(0)
    {
    }
    
    drawable::~drawable ()
    { 
    }
    
    bool drawable::update ()
    {
        return true; 
    }
    
}
