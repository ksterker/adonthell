/*
 $Id: moving.cc,v 1.2 2007/05/21 04:44:11 ksterker Exp $
 
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
 * @file   world/moving.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the moving class.
 * 
 * 
 */

#include "world/moving.h"
#include "world/area.h"

using world::moving;
using world::area;

moving::moving (world::area & mymap)
    : placeable (mymap), coordinates ()
{
    Vx = 0.0;
    Vy = 0.0; 
    Vz = 0.0;

    Lx = 65535;
    Ly = 65535;
    
    fox = 0.0; 
    foy = 0.0; 
    foz = 0.0;

    Has_moved = false;
    Is_falling = false;

    zground = -100000;
}

void moving::set_offset (u_int16 ox, u_int16 oy)
{
    coordinates::set_offset (ox, oy);
    fox = ox;
    foy = oy; 
}

void moving::set_velocity (float vx, float vy) 
{
    Vx = vx;
    Vy = vy;
}

void moving::set_vertical_velocity(float vz)
{
    Vz = vz;
}

void moving::set_limits (u_int16 mx, u_int16 my)
{
    Lx = mx;
    Ly = my; 
}

void moving::update_pos2()
{
    Has_moved = 0; 

    u_int16 nX = X;
    float nfox = fox;
    u_int16 nY = Y;
    float nfoy = foy;
    s_int32 prevz = z();

    // Calculating new X position
    if (vx ()) 
    {
        Has_moved = 1;
        
        nfox += vx ();
        
        while (nfox < 0) 
        {
            if (nX > 0)
            {
                nX--;
                nfox += SQUARE_SIZE;
            }
            else nfox = 0.0; 
        }

        while (nfox >= SQUARE_SIZE) 
        {
            if (nX < Lx) 
            {
                nX++; 
                nfox -= SQUARE_SIZE;
            }
            else
            {
                nX = Lx - 1;
                nfox = 39; 
            }
        }
        if (nX == Lx - 1) 
        {
            nfox = 0;
        }
    }

    // Calculating new Y position
    if (vy ()) 
    {
        Has_moved = 1; 

        nfoy += vy ();
        while (nfoy < 0) 
        {
            if (nY > 0)
            {
                nfoy += SQUARE_SIZE;
                nY--;
            }
            else nfoy = 0.0; 
        }
        
        while (nfoy >= SQUARE_SIZE) 
        {
            if (nY < Ly) 
            {
                nfoy -= SQUARE_SIZE;
                nY++;
            }
            else 
            {
                nY = Ly - 1;
                nfoy = 39; 
            }
        }
        if (nY == Ly - 1) 
        {
            nfoy = 0; 
        }
    }

    // Calculating new Z position
    if (vz())
    {
        Has_moved = 1;

        foz += vz();
        while (foz <= -1.0)
        {
            Z--;
            foz += 1.0;
        }
        while (foz >= 1.0)
        {
            Z++;
            foz -= 1.0;
        }
    }

    // Now checking walkability
    placeable_area * state = current_state();

    u_int16 nbx = 1 + (nfox != 0.0);
    u_int16 nby = 1 + (nfoy != 0.0);

    u_int16 px = nX - state->base.x();
    u_int16 py = nY - state->base.y();

    u_int16 j;
    u_int16 i;

    Is_falling = true;
    s_int32 nzground = -100000;

    for (j = 0; j < state->area_height(); px -= i, ++j, ++py)
    {
        for (i = 0; i < state->area_length(); ++i, ++px)
        {
            if (state->get(i, j).is_walkable()) continue;

            for (u_int16 l = 0; l < nby; ++l)
                for (u_int16 k = 0; k < nbx; ++k)
                {
                    square * msqr = Mymap.get(px + k, py + l);
                    
                    if (vz() > 0)
                    {
                        for (square::iterator it = msqr->begin(); it != msqr->end(); it++)
                        {
                            s_int32 objz = it->z();
                            if (objz > z() + current_state()->zsize || objz < prevz + current_state()->zsize) continue;
                            
                            if (!it->obj->current_state()->get(px + k - it->x() + it->obj->current_state()->base.x(),
                                                               py + l - it->y() + it->obj->current_state()->base.y())
                                .is_walkable()) 
                            {
                                set_altitude(objz - (current_state()->zsize + 1));
                                set_vertical_velocity(0.0);
                                break;
                            }
                        }
                    }

                    // Check whether we hit the ground or not and calculate zground.
                    for (square::iterator it = msqr->begin(); it != msqr->end(); it++)
                    {
                        s_int32 objz = it->z() + it->obj->current_state()->zsize;
                        if (objz > prevz + climb_capability()) continue;
                        
                        if (it->obj->current_state()->get(px + k - it->x() + it->obj->current_state()->base.x(),
                                                           py + l - it->y() + it->obj->current_state()->base.y())
                            .is_walkable()) continue;

                        if (objz > nzground) nzground = objz;
                        if (objz < z()) continue;
                        
                        if (vz() <= 0)
                        {
                            set_altitude(objz);
                            Is_falling = false;
                            set_vertical_velocity(0.0);
                            break;
                        }
                    }
                    
                }

            for (u_int16 l = 0; l < nby; l++)
                for (u_int16 k = 0; k < nbx; k++)
                {
                    square * msqr = Mymap.get(px + k, py + l);

                    for (square::iterator it = msqr->begin(); it != msqr->end(); ++it)
                    {
                        if (it->obj->current_state()->get(px + k - it->x() + it->obj->current_state()->base.x(),
                                                          py + l - it->y() + it->obj->current_state()->base.y())
                            .is_walkable()) continue;
                        
                        if (z() + climb_capability() < it->z() + it->obj->current_state()->zsize && 
                            z() + state->zsize > it->z())
                            goto rollback;
                    }
                }
        }
    }

    X = nX;
    fox = nfox;
    Ox = (u_int16) fox;
    Y = nY;
    foy = nfoy;
    Oy = (u_int16) foy;

 rollback:
    zground = nzground;
}

void moving::update_pos()
{
    Has_moved = 0; 

    if (vx ()) 
    {
        u_int16 nX = X;
        float nfox = fox;

        Has_moved = 1;
        
        nfox += vx ();
        
        while (nfox < 0) 
        {
            if (nX > 0)
            {
                nX--;
                nfox += SQUARE_SIZE;
            }
            else nfox = 0.0; 
        }

        while (nfox >= SQUARE_SIZE) 
        {
            if (nX < Lx) 
            {
                nX++; 
                nfox -= SQUARE_SIZE;
            }
            else
            {
                nX = Lx - 1;
                nfox = 39; 
            }
        }
        if (nX == Lx - 1) 
        {
            nfox = 0;
        }

        // Now check for walkability
        u_int16 nbr_sqr = nX > X ? nX - X : X - nX;
        placeable_area * state = current_state();
        
        for (u_int16 j = 0; j < state->area_height(); j++)
            for (u_int16 i = 0; i < state->area_length(); i++)
            {
                if (state->get(i, j).is_walkable()) continue;

                u_int16 px = x() - state->base.x() + i - (vx() < 0 ? nbr_sqr : 0);
                u_int16 py = y() - state->base.y() + j;
                
                u_int16 nbx = 1 + (nfox != 0) + (vx() > 0 ? nbr_sqr : 0);
                u_int16 nby = 1 + (oy() != 0);

                for (u_int16 l = 0; l < nby; l++)
                    for (u_int16 k = 0; k < nbx; k++)
                    {
                        square * msqr = Mymap.get(px + k, py + l);
                        for (square::iterator it = msqr->begin(); it != msqr->end(); it++)
                        {
                            if (it->obj->current_state()->get(px + k - it->x() + it->obj->current_state()->base.x(),
                                                               py + l - it->y() + it->obj->current_state()->base.y())
                                .is_walkable()) continue;

                            if (z() + climb_capability() < it->z() + it->obj->current_state()->zsize && 
                                z() + state->zsize > it->z())
                                goto ytest;
                        }
                    }
            }

        X = nX;
        fox = nfox;
        Ox = (u_int16) fox;
    }

 ytest:

    if (vy ()) 
    {
        u_int16 nY = Y;
        float nfoy = foy;

        Has_moved = 1; 

        nfoy += vy ();
        while (nfoy < 0) 
        {
            if (nY > 0)
            {
                nfoy += SQUARE_SIZE;
                nY--;
            }
            else nfoy = 0.0; 
        }
        
        while (nfoy >= SQUARE_SIZE) 
        {
            if (nY < Ly) 
            {
                nfoy -= SQUARE_SIZE;
                nY++;
            }
            else 
            {
                nY = Ly - 1;
                nfoy = 39; 
            }
        }
        if (nY == Ly - 1) 
        {
            nfoy = 0; 
        }

        // Now check for walkability
        u_int16 nbr_sqr = nY > Y ? nY - Y : Y - nY;
        placeable_area * state = current_state();
        
        for (u_int16 j = 0; j < state->area_height(); j++)
            for (u_int16 i = 0; i < state->area_length(); i++)
            {
                if (state->get(i, j).is_walkable()) continue;

                u_int16 px = x() - state->base.x() + i;
                u_int16 py = y() - state->base.y() + j - (vy() < 0 ? nbr_sqr : 0);
                
                u_int16 nbx = 1 + (ox() != 0);
                u_int16 nby = 1 + (nfoy != 0) + (vy() > 0 ? nbr_sqr : 0);

                for (u_int16 l = 0; l < nby; l++)
                    for (u_int16 k = 0; k < nbx; k++)
                    {
                        square * msqr = Mymap.get(px + k, py + l);
                        for (square::iterator it = msqr->begin(); it != msqr->end(); ++it)
                        {
                            if (it->obj->current_state()->get(px + k - it->x() + it->obj->current_state()->base.x(),
                                                               py + l - it->y() + it->obj->current_state()->base.y())
                                .is_walkable()) continue;

                            if (z() + climb_capability() < it->z() + it->obj->current_state()->zsize && 
                                z() + state->zsize > it->z())
                                goto ztest;
                        }
                    }
            }

        Y = nY;
        foy = nfoy;
        Oy = (u_int16) foy;
    }

 ztest:

    s_int32 prevz = z();

    if (vz())
    {
        Has_moved = 1;

        foz += vz();
        while (foz <= -1.0)
        {
            Z--;
            foz += 1.0;
        }
        while (foz >= 1.0)
        {
            Z++;
            foz -= 1.0;
        }
    }

    placeable_area * state = current_state();

    Is_falling = true;
    
    for (int j = 0; j < state->area_height(); j++)
        for (int i = 0; i < state->area_length(); i++)
        {
            if (state->get(i, j).is_walkable()) continue;
            u_int16 px = x() - state->base.x() + i;
            u_int16 py = y() - state->base.y() + j;

            u_int16 nbx = 1 + (ox() != 0);
            u_int16 nby = 1 + (oy() != 0);

            for (u_int16 l = 0; l < nby; l++)
                for (u_int16 k = 0; k < nbx; k++)
                {
                    square * msqr = Mymap.get(px + k, py + l);
                    for (square::iterator it = msqr->begin(); it != msqr->end(); it++)
                    {
                        if (vz() > 0)
                        {
                            s_int32 objz = it->z();
                            if (objz > z() + current_state()->zsize || objz < prevz + current_state()->zsize) continue;

                            if (!it->obj->current_state()->get(px + k - it->x() + it->obj->current_state()->base.x(),
                                                               py + l - it->y() + it->obj->current_state()->base.y())
                                .is_walkable()) 
                            {
                                set_altitude(objz - (current_state()->zsize + 1));
                                set_vertical_velocity(0.0);
                                break;
                            }
                        }
                        else
                        {
                            s_int32 objz = it->z() + it->obj->current_state()->zsize;
                            if (objz < z() || objz > prevz + climb_capability()) continue;
                            
                            if (!it->obj->current_state()->get(px + k - it->x() + it->obj->current_state()->base.x(),
                                                               py + l - it->y() + it->obj->current_state()->base.y())
                                .is_walkable()) 
                            {
                                set_altitude(objz);
                                Is_falling = false;
                                set_vertical_velocity(0.0);
                                break;
                            }
                        }
                    }
                }
        }
}

bool moving::update()
{
    Mymap.remove(this);

    update_pos2();

    Mymap.put(this);
    return true; 
}
