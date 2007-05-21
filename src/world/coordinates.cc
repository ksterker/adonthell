/*
 $Id: coordinates.cc,v 1.2 2007/05/21 04:44:11 ksterker Exp $
 
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
 * @file   world/coordinates.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the coordinates class.
 * 
 * 
 */

#include "coordinates.h"

using world::coordinates;

coordinates::coordinates ()
{
    X = 0;
    Y = 0;
    Z = 0;
    Ox = 0;
    Oy = 0; 
}

coordinates::coordinates (u_int16 x, u_int16 y, u_int16 z, u_int16 ox, u_int16 oy)
{
    X = x; 
    Y = y;
    Z = z;
    Ox = ox; 
    Oy = oy; 
}

bool coordinates::operator == (const coordinates & mc)
{
    return X == mc.X && Y == mc.Y && Z == mc.Z && Ox == mc.Ox && Oy == mc.Oy; 
}

bool coordinates::operator < (const coordinates & mc)
{
    if (Y < mc.Y) return true;
    if (Y != mc.Y) return false;
    if (Oy < mc.Oy) return true;
    if (Oy != mc.Oy) return false;
    if (X < mc.X) return true;
    if (X != mc.X) return false;
    if (Ox < mc.Ox) return true;
    return false;
}

bool coordinates::operator <= (const coordinates & mc) 
{
    if (Y <= mc.Y) return true;
    if (Y != mc.Y) return false; 
    if (Oy <= mc.Oy) return true;
    if (Oy != mc.Oy) return false;
    if (X <= mc.X) return true;
    if (X != mc.X) return false;
    if (Ox <= mc.Ox) return true;
    return false; 
}

bool coordinates::operator > (const coordinates & mc) 
{
    if (Y > mc.Y) return true;
    if (Y != mc.Y) return false; 
    if (Oy > mc.Oy) return true;
    if (Oy != mc.Oy) return false;
    if (X > mc.X) return true;
    if (X != mc.X) return false;
    if (Ox > mc.Ox) return true;
    return false; 
}

bool coordinates::operator >= (const coordinates & mc)
{
    if (Y >= mc.Y) return true;
    if (Y != mc.Y) return false; 
    if (Oy >= mc.Oy) return true;
    if (Oy != mc.Oy) return false;
    if (X >= mc.X) return true;
    if (X != mc.X) return false;
    if (Ox >= mc.Ox) return true;
    return false; 
}

coordinates & coordinates::operator + (const coordinates & mc)
{
    X += mc.X;
    Y += mc.Y;
    Ox += mc.Ox;
    Oy += mc.Oy;

    while (Ox >= SQUARE_SIZE)
    {
        X++;
        Ox -= SQUARE_SIZE;
    }

    while (Oy >= SQUARE_SIZE)
    {
        Y++;
        Oy -= SQUARE_SIZE;
    }

    return *this;
}

coordinates & coordinates::operator - (const coordinates & mc)
{
    if (X < mc.X) X -= mc.X;
    else X = 0;

    if (Y < mc.Y) Y -= mc.Y;
    else Y = 0;

    if (Ox < mc.Ox)
    {
        s_int16 o;
        for (o = Ox - mc.Ox; o < 0; o += SQUARE_SIZE)
        {
            if (X) X--;
        }
        Ox = (u_int16) o;
    }
    else Ox -= mc.Ox;
    
    if (Oy < mc.Oy)
    {
        s_int16 o;
        for (o = Oy - mc.Oy; o < 0; o += SQUARE_SIZE)
        {
            if (Y) Y--;
        }
        Oy = (u_int16) o;
    }
    else Oy -= mc.oy();
    
    return *this;
}
