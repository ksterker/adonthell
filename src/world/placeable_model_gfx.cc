/*
 $Id: placeable_model_gfx.cc,v 1.12 2008/01/13 18:36:02 ksterker Exp $
 
 Copyright (C) 2002 Alexandre Courbot <alexandrecourbot@linuxgames.com>
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
 * @file   world/placeable_model_gfx.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the placeable_model_gfx class.
 * 
 * 
 */

#include "gfx/image.h"
#include "world/coordinates.h"
#include "world/placeable_model_gfx.h"

using world::placeable_model_gfx;

// ctor
placeable_model_gfx::placeable_model_gfx (placeable_model & target) : Target (target)
{
}

// dtor
placeable_model_gfx::~placeable_model_gfx()
{
}

// set sprite
bool placeable_model_gfx::set_sprite(const std::string & file)
{
    Sprite.load_animation (file);	
    return true;
}

// select animation from sprite
bool placeable_model_gfx::set_gfx (const std::string & name) 
{
    if (Sprite.change_animation (name))
    {
 	    Sprite.play ();
    	return true;
    }
   
   	fprintf (stderr, "*** placeable_model_gfx::set_gfx: state '%s' not contained in\n" \
             "    sprite '%s' of\n" \
             "    object '%s'\n", 
	   		name.c_str(), Sprite.filename().c_str(), Target.filename().c_str());
	 return false;
}

// placeable_area_gfx * placeable_model_gfx::get_gfx (const std::string & name)
// {
//    std::map <const std::string, placeable_area_gfx *>::iterator Gfx;
//    Gfx = Gfxs.find (name); 
//    
//    if (Gfx != Gfxs.end())
//        return Gfx->second;
//    else return NULL;
// }

// placeable_area_gfx * placeable_model_gfx::add_gfx(const std::string & name)
// {
//     placeable_area_gfx * newmpa = new placeable_area_gfx();
//    
//     return (Gfxs.insert(std::pair<const std::string, placeable_area_gfx *>
//                        (name, newmpa))).first->second;
// }

// bool placeable_model_gfx::del_gfx(const std::string & name)
// {
//     std::map <const std::string, placeable_area_gfx *>::iterator it = Gfxs.find(name);
//     if (it == Gfxs.end()) return false;
//     delete it->second;
//     return true;
// }

// update state of object
bool placeable_model_gfx::update() 
{
    if (Target.StateChanged)
    {
        Target.StateChanged = false;
        return set_gfx (Target.current_shape_name ()); 
    }
    
    return true;
}

// render sprite
void placeable_model_gfx::draw (s_int16 x, s_int16 y, const gfx::drawing_area * da_opt,
                                gfx::surface * target) const
{
    const placeable_shape * shape = Target.current_shape();
    
    Sprite.draw (
        x, 
        y - shape->height(), 
        da_opt, target);
}

// draw collision area
void placeable_model_gfx::draw_walkable(s_int16 x, s_int16 y, 
                                        const gfx::drawing_area * da_opt,
                                        gfx::surface * target) const
{
    /* fixme
	// for now, collision area is square, will change later
    gfx::image im (SQUARE_SIZE, SQUARE_SIZE);
    im.fillrect (0, 0, im.length(), im.height(), 0xFF, 0, 0);
    im.set_alpha (128);
    
    // get collision information for object's current state
    placeable_shape * st = Target.current_shape();
    
    for (int j = 0; j < st->area_height(); j++)
    {
        for (int i = 0; i < st->area_length(); i++)
        {
            if (!st->get(i, j).is_walkable())
            {
       	    	im.draw(
       	    		x - (st->base.x() - i) * SQUARE_SIZE - st->base.ox(), 
                    y - (st->base.y() - j) * SQUARE_SIZE - st->base.oy(), 
                    da_opt, target);
            }
        }
    }
    */
}

// draw bounding box around object
void placeable_model_gfx::draw_bounding_box (s_int16 x, s_int16 y, 
                                      const gfx::drawing_area * da_opt,
                                      gfx::surface * target) const
{
    placeable_shape *shape = Target.current_shape();
    cube3 bbox (shape->length(), shape->width(), shape->height());
    bbox.draw (x + shape->x(), y + shape->y(), da_opt, target);
}

// save to stream
bool placeable_model_gfx::put_state (base::flat & file) const
{
    file.put_string ("sprite", Sprite.filename());
    return true;
}

// load from stream
bool placeable_model_gfx::get_state (base::flat & file)
{
    std::string name = file.get_string ("sprite");
    if (!file.success ()) return false;
    
    if (name.length() > 0)
    {
    	return Sprite.load_animation (name);
	}
	
	return true;
}

// get current graphics
// gfx::surface * placeable_model_gfx::current_gfx()
// {
//    if (Current_gfx != Gfxs.end ())
//        return Current_gfx->second;
//    else return NULL; 
// }
