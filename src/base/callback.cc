/*
   $Id: callback.cc,v 1.3 2003/11/22 09:35:21 ksterker Exp $

   Copyright (C) 2003   Alexandre Courbot <alexandrecourbot@linuxgames.com>
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
 * @file   base/callback.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the callback interface.
 * 
 * 
 */

#include "callback.h"

namespace base
{
    functor_base::functor_base()
        : func(0), callee(0)
    {
    }
    
    functor_base::functor_base(void* c, PFunc f, const void* mf, size_t sz)
        : func(0), callee(c)
    {
        if(callee)
        {
            memcpy(memFunc, mf, sz);
            if(sz < MEM_FUNC_SIZE)
            {
                memset(memFunc+sz, 0, MEM_FUNC_SIZE-sz);
            }
        }
        else
        {
            func = f;
        }
    }
    
    functor_0::functor_0(Thunk t, void * c,PFunc f, const void * mf,size_t sz)
        : functor_base(c,f,mf,sz), thunk(t)
    {
    }
    
    functor_0::functor_0()
    {
    }
    
    functor_0::~functor_0()
    {
    }   
}
