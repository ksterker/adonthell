/*
 $Id: chunk.cc,v 1.9 2009/01/09 22:39:41 ksterker Exp $
 
 Copyright (C) 2008/2009 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file   world/chunk.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Implements the chunk class.
 * 
 * 
 */

#include <algorithm>
#include "world/chunk.h"

using world::chunk;
using world::chunk_info;

/// node positions
typedef enum
{
    LEFT    = 0,
    RIGHT   = 1,
    FRONT   = 0,
    BACK    = 2,
    BOTTOM  = 0,
    TOP     = 4
} position;

/// number of objects in a node before we try to split it
#define MAX_OBJECTS 16
/// minimum node size (in all 3 dimensions)
#define MIN_SIZE 240
 
bool chunk_info::operator == (const chunk_info & ci) const
{
    if (Object == ci.Object)
    {
        // we only check min, as that is the objects position. Max might
        // change if the shape of the object changes.
        return Min == ci.Min;
    }
    
    return false;
}

// ctor
chunk::chunk () : Split ()
{
    // chunk does not have to be resized
    Resize = false;
    // initialise children to NULL
    memset (Children, 0, 8 * sizeof(chunk*));
}

// add an object to chunk
void chunk::add (placeable * object, const coordinates & pos)
{
    // calculate axis-aligned bbox for object
    vector3<s_int32> max (pos.x() + object->max_length(),
                          pos.y() + object->max_width(),
                          pos.z() + object->max_height());
    
    add (chunk_info (object, pos, max));
}

void chunk::remove (placeable * object, const coordinates & pos)
{
    // calculate axis-aligned bbox for object
    vector3<s_int32> max (pos.x() + object->max_length(),
                          pos.y() + object->max_width(),
                          pos.z() + object->max_height());
    
    remove (chunk_info (object, pos, max));
}

// add an object to chunk
void chunk::add (const chunk_info & ci)
{
    // update bounding box of chunk
    Min.set_x (std::min (Min.x(), ci.Min.x()));
    Min.set_y (std::min (Min.y(), ci.Min.y()));
    Min.set_z (std::min (Min.z(), ci.Min.z()));

    Max.set_x (std::max (Max.x(), ci.Max.x()));
    Max.set_y (std::max (Max.y(), ci.Max.y()));
    Max.set_z (std::max (Max.z(), ci.Max.z()));

    // we're in a leaf ...
    if (is_leaf())
    {
        // ... and we have room for another object
        if (Objects.size() < MAX_OBJECTS || !can_split())
        {
            Objects.push_back (ci);
            
            // end recursion
            return;
        }
        
        // otherwise we need to split the chunk
        else
        {
            split ();
            
            s_int8 chunks[8];
            std::list<chunk_info>::iterator i = Objects.begin();
            while (i != Objects.end ())
            {
                const u_int8 num = find_chunks (chunks, (*i).Min, (*i).Max);
                
                // if objects would be split between children, we have to keep them
                // in the current node
                if (num == 1)
                {
                    chunk *c = Children[chunks[0]];
                    
                    // chunk might be NULL, if no objects live in that part of space yet.
                    if (c == NULL)
                    {
                        c = new chunk;
                        c->Min = (*i).Min;
                        Children[chunks[0]] = c; 
                    }
                    
                    // recurse
                    c->add (*i);

                    // erase object as its now part of child
                    i = Objects.erase (i);
                    continue;
                }
                
                i++;
            }
            
            // don't return, we have to add the new object as well!
        }
    }
    
    // we're not (or no longer) in a leaf, so we need to find the chunk
    // to which we add the object to
    s_int8 chunks[8];
    const u_int8 num = find_chunks (chunks, ci.Min, ci.Max);
    if (num == 1)
    {
        chunk *c = Children[chunks[0]];
        
        // c might be NULL, if no objects live in that part of space yet.
        if (c == NULL)
        {
            c = new chunk;
            c->Min = ci.Min;
            Children[chunks[0]] = c; 
        }
        
        // recurse
        c->add (ci);
    }
    else
    {
        Objects.push_back (ci);
        
        // TODO: here's where we most likely would like to rebalance the tree
        // if the size of objects grows too big again
    }
}

void chunk::remove (const chunk_info & ci)
{
    if (!is_leaf())
    {
        s_int8 chunks[8];
        const u_int8 num = find_chunks (chunks, ci.Min, ci.Max);
        if (num == 1)
        {
            chunk *c = Children[chunks[0]];
            if (c != NULL)
            {
                c->remove (ci);
                
                // we can get rid of empty leafs
                if (c->is_empty() && c->is_leaf())
                {
                    delete c;
                    Children[chunks[0]] = NULL;
                }
                
                return;
            }
        }
    }
    
    std::list<chunk_info>::iterator it = find (Objects.begin(), Objects.end(), ci);
    if (it != Objects.end())
    {
        if (!Resize || 
            Min.x() == (*it).Min.x() || Min.y() == (*it).Min.y() || Min.z() == (*it).Min.z() ||
            Max.x() == (*it).Max.x() || Max.y() == (*it).Max.y() || Max.z() == (*it).Max.z())
        {
            Resize = true;
        }
        
        Objects.erase (it);
    }
}

// return list of objects in the given view
std::list<world::chunk_info> chunk::objects_in_view (const s_int32 & x, const s_int32 & y, const s_int32 & z, const s_int32 & length, const s_int32 & width) const
{
    std::list<chunk_info> result;
    objects_in_view (x, x + length, y - z, y - z + width, result);
    return result;
}

// recursively collect objects in given view
void chunk::objects_in_view (const s_int32 & min_x, const s_int32 & max_x, const s_int32 & min_yz, const s_int32 & max_yz, std::list<chunk_info> & result) const
{
    // process childrem
    for (u_int32 i = 0; i < 8; i++)
    {
        chunk *c = Children[i];
        if (c != NULL && in_view (min_x, max_x, min_yz, max_yz, c->Min, c->Max))
        {
            // recurse
            c->objects_in_view (min_x, max_x, min_yz, max_yz, result);
        }
    }

    // process contained map objects
    std::list<chunk_info>::const_iterator i;
    for (i = Objects.begin (); i != Objects.end(); i++)
    {
        if (in_view (min_x, max_x, min_yz, max_yz, (*i).Min, (*i).Max))
        {
            result.push_back (*i);
        }
    }
    
    /*
    // we actually searched this node because its size is no longer accurate 
    if (Resize && num == result.size())
    {
        vector3<s_int32> tmp = Max;
        Max = Min;
        Min = Max;
        
        for (i = Objects.begin (); i != Objects.end(); i++)
        {
            Min.set_x (std::min (Min.x(), (*i).Min.x()));
            Min.set_y (std::min (Min.y(), (*i).Min.y()));
            Min.set_z (std::min (Min.z(), (*i).Min.z()));
            
            Max.set_x (std::max (Max.x(), (*i).Max.x()));
            Max.set_y (std::max (Max.y(), (*i).Max.y()));
            Max.set_z (std::max (Max.z(), (*i).Max.z()));
        }
        
        Resize = false;
    }
     */
}

// does given AABB overlap with mapview?
bool chunk::in_view (const s_int32 & min_x, const s_int32 & max_x, const s_int32 & min_yz, const s_int32 & max_yz, const vector3<s_int32> & min, const vector3<s_int32> & max) const
{
    // no overlap on x-axis
    if (max_x < min.x() || min_x > max.x()) return false;
    // no overlap on y/z-axis
    if (max_yz < (min.y() - max.z()) || min_yz > (max.y() - min.z())) return false;
    
    return true;
}

std::list<chunk_info> chunk::objects_in_bbox (const vector3<s_int32> & min, const vector3<s_int32> & max) const
{
    std::list<chunk_info> result;
    objects_in_bbox (min, max, result);
    return result;
}

void chunk::objects_in_bbox (const vector3<s_int32> & min, const vector3<s_int32> & max, std::list<chunk_info> & result) const
{
    s_int8 chunks[8];
    
    // process children
    u_int8 num = find_chunks (chunks, min, max);
    for (u_int32 i = 0; i < num; i++)
    {
        chunk *c = Children[chunks[i]];
        if (c != NULL)
        {
            // recurse
            c->objects_in_bbox (min, max, result);
        }
    }
    
    // process objects contained in chunk
    std::list<chunk_info>::const_iterator i;
    for (i = Objects.begin (); i != Objects.end(); i++)
    {
        if (in_bbox (min, max, i->real_min(), i->real_max()))
        {
            result.push_back (*i);
        }
    }
}

// does given AABBs overlap with each other?
bool chunk::in_bbox (const vector3<s_int32> & a_min, const vector3<s_int32> & a_max, const vector3<s_int32> & b_min, const vector3<s_int32> & b_max) const
{
    // no overlap on x-axis
    if (a_max.x() < b_min.x() || a_min.x() > b_max.x()) return false;
    // no overlap on y-axis
    if (a_max.y() < b_min.y() || a_min.y() > b_max.y()) return false;
    // no overlap on z-axis
    if (a_max.z() < b_min.z() || a_min.z() > b_max.z()) return false;
    
    return true;
}

// return a list of chunks that intersect with the given bbox
const u_int8 chunk::find_chunks (s_int8 chunks[8], const vector3<s_int32> & min, const vector3<s_int32> & max) const
{
    // FIXME: we may need to call in_bbox here, to make sure that the given bbox 
    // intersects with this chunk at all. If it does not, the code below will 
    // return wrong results. Maybe it should be rewritten to consider the chunk
    // boundaries as well as the split plane.
    
    // usually, there will be one chunk returned, but there might be two, four or eight
    u_int8 num = 1;
    
    /*
     The algorithm works as follows:
     
     For each of the 3 coordinates, we check if it lies in the "smaller" or 
     "larger" half of this chunk.
     - if it is in the larger half, we're done
     - otherwise, we need to check if it intersects with the larger half.
       If it does intersect, the number of chunks returned will double.
     
     Each coordinate is represented by a single bit, that is either set or not.
     If it lies in the smaller half, it is not set, otherwise it is set. Combined,
     these 3 bits result in a value between 0 and 7, which directly corresponds to
     the sub-cube containing the coordinate-triplet.
     
     (I know, that's probably the worst explanation ever ... please update as
      appropriate)
     */
    
    u_int8 a = min.x() < Split.x() ? LEFT : RIGHT;
    chunks[0] = a;
    if (a == 0)
    {
        a = max.x() <= Split.x() ? LEFT : RIGHT;
        if (a != 0)
        {
            chunks[1] = a;
            num = 2;
        }
    }

    a = min.y() < Split.y() ? FRONT : BACK;
    if (a != 0)
    {
        for (int i = 0; i < num; i++)
            chunks[i] |= a; 
    }
    else
    {
        a = max.y() <= Split.y() ? FRONT : BACK;
        if (a != 0)
        {
            for (int i = 0; i < num; i++)
                chunks[i+num] = chunks[i] | a;
            num *= 2;
        }
    }

    a = min.z() < Split.z() ? BOTTOM : TOP;
    if (a != 0)
    {
        for (int i = 0; i < num; i++)
            chunks[i] |= a;
    }
    else
    {
        a = max.z() <= Split.z() ? BOTTOM : TOP;
        if (a != 0)
        {
            for (int i = 0; i < num; i++)
                chunks[i+num] = chunks[i] | a;
            num *= 2;
        }
    }
    
    return num;
}

// split chunk into (up to) 8 parts
void chunk::split ()
{
    // split node, but do not allow anything smaller than our minimum node size.
    Split.set (
        Min.x() + std::max (MIN_SIZE, (Max.x() - Min.x()) / 2),
        Min.y() + std::max (MIN_SIZE, (Max.y() - Min.y()) / 2),
        Min.z() + std::max (MIN_SIZE, (Max.z() - Min.z()) / 2));

    // make sure we are aligned to a multiple of regular tile size, so we get
    // less overlap
    Split.set_x (Split.x() - Split.x() % MIN_SIZE);
    Split.set_y (Split.y() - Split.y() % MIN_SIZE);
    Split.set_z (Split.z() - Split.z() % MIN_SIZE);
}

// check whether we can further split a node
bool chunk::can_split () const
{
    vector3<s_int32> size = Max - Min;
    static s_int32 min_size2 = MIN_SIZE * 2;
    return size.x() >= min_size2 || size.y() >= min_size2 || size.z() >= min_size2;
}

// create a picture of the tree in .dot format
void chunk::debug () const
{
    char out[128];
    sprintf (out, "%i\\n[%i %i %i]-[%i %i %i]\\n[%i %i %i]", Objects.size(),
             Min.x(), Min.y(), Min.z(), Max.x(), Max.y(), Max.z(), Split.x(), Split.y(), Split.z());
    std::ofstream graph ("chunk_debug.dot");
    graph << "digraph chunk_debug {" << std::endl;
    graph << "n0 [label=\"" << out << "\"];" << std::endl;  
    
    for (int i = 0; i < 8; i++) 
    {
        if (Children[i] != NULL) 
            Children[i]->debug (graph, 0);
    }
    
    graph << "}" << std::endl;
    graph.close();
}

// recursively create a picture pf the tree's internal nodes
void chunk::debug (std::ofstream & graph, const int & parent) const
{
    static char out[128];
    static int node = 0;
    int cur_node = ++node;
    sprintf (out, "%i\\n[%i %i %i]-[%i %i %i]\\n[%i %i %i]", Objects.size(),
             Min.x(), Min.y(), Min.z(), Max.x(), Max.y(), Max.z(), Split.x(), Split.y(), Split.z());
    graph << "  n" << cur_node << " [label=\"" << out << "\"];" << std::endl;  
    graph << "  n" << parent << " -> n" << cur_node << ";\n" << std::endl;
    
    for (int i = 0; i < 8; i++) 
    {
        if (Children[i] != NULL) 
            Children[i]->debug (graph, cur_node);
    }    
}
