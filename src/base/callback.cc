/*
   $Id: callback.cc,v 1.2 2003/07/24 12:57:58 gnurou Exp $

   Copyright (C) 2003   Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
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
