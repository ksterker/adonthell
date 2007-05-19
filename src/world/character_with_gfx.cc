/*
 $Id: character_with_gfx.cc,v 1.1 2007/05/19 07:42:08 ksterker Exp $
 
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
 * @file   world/character_with_gfx.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the character_with_gfx class.
 * 
 * 
 */

#include "world/character_with_gfx.h"

using world::character;
using world::character_with_gfx;
using world::placeable;
using world::placeable_gfx;

character_with_gfx::character_with_gfx (area & mymap) : character (mymap),
                                                           placeable_gfx((placeable &)*this),
                                                           shadow (40, 15)
{
//     placeable_area * moa;

//     moa = add_state ("n_stand");
//     moa->set_area_size (1, 2);
//     moa->base.set_position (0, 1);
//     moa->base.set_offset (40, 40);

//     moa = add_state ("s_stand");
//     moa->set_area_size (1, 2);
//     moa->base.set_position (0, 1);
//     moa->base.set_offset (40, 40);

//     moa = add_state ("e_stand");
//     moa->set_area_size (1, 2);
//     moa->base.set_position (0, 1);
//     moa->base.set_offset (40, 40);

//     moa = add_state ("w_stand");
//     moa->set_area_size (1, 2);
//     moa->base.set_position (0, 1);
//     moa->base.set_offset (40, 40);

//     moa = add_state ("n_walk");
//     moa->set_area_size (1, 2);
//     moa->base.set_position (0, 1);
//     moa->base.set_offset (40, 40);

//     moa = add_state ("s_walk");
//     moa->set_area_size (1, 3);
//     moa->base.set_position (0, 2);
//     moa->base.set_offset (40, 40);

//     moa = add_state ("e_walk");
//     moa->set_area_size (1, 2);
//     moa->base.set_position (0, 1);
//     moa->base.set_offset (40, 40);

//     moa = add_state ("w_walk");
//     moa->set_area_size (1, 2);
//     moa->base.set_position (0, 1);
//     moa->base.set_offset (40, 40);

//     moa = add_state ("n_run");
//     moa->set_area_size (1, 2);
//     moa->base.set_position (0, 1);
//     moa->base.set_offset (40, 40);

//     moa = add_state ("s_run");
//     moa->set_area_size (1, 2);
//     moa->base.set_position (0, 1);
//     moa->base.set_offset (40, 40);

//     moa = add_state ("e_run");
//     moa->set_area_size (1, 2);
//     moa->base.set_position (0, 1);
//     moa->base.set_offset (40, 40);

//     moa = add_state ("w_run");
//     moa->set_area_size (1, 2);
//     moa->base.set_position (0, 1);
//     moa->base.set_offset (40, 40);

//     moa = add_state("say_hi");
//     moa->set_area_size (1, 2);
//     moa->base.set_position (0, 1);
//     moa->base.set_offset (40, 40);

    
//     placeable_area_gfx * moag;
//     animation * an;
//     image * im;
//     animationframe af;

//     moag = add_gfx ("n_stand");
//     moag->get_animation()->load("adontest/standnorth2.anim");
    
//     moag = add_gfx ("s_stand");
//     an = moag->get_animation();
//     im = new image();
//     im->load_pnm("adontest/anim/south_base.pnm");
//     an->insert_image(im, 0);
//     af.set_mask(true);
//     an->insert_frame(af, 0);

//     moag = add_gfx ("e_stand");
//     moag->get_animation()->load("adontest/standeast2.anim");

//     moag = add_gfx ("w_stand");
//     moag->get_animation()->load("adontest/standwest2.anim");

//     moag = add_gfx ("n_walk");
//     moag->get_animation()->load("adontest/walknorth2.anim");

//     moag = add_gfx ("s_walk");
//     an = moag->get_animation();
//     im = new image();
//     im->load_pnm("adontest/anim/south_mov1.pnm");
//     an->insert_image(im, 0);
//     im = new image();
//     im->load_pnm("adontest/anim/south_mov2.pnm");
//     an->insert_image(im, 1);
//     im = new image();
//     im->load_pnm("adontest/anim/south_mov3.pnm");
//     an->insert_image(im, 2);
//     im = new image();
//     im->load_pnm("adontest/anim/south_mov4.pnm");
//     an->insert_image(im, 3);
//     im = new image();
//     im->load_pnm("adontest/anim/south_mov5.pnm");
//     an->insert_image(im, 4);
//     im = new image();
//     im->load_pnm("adontest/anim/south_mov6.pnm");
//     an->insert_image(im, 5);
//     af.set_mask(true);
//     af.set_offset(0,39);
//     af.set_image_nbr(0);
//     af.set_delay(9);
//     af.set_nextframe(1);
//     an->insert_frame(af, 0);
//     af.set_image_nbr(1);
//     af.set_delay(9);
//     af.set_nextframe(2);
//     an->insert_frame(af, 1);
//     af.set_image_nbr(2);
//     af.set_delay(9);
//     af.set_nextframe(3);
//     an->insert_frame(af, 2);
//     af.set_image_nbr(3);
//     af.set_delay(9);
//     af.set_nextframe(4);
//     an->insert_frame(af, 3);
//     af.set_image_nbr(4);
//     af.set_delay(9);
//     af.set_nextframe(5);
//     an->insert_frame(af, 4);
//     af.set_image_nbr(5);
//     af.set_delay(9);
//     af.set_nextframe(0);
//     an->insert_frame(af, 5);

//     moag = add_gfx ("e_walk");
//     moag->get_animation()->load("adontest/walkeast2.anim");

//     moag = add_gfx ("w_walk");
//     moag->get_animation()->load("adontest/walkwest2.anim");

//     moag = add_gfx ("n_run");
//     moag->get_animation()->load("adontest/runnorth2.anim");

//     moag = add_gfx ("s_run");
//     moag->get_animation()->load("adontest/runsouth2.anim");

//     moag = add_gfx ("e_run");
//     moag->get_animation()->load("adontest/runeast2.anim");

//     moag = add_gfx ("w_run");
//     moag->get_animation()->load("adontest/runwest2.anim");
    
//     set_state("s_stand");
//     save("adontest/nakedguy.mdl");
}

void character_with_gfx::put(base::ogzstream & file) const
{
    character::put(file);
    placeable_model_gfx::put(file);
}

void character_with_gfx::get(base::igzstream & file)
{
    character::get(file);
    placeable_model_gfx::get(file);
    set_gfx(current_state_name());
}

s_int8 character_with_gfx::save(const std::string fname) const
{
    base::ogzstream file (fname);
    s_int8 ret = 0; 
    
    if (!file.is_open ())
        return 1;
    put (file);
    file.close (); 
    Filename = fname;
    return ret;
}

s_int8 character_with_gfx::load(const std::string fname)
{
    base::igzstream file (fname);
    s_int8 ret = 0; 
    
    if (!file.is_open ())
        return 1;
    get (file);
    file.close (); 
    Filename = fname;
    return ret;
}

void character_with_gfx::draw (s_int16 x, s_int16 y, const gfx::drawing_area * da_opt,
                               gfx::surface * target)
{
    draw_shadow(x, y, da_opt, target);
    y -= z();
    placeable_model_gfx::draw(x, y, da_opt, target);
}

void character_with_gfx::draw_shadow (s_int16 x, s_int16 y, const gfx::drawing_area * da_opt,
                                      gfx::surface * target)
{
    shadow.draw(x, y + 25 - zground, da_opt, target);
}
