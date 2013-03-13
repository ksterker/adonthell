/*
   $Id: file.cc,v 1.2 2004/11/03 07:30:16 ksterker Exp $

   Copyright (C) 1999/2000/2001/2002/2003 Alexandre Courbot
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
 * @file   base/file.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the igzstream, ogzstream and fileops classes.
 * 
 */

#include <stdio.h>
#include <iostream>
#include "endians.h"
#include "file.h"

using base::gz_file;
using base::igzstream;
using base::ogzstream;

gz_file::gz_file ()
{
    opened = false;
    file = NULL;
}

gz_file::gz_file (const string & fname, gz_type t)
{
    opened = false;
    open (fname, t);
}

gz_file::~gz_file ()
{
    if (is_open ()) gzclose (file);
}

bool gz_file::open (const string & fname, gz_type t)
{
    if (t == READ) file = gzopen (fname.c_str (),"rb6");
    else file = gzopen (fname.c_str (),"wb6");
    if (!file) return false;
    opened = true;
    return true;
}

void gz_file::close ()
{
    if (is_open ()) gzclose (file);
    opened = false;
}

igzstream::igzstream () : gz_file ()
{
}

igzstream::igzstream (const string & fname) : gz_file (fname, READ)
{
}

igzstream::~igzstream ()
{
}

bool igzstream::open (const string & fname)
{
    return gz_file::open (fname, READ);
}

/// Read a block of size chars
void igzstream::get_block (void * to, u_int32 size)
{
    gzread (file, to, size); 
}
    

/// Reads a boolean.
bool& base::operator << (bool& n, igzstream& gfile)
{
    u_int8 b;
    gzread (gfile.file, &b, sizeof (b));
    return (n = b);
}

/// Reads a char.
char& base::operator << (char& n, igzstream& gfile)
{
    gzread (gfile.file, &n, sizeof (n));
    return n;
}

/// Reads a u_int8.
u_int8& base::operator << (u_int8& n, igzstream& gfile)
{
    gzread(gfile.file, &n, sizeof (n));
    return n;
}

/// Reads a s_int8.
s_int8& base::operator << (s_int8& n, igzstream& gfile)
{
    gzread(gfile.file, &n, sizeof (n));
    return n;
}

/// Reads a u_int16.
u_int16& base::operator << (u_int16& n, igzstream& gfile)
{
    gzread(gfile.file, &n, sizeof (n));
    n = SwapLE16(n);
    return n;
}

/// Reads a s_int16.
s_int16& base::operator << (s_int16& n, igzstream& gfile)
{
    gzread(gfile.file, &n, sizeof (n));
    n = SwapLE16(n);
    return n;
}

/// Reads a u_int32.
u_int32& base::operator << (u_int32& n, igzstream& gfile)
{
    gzread(gfile.file, &n, sizeof (n));
    n = SwapLE32(n);
    return n;
}

/// Reads a s_int32.
s_int32& base::operator << (s_int32& n, igzstream& gfile)
{
    gzread(gfile.file, &n, sizeof (n));
    n = SwapLE32(n);
    return n;
}

/// Reads a string.
string& base::operator << (string& s, igzstream& gfile)
{
    u_int16 strl;
    char c;
    s = ""; 
    strl << gfile;
    while (strl)
    {
	c << gfile;
	s += c;
	strl --;
    }
    return s;
}

/// Reads a float.
float& base::operator << (float& f, igzstream& gfile)
{
    string sf;
    sf << gfile;
    
    // floats saved as strings to remain independent of architecture
    sscanf (sf.c_str (), "%f", &f);
    
    return f;
}


ogzstream::ogzstream () : gz_file ()
{
}

ogzstream::ogzstream (const string & fname) : gz_file (fname, WRITE)
{
}

ogzstream::~ogzstream ()
{
}

bool ogzstream::open (const string & fname)
{
    return gz_file::open (fname, WRITE);
}

void ogzstream::put_block (void * to, u_int32 size)
{
    gzwrite (file, to, size); 
}

/// Writes a boolean.
const bool& base::operator >> (const bool& n, ogzstream& gfile)
{
    u_int8 b = n;
    gzwrite (gfile.file, &b, sizeof (b));
    return n;
}

/// Writes a char.
const char& base::operator >> (const char& n, ogzstream& gfile)
{
    gzwrite (gfile.file, (char *) &n, sizeof (n));
    return n;
}

/// Writes a u_int8.
const u_int8& base::operator >> (const u_int8& n, ogzstream& gfile)
{
    gzwrite(gfile.file, (u_int8 *) &n, sizeof (n));
    return n;
}

/// Writes a s_int8.
const s_int8& base::operator >> (const s_int8& n, ogzstream& gfile)
{
    gzwrite(gfile.file, (s_int8 *) &n, sizeof (n));
    return n;
}

/// Writes a u_int16.
const u_int16& base::operator >> (const u_int16& n, ogzstream& gfile)
{
    u_int16 s = SwapLE16(n);
    gzwrite(gfile.file, (u_int16 *) &s, sizeof (n));
    return n;
}

/// Writes a s_int16.
const s_int16& base::operator >> (const s_int16& n, ogzstream& gfile)
{
    s_int16 s = SwapLE16(n);
    gzwrite(gfile.file, (s_int16 *) &s, sizeof (n));
    return n;
}

/// Writes a u_int32.
const u_int32& base::operator >> (const u_int32& n, ogzstream& gfile)
{
    u_int32 s = SwapLE32(n);
    gzwrite(gfile.file, (u_int32 *) &s, sizeof (n));
    return n;
}

/// Writes a s_int32.
const s_int32& base::operator >> (const s_int32& n, ogzstream& gfile)
{
    s_int32 s = SwapLE32(n);
    gzwrite(gfile.file, (s_int32 *) &s, sizeof (n));
    return n;
}

/// Writes a string.
string& base::operator >> (const string& s, ogzstream& gfile)
{
    u_int16 strl = s.length ();
    string::iterator i; 
    strl >>  gfile;
    
    for (i = ((string&) s).begin (); i != ((string&) s).end (); i++) 
        (*i) >>  gfile;
    return (string&) s;
}

/// Writes a float.
const float& base::operator >> (const float& f, ogzstream& gfile)
{
    char sf[16];
    
    // floats saved as strings to remain independent of architecture
    snprintf (sf, 16, "%f", f);
    ((string)sf) >> gfile;
    
    return f;
}
