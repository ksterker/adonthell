/*
 $Id: entity.h,v 1.1 2008/07/10 20:19:41 ksterker Exp $
 
 Copyright (C) 2008 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file   world/entity.h
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Declares the entity class.
 */

#include "world/placeable.h"

#ifndef WORLD_ENTITY_H
#define WORLD_ENTITY_H

namespace world
{

/**
 * An anonymous entity on the map. Such entities can be
 * placed on the map, but there is no way to get at them
 * later. This is okay for most static scenery objects,
 * however.
 */
class entity
{
public:
    entity (placeable *object) : Object (object) { }
    
    virtual ~entity ()
    {
        delete Object;
    }
    
    placeable *get_object () const
    {
        return Object;
    }
    
    bool has_name () const 
    { 
        return id () != NULL; 
    }
    
    virtual const std::string * id () const 
    { 
        return NULL; 
    }
    
protected:
    /// an entity on a map
    placeable *Object;
};

/**
 * A uniquely identified entity. This may correspond to 
 * a unique object on the map (such as an individual 
 * character or door). It is also conceivable that the
 * same object is added as distinct instances multiple
 * times (i.e. items that will have the same appearence
 * on the map, but are backed by different rpg instances). 
 */
class named_entity : public entity
{
public:
    named_entity (placeable *object, const std::string & id) : entity (object), Id (id) { }
    
    const std::string * id () const
    {
        return &Id;
    }
    
private:
    /// name of entity
    std::string Id;
};

}

#endif // WORLD_ENTITY_H