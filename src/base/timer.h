/*
   $Id: timer.h,v 1.2 2003/11/22 09:35:21 ksterker Exp $

   Copyright (C) 2003 Alexandre Courbot.
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

#include <iostream>
#include <sys/time.h>
#include <time.h>

namespace base
{
    class timer
    {
    public:
        timer();
        unsigned long int slice() const { return Slice; }
        unsigned long int current_time() const;
        unsigned long int frames_missed() const { return Frames_missed; }
        void sleep(unsigned long int msecs) const;

        void set_slice(unsigned long int sl);

        void update();

    private:
        unsigned long int convert_timeval (const struct timeval & tv) const
        { return ((tv.tv_sec - initial_time.tv_sec) * 1000 + (tv.tv_usec - initial_time.tv_usec) / 1000); }
        struct timeval initial_time;

        unsigned long int Slice;
        unsigned long int Lasttime;
        unsigned long int Frames_missed;
    };
}
