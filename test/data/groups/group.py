#
# Copyright (C) 2009 Frederico Cerveira <frederico.cerveira@gmail.com>
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

class group (object):
    """
     This is the python counterpart to rpg/group.h. It contains an
     group's most basic attributes. Any actual group should inherit
     from this class.
    """ 
        
    def __init__ (self):
        """ctor"""
        # -- reference to the underlying rpg.group instance
        self.this = None

    def destroy (self):
        """properly delete a group"""
        if self.this != None:
            self.this.destroy ()
    
    def estimate_speed (self, terrain):
        """ returns a percentage depending on how nicely this group
            and the terrain interact.
            @param terrain the type of terrain we want to interact with
        """
        return 0 # -- there's no effect
        
    def estimate_speed_for_pathfinding (self, terrain):
        """ similar to estimate_speed but now the result is going to
            be used in all the pathfinding searchs. Therefore adding
            special cases here can mold the behaviour of a NPC.
            For example a noble NPC won't cross a swamp unless he is under attack
            @param terrain the type of terrain we want to interact with
        """
        return self.estimate_speed(terrain)
