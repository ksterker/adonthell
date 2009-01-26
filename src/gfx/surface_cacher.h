/*
 $Id: surface_cacher.h,v 1.3 2009/01/26 21:09:14 ksterker Exp $
 
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
 * @file   gfx/surface_cacher.h
 * @author Rian Shelley <rians@cc.usu.edu>
 *
 * @brief  Declares the surface cache interface.
 *
 *
 */

#ifndef SURFACECACHER_INCLUDED
#define SURFACECACHER_INCLUDED

#include "base/types.h"
#include <ctime>
#include <string>
#include <map>

#define DEFAULT_CACHE_SIZE 10000000

namespace gfx
{
	class surface;

	class surface_ref 
	{
	public:
		const surface* s;
		surface_ref(surface* surf=NULL):s(surf),RefCount(1),TimeStamp(0) {}
		surface_ref(surface_ref* olds ):s(olds->s) {}
		~surface_ref();
        
    private:
        friend class surface_cacher;
		u_int32 RefCount;
		u_int32 TimeStamp;
		void newref() {TimeStamp=time(0); RefCount++;}
		void delref() {if (RefCount) RefCount--;}
	};

	/**
	 * Allows us to keep track of cached image instances so that they can be deleted if necessary.
     * The cache has a maximum size that it will fill up with images. Once that limit is exceeded,
     * it will try to discard unreferenced images until memory usage drops below the limit again.
     *
     * It may stay permanently over the limit if too many images are referenced. For best performance,
     * the size should allow to keep at least some images in cache, even if they are not referred to
     * right now. Default cache size is 10,000,000 bytes.
	 */
	class surface_cacher
	{
	public:
        /**
         * Create surface cache with a maximum cache size.
         * @param max maximum cache size.
         */
		surface_cacher (const u_int32 & max = DEFAULT_CACHE_SIZE) : MemUsed(0), MemMax(max) { }
		
        /**
         * Delete surface cache and its contents.
         */
        ~surface_cacher();
		
        /**
         * @name Surface related methods.
         */
        //@{
		/**
		 * Finds a surface object and increments the reference count, or loads and caches it if it can't. 
		 * This method will free memory if it can to stay under the set memory limit
		 * Delete the returned pointer to decrement the reference count, or 
		 * call free_surface with the surface in the pointer. 
		 *
		 * @param file which file to return a cached version of
         * @param set_mask whether to enable image masking
         * @param invert_x whether to mirror image on vertical axis
		 *
		 * @return a pointer to a drawable object
		 */
		const surface_ref* get_surface(const std::string & file, bool set_mask=true, bool invert_x = false);
		
		/**
         * Finds a surface object and increments the reference count, or loads and caches it if it can't. 
		 * This method will free memory if it can to stay under the set memory limit
		 * Call free_surface with the surface pointer to decrease the reference count. 
		 *
		 * @param file which file to return a cached version of
         * @param set_mask whether to enable image masking
         * @param invert_x whether to mirror image on vertical axis
		 *
		 * @return a pointer to a drawable object
		 */
        const surface* get_surface_only(const std::string & file, bool set_mask=true, bool invert_x = false);
		
        /**
		 * Decrements the reference count for the the surfaces. Does not deallocate surfaces
		 *
		 * @param file which file to find and decrement its reference
         * @param set_mask whether to enable image masking
         * @param invert_x whether to mirror image on vertical axis
		 */
		void free_surface(const std::string & file, bool set_mask=true, bool invert_x = false);
		
        /**
		 * Decrements the reference count for the the surfaces. Does not deallocate surfaces
		 *
		 * @param surf a pointer to the surface to free
		 */
		void free_surface(const surface* surf);
		
        /**
         * Count references for given surface.
         *
         * @param file which file to find and decrement its reference
         * @param set_mask whether to enable image masking
         * @param invert_x whether to mirror image on vertical axis
		 *
         * @return the number of references there are to a surface
		 */
		u_int32 count_surface(const std::string & file, bool set_mask=true, bool invert_x = false);
		//@}
        
        /**
         * Cache cleanup methods.
         */
        //@{
        /**
		 * deletes all surfaces with zero references 
		 */
		void purge();
		
        /**
		 * deletes surfaces with zero references until we are under our memory limit
		 */
		void conditional_purge();
        //@}
        
        /**
         * @name Cache size methods.
         */
        //@{
		/**
         * Get current memory utilization.
		 * @return how much memory the cacher is using
		 */
		u_int32 used_mem() const 
        {
            return MemUsed;
        }
        
        /**
         * Get amount allowed for the cacher to use.
		 * @return the maximum amount of memory the cacher should try to use
		 */
		u_int32 max_mem() const 
        {
            return MemMax;
        }
        
		/**
		 * Allows you to set the new maximum memory. 
         * Will delete zero-referenced surfaces to fit the new limit.
		 *
		 * @param mm the new maximum memory. 
		 */
		void set_max_mem (u_int32 mm) 
        { 
            MemMax = mm; 
            conditional_purge();
        }	
		//@}
        
    private:
        /// list of surfaces by name
        std::map<std::string, surface_ref> Cache;
        /// mapping of surface to name
		std::map<const surface*, std::string> SurfToString;
        /// memory used by cache
		u_int32 MemUsed;
        /// memory allowed to use
		u_int32 MemMax;
        
        /**
         * Decrease reference of named surface.
         * @param surfname name of surface to decref.
         */
		void free_by_name(const std::string& surfname);
	};


	/**
	 * A singleton surface_cacher
	 */
#ifndef SWIG
	extern surface_cacher* surfaces;
#endif
}

#endif

