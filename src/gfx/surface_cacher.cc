/*
 $Id: surface_cacher.cc,v 1.2 2009/04/25 13:17:50 ksterker Exp $
 
 Copyright (C) 2008 Rian Shelley <rians@cc.usu.edu>
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
 * @file   gfx/surface_cacher.cc
 * @author Rian Shelley <rians@cc.usu.edu>
 *
 * @brief  Declares the surface cache implementation.
 *
 *
 */

#include <cstdio>
#include <sstream>
#include "gfx.h"
#include "surface_cacher.h"

namespace gfx 
{
    // ctor
	surface_ref::~surface_ref()
	{
		surfaces->free_surface(s);
	}
	
    // dtor
	surface_cacher::~surface_cacher()
	{
	    purge();
	}
    
	// return surface reference for a dynamic image
	const surface_ref* surface_cacher::get_surface_mem (const string & name)
	{
        map<string, surface_ref>::iterator idx = Cache.find(name);
        if (idx != Cache.end())
        {
            //We already found it in the cache. Increment the reference
            idx->second.newref();
            return new surface_ref(&(idx->second));
        }

        // that surface does not exist in the cache
        return NULL;
	}

    // add existing surface to the cache
	const surface_ref* surface_cacher::add_surface_mem (const string & name, gfx::surface *surf)
    {
        surface_ref ret = surface_ref(surf);
        ret.newref();
        Cache[name] = ret;
        SurfToString[surf] = name;
        ret.newref();

        // remove any extra surfaces we have
        conditional_purge();
        return new surface_ref(&ret);
    }

    // return surface reference for a given file
	const surface_ref* surface_cacher::get_surface(const string & file, bool set_mask, bool invert_x, bool invert_y, blend_mode alpha)
	{
		// create a unique name based on all parameters
        std::stringstream cache_name (std::ios::out);
        cache_name << file << "_" << set_mask << "_" << invert_x << "_" << invert_y << "_" << alpha;

		map<string, surface_ref>::iterator idx = Cache.find(cache_name.str());
		if (idx != Cache.end())
		{
			//We already found it in the cache. Increment the reference
			idx->second.newref();
			return new surface_ref(&(idx->second));
		}

		//Cache miss, try to load the file
		surface *cur = create_surface();
		cur->load_png(file);
		cur->set_mask(set_mask);
		cur->mirror(invert_x, invert_y);
        
        // either use alpha setting of image, or set to user specified value
        if (alpha != AUTOMATIC)
        {
            cur->set_alpha(255, alpha == BLEND);
        }
		MemUsed += cur->size();
        
		//Add it to the cache
		surface_ref ret = surface_ref(cur);
		ret.newref();
		Cache[cache_name.str()] = ret;
		SurfToString[cur] = cache_name.str();
		ret.newref();
		
        //Remove any extra surfaces we have
		conditional_purge();
		return new surface_ref(&ret);
	}

    // return pointer to surface
    const surface* surface_cacher::get_surface_only(const string& file, bool set_mask, bool invert_x, bool invert_y, blend_mode alpha)
    {
        const surface_ref * sref = get_surface(file, set_mask, invert_x, invert_y, alpha);
        const surface* ret = sref->s;
        Cache[SurfToString[ret]].newref();
        delete sref;
        return ret;
    }

    // release reference of named surface
	void surface_cacher::free_by_name(const string & s)
	{
		map<string, surface_ref>::iterator idx = Cache.find(s);

		if (idx != Cache.end())
		{
			idx->second.delref();
	        conditional_purge();
		}
	}
    
    // release reference
	void surface_cacher::free_surface(const string & file, bool set_mask, bool invert_x, bool invert_y, blend_mode alpha)
	{
        std::stringstream cache_name (std::ios::out);
        cache_name << file << "_" << set_mask << "_" << invert_x << "_" << invert_y << "_" << alpha;
		free_by_name(cache_name.str());
	}
    
    // release reference
	void surface_cacher::free_surface(const surface* surf)
	{
        if (surf != NULL)
        {
            free_by_name(SurfToString[surf]);
        }
	}
    
    // get refcount for given surface
	unsigned int surface_cacher::count_surface(const string & file, bool set_mask, bool invert_x, bool invert_y, blend_mode alpha)
	{
        std::stringstream cache_name (std::ios::out);
        cache_name << file << "_" << set_mask << "_" << invert_x << "_" << invert_y << "_" << alpha;
		map<string, surface_ref>::iterator idx = Cache.find(cache_name.str());
		if (idx != Cache.end())
        {
            return idx->second.RefCount;
        }
        return 0;
	}
    
    // remove all items no longer referenced
	void surface_cacher::purge()
	{
		unsigned int tmp = MemMax;
		set_max_mem(0);
		set_max_mem(tmp);
	}
    
    // remove items as long as cache size is over limit 
	void surface_cacher::conditional_purge()
	{
		if (MemUsed >= MemMax) 
		{
			map<string, surface_ref>::iterator idx = Cache.begin();
			while (idx!=Cache.end() && MemUsed >= MemMax)
			{
				if (idx->second.RefCount == 0)
				{
					if (idx->second.s)
					{
						SurfToString.erase(idx->second.s);
						MemUsed -= idx->second.s->size();
						delete idx->second.s;
					}
					map<string, surface_ref>::iterator deleteme = idx;
					idx++;
					Cache.erase(deleteme);
				}
				else
                {
					idx++;
                }
			}
		}
	}
}
