/*
 $Id: area.h,v 1.4 2007/10/22 06:05:08 ksterker Exp $
 
 Copyright (C) 2002 Alexandre Courbot <alexandrecourbot@linuxgames.com>
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
 * @file   world/area.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Declares the area class.
 * 
 * 
 */


#ifndef WORLD_AREA_H
#define WORLD_AREA_H

#include "world/character_with_gfx.h"
#include "world/object_with_gfx.h"
#include "world/square.h"

namespace world
{
    /**
     * The plane of existance. A map consists of a grid of squares that
     * contain the actual scenery elements, characters and items.
     */
    class area
    {
    private:

#ifndef SWIG

        template<class T, class T_gfx>
        class thing_manager
        {
        protected:
            std::vector<T *> objects;
            
            bool handle_gfx;
            
        public:
            thing_manager()
            {
                handle_gfx = true;
            }
            
            ~thing_manager()
            {
                clear();
            }
            
            void clear()
            {
                // FIXME: doesn't a virtual destructor take care of deleting the proper object already???
                if(handle_gfx)
                    for (typename std::vector<T *>::iterator i = objects.begin();
                         i != objects.end(); i++)
                        // FIXME: check whether it works or not.
                        delete ((T_gfx*)*i);
                else
                    for (typename std::vector<T *>::iterator i = objects.begin();
                         i != objects.end(); i++)
                        delete *i;
                
                objects.clear();
            }
            
            bool update()
            {
                if (handle_gfx) return update_gfx();
                else return update_nogfx();
            }
            
            bool update_nogfx()
            {
                for (typename std::vector<T *>::iterator i = objects.begin();
                     i != objects.end(); i++)
                    (*i)->update();
                
                return true;
            }
            
            bool update_gfx()
            {
                for (typename std::vector<T *>::iterator i = objects.begin();
                     i != objects.end(); i++)
                {
                    (*i)->update();
                    ((T_gfx *)(*i))->placeable_gfx::update();
                }
                return true;
            }
            
            T * add(area & lm)
            {
                T * obj;
                if (handle_gfx) obj = new T_gfx(lm);
                else obj = new T(lm);
                
                objects.push_back(obj);
                
                return (obj);
            }
            
            
            T * operator [] (const int & index)
            {
                return objects[index];
            }
        };
        
        /// Pointers to objects that are placed on this map.
        thing_manager<object, object_with_gfx> objects;
        
        /// Pointers to the characters that are on this map.
        thing_manager<character, character_with_gfx> characters;

        /// The individual map squares
        std::vector <std::vector <square> > Grid;
        
        bool put (placeable * obj, coordinates & pos); 
        bool put (moving * obj); 
        bool remove (placeable * obj, coordinates & pos); 
        bool remove (moving * obj); 

#endif // SWIG
        
    public:
        /**
         * Create an empty map. Call resize to set its initial size
         * or load its state from disk.
         */
        area () { }
        
        /**
         * Delete the map and everything on it.
         */
        ~area ();
        
        /**
         * @name The Grid
         */
        //@{
        /**
         * Return size of the map grid along the X axis.
         * @return extension of the map in X direction.
         */
        u_int16 length () const
        {
            return Grid.size ();
        }
        
        /**
         * Return size of the map grid along the Y axis.
         * @return extension of the map in Y direction.
         */
        u_int16 height () const
        {
            if (Grid.size ()) return Grid[0].size ();
            else return 0; 
        }
        
        /**
         * Change the size of the map grid. It's probably a good
         * idea to do that before placing anything onto the map,
         * although growing the map should have no ill effect.
         * @param nx new number of cells in x direction.
         * @param ny new number of cells in y direction.
         */
        void resize (const u_int16 & nx, const u_int16 & ny);

        /**
         * Get the map square at the given location.
         * @param x x location on the map grid
         * @param y y location on the map grid
         * @return square at given position on the grid.
         */
        square * get (const u_int16 & x, const u_int16 & y);         
        //@}
        
        /**
         * @name Scenery and Characters
         */
        //@{
        /**
         * Remove all objects and characters from the map.
         */
        void clear();
        
        /**
         * Update state of all characters and objects on the map.
         * @todo decide if we need that, or if everything can be
         *     updated based on events.
         */
        void update();
        
        /** 
         * Adds an object to this map. Creates a new, empty object and returns
         * it, so that it can be initialized by the caller of this method.
         * The map will take ownership of the object.
         *
         * @todo this function should load objects from files, being given
         * a object file name. That way, there won't be any ownership problem
         * anymore.
         *
         * @return new object in case of success, \e NULL otherwise.
         */
        object * add_object();
        
        /** 
         * Adds a new character to the map. Creates a new, empty character and 
         * returns it, so that it can be initialized by the caller of this method.
         * The map will take ownership of the character.
         * 
         * @todo this function should load objects from files, being given
         * a character file name. That way, there won't be any ownership problem
         * anymore.
         * 
         * @return pointer to the character instance in case of success, 
         *      \e NULL otherwise.
         */
        character * add_character();

        /**
         * Place object at a given index at the given location.
         * @param index index of object int the list of objects.
         * @param pos position to place object at.
         */
        bool put_object (const u_int32 & index, coordinates & pos);
        //@}
        
        /**
         * For debugging. Print how many objects are located on each square of 
         * the map.
         */
        void output_occupation();
        
        /// This class is allowed to change and move objects on the map.
        friend class moving;
    }; 
}

#endif // WORLD_AREA_H
