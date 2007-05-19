/*
 $Id: placeable_model_gfx.cc,v 1.1 2007/05/19 07:42:10 ksterker Exp $
 
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
#include "placeable_model_gfx.h"

using world::placeable_area_gfx;
using world::placeable_model_gfx;

placeable_model_gfx::placeable_model_gfx (placeable_model & target) : Target(target)
{
    Current_gfx = Gfxs.begin (); 
}

placeable_model_gfx::~placeable_model_gfx()
{
    for (Current_gfx = Gfxs.begin(); Current_gfx != Gfxs.end(); Current_gfx++)
        delete Current_gfx->second;
}

void placeable_model_gfx::set_gfx (const std::string & name) 
{
    if (Current_gfx != Gfxs.end() && Current_gfx->first == name)
        return;
    
    if (Current_gfx != Gfxs.end())
    {
        Current_gfx->second->stop();
        Current_gfx->second->rewind();
    }
    
    std::map <const std::string, placeable_area_gfx *>::iterator Previous_gfx;
    Previous_gfx = Current_gfx;
    Current_gfx = Gfxs.find (name); 
    
    if (Current_gfx != Gfxs.end())
        Current_gfx->second->play();
    else Current_gfx = Previous_gfx;
}

placeable_area_gfx * placeable_model_gfx::get_gfx(const std::string & name)
{
    std::map <const std::string, placeable_area_gfx *>::iterator Gfx;
    Gfx = Gfxs.find (name); 
    
    if (Gfx != Gfxs.end())
        return Gfx->second;
    else return NULL;
}

placeable_area_gfx * placeable_model_gfx::add_gfx(const std::string & name)
{
    placeable_area_gfx * newmpa = new placeable_area_gfx();
    
    return (Gfxs.insert(std::pair<const std::string, placeable_area_gfx *>
                        (name, newmpa))).first->second;
}

bool placeable_model_gfx::del_gfx(const std::string & name)
{
    std::map <const std::string, placeable_area_gfx *>::iterator it = Gfxs.find(name);
    if (it == Gfxs.end()) return false;
    delete it->second;
    return true;
}

bool placeable_model_gfx::update() 
{
    if (Target.State_changed)
    {
        Target.State_changed = false;
        set_gfx (Target.current_state_name()); 
    }
    
    if (Current_gfx != Gfxs.end ())
        Current_gfx->second->update ();
    
    return true;
}

void placeable_model_gfx::draw (s_int16 x, s_int16 y, const gfx::drawing_area * da_opt,
                                gfx::surface * target) const
{
    if (Current_gfx != Gfxs.end ())
    {
        if (Target.Current_state != Target.States.end())
        {
            const placeable_area & t = Target.Current_state->second;
            Current_gfx->second->draw (x - (t.base.x()) * 
                                       square_size - t.base.ox(), 
                                       y - (t.base.y()) * 
                                       square_size - t.base.oy(), 
                                       da_opt, target); 
        }
        else
            Current_gfx->second->draw (x, y, da_opt, target); 
    }
}

void placeable_model_gfx::draw_walkable(s_int16 x, s_int16 y, 
                                        const gfx::drawing_area * da_opt,
                                        gfx::surface * target) const
{
    gfx::image im(square_size, square_size);
    im.fillrect(0, 0, im.length(), im.height(), 0xFF, 0, 0);
    im.set_alpha(128);
    
    placeable_area * st = Target.current_state();
    

    for (int j = 0; j < st->area_height(); j++)
        for (int i = 0; i < st->area_length(); i++)
        {
            if (!st->get(i, j).is_walkable())
            {
            im.draw(x - (st->base.x() - i) * 
                    square_size - st->base.ox(), 
                    y - (st->base.y() - j) * 
                    square_size - st->base.oy(), 
                    da_opt, target); 
            }
        }
}

void placeable_model_gfx::draw_border(s_int16 x, s_int16 y, 
                                      const gfx::drawing_area * da_opt,
                                      gfx::surface * target) const
{
    placeable_area * st = Target.current_state();

    x -= st->base.x() * square_size + st->base.ox();
    y -= st->base.y() * square_size + st->base.oy();

    gfx::surface *display = gfx::screen::get_surface();
    display->fillrect(x, y, st->area_length() * square_size, 1, 0xFFFFFF);
    display->fillrect(x, y, 1, st->area_height() * square_size, 0xFFFFFF);
    display->fillrect(x + st->area_length() * square_size - 1, y, 1, st->area_height() * square_size, 0xFFFFFF);
    display->fillrect(x, y + st->area_height() * square_size - 1, st->area_length() * square_size, 1, 0xFFFFFF);
}

void placeable_model_gfx::put(base::ogzstream & file) const
{
    u_int32 s = Gfxs.size();
    s >> file;
    
    for (std::map <const std::string, placeable_area_gfx *>::iterator i = Gfxs.begin();
         i != Gfxs.end(); i++)
    {
        i->first >> file;
        i->second->put(file);
    }   
}

void placeable_model_gfx::get(base::igzstream & file)
{
    u_int32 size;
    
    size << file;
    
    for (u_int32 i = 0; i < size; i++)
    {
        std::string s;
        s << file;
        
        placeable_area_gfx * mpa = add_gfx(s);
        mpa->get(file);
        
    }
}

placeable_area_gfx * placeable_model_gfx::current_gfx()
{
    if (Current_gfx != Gfxs.end ())
        return Current_gfx->second;
    else return NULL; 
}
