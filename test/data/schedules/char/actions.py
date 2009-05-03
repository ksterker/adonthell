#
# $Id: actions.py,v 1.2 2009/05/03 16:26:01 ksterker Exp $
#   
# Copyright (C) 2009 Kai Sterker <kaisterker@linuxgames.com>
# Part of the Adonthell Project http://adonthell.linuxgames.com
#
# Adonthell is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# Adonthell is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Adonthell; if not, write to the Free Software 
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#

from adonthell import rpg, world
import math

ACTION_NORMAL = 0
ACTION_COMBAT = 1
ACTION_SNEAK  = 2

def perform_action (chr, action):
    """
     Let the given character perform the given action.
     
     This function will determine if there is an object
     present that can be acted upon and if so will
     perform the action with this object.
     
     @param chr the map character instance perfomring the action
     @param action the type of action being performed
    """
    # -- get the character id
    name = chr.uid()
    # -- get associated rpg instance
    rpg_char = rpg.character.get_character (name)
    # -- get the Python character script
    py_rpg_char = rpg_char.get_instance ()
    
    # -- the area of effect is given as a radius and an angle
    #    with the character at the center
    radius, arc = py_rpg_char.get_area_of_effect (action)

    # -- the objects in the area of effect
    object_list = get_objects (chr, radius, arc, world.CHARACTER | world.ITEM)
    
    # -- any objects found at all?
    if len(object_list) > 0:
        # -- perform the action with the closest object
        chunk_data = object_list[0]
        # -- get object id
        obj_name = chunk_data.get_entity().id()
        # -- get associated rpg instance
        #    FIXME in the future, obj might be an item too
        rpg_obj = rpg.character.get_character (obj_name)
        # -- get the Python character script
        py_rpg_obj = rpg_obj.get_instance ()
        # -- execute the action
        py_rpg_obj.perform_action (action, chr)


_opposite_direction = {
    world.character.NONE  : world.character.NONE,
    world.character.EAST  : world.character.WEST,
    world.character.SOUTH : world.character.NORTH,
    world.character.WEST  : world.character.EAST,
    world.character.NORTH : world.character.SOUTH,
    world.character.SOUTH | world.character.EAST : world.character.NORTH | world.character.WEST,
    world.character.SOUTH | world.character.WEST : world.character.NORTH | world.character.EAST,
    world.character.NORTH | world.character.EAST : world.character.SOUTH | world.character.WEST,
    world.character.NORTH | world.character.WEST : world.character.SOUTH | world.character.EAST
}
"""
 Mapping of direction a character is facing and the
 opposite of that direction
"""


def face_character (myself, other):
    """
     Change the direction of myself to look at the other character
     @param myself map character whose direction to change
     @param other map character at whom to look
    """
    direction = _opposite_direction[other.heading ()]
    # -- look towards other character
    myself.set_direction (direction)
    # -- FIXME: this is pretty awkward
    myself.stop ()
    myself.update_state ()
    

_angle_of_direction = {
    world.character.NONE  : 0,
    world.character.EAST  : 0,
    world.character.SOUTH : 90,
    world.character.WEST  : 180,
    world.character.NORTH : -90,
    world.character.SOUTH | world.character.EAST : 45,
    world.character.SOUTH | world.character.WEST : 135,
    world.character.NORTH | world.character.EAST : -45,
    world.character.NORTH | world.character.WEST : -135
}
"""
 Mapping of direction a character is facing and the
 an angle in degrees
"""

def get_objects (chr, radius, arc, type):
    """
     Return a sorted list of objects of the given type that
     are in the given radius and arc from the given characters center
     and the direction the character is facing. The objects in the
     list are of type world.chunk_info, which allows access to object
     coordinate, unique id and actual object instance.
     
     @param chr the character whose surroundings to check
     @param radius the distance to check for objects
     @param arc an angle between 0 and 360 degree
     @param type the type(s) of objects to return
     
     @return list of objects in given location.
    """
    # -- center of the character
    chr_x = chr.x() + chr.length()/2
    chr_y = chr.y() + chr.width()/2

    # -- get bbox enclosing area of effect around the character
    min = world.vector3i (chr_x - radius, chr_y - radius, chr.z())
    max = world.vector3i (chr_x + radius, chr_y + radius, chr.z() + chr.height())
        
    # -- get list of characters and items enclosed be bbox
    object_list = chr.map().objects_in_bbox (min, max, type)

    # -- get start and end of the area of effect
    end = arc/2 + _angle_of_direction[chr.heading ()]
    start = end - arc

    result_list = []

    # -- get uid to exclude this character from result list
    name = chr.uid()

    # -- find objects actually in the area of effect
    for chunk_data in object_list:
        # -- get the map entity
        entity = chunk_data.get_entity()
        # -- do not consider ourselves
        if entity.id() == name: continue
        
        # -- the actual object represented by the entity
        object = entity.get_object()
        
        # -- line between object and character
        px = chunk_data.Min.x() + object.length()/2 - chr_x
        py = chunk_data.Min.y() + object.width()/2  - chr_y
        
        # -- calculate angle between line and x-axis
        angle = math.degrees (math.atan2 (py, px))

        # -- if facing west, angle might be around 180/-180 
        if chr.heading() == world.character.WEST:
            angle = abs(angle)
        
        # -- check if angle is in given arc
        if angle >= start and angle <= end:
            # -- the distance between character and object
            distance = math.hypot (px, py)
            # -- proximity of object to the direction the character is facing
            direction = math.fabs ((start + end)/2 - angle)
            # -- add tuple of sorting criteria and object to result list
            result_list.append ((distance * direction, direction, chunk_data))

    # -- sort result list by sorting criteria
    result_list.sort()
    # -- strip sorting criteria from result
    return [chunk_data for ignore, ignore, chunk_data in result_list]
