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
import faction

class noble (faction.faction):
    """
     This is an example implementation of the Noble Faction
    """

    def __init__ (self):
        """ctor"""
        faction.faction.__init__(self)



    def estimate_speed (self, terrain):
        """ returns a percentage depending on how nicely this group
            and the terrain interact.
            @param terrain the type of terrain we want to interact with
        """
        # -- Convert terrain to lowercase
        terrain = terrain.lower()

        # -- Dictionary with the terrains and their respective impact
        dic = { "swamp" : -30 ,
                "grass" : -5  ,
                "water" : -20
              }

        try:
            # -- correspondence found
            return dic[terrain]
        except:
            # -- the terrain passed does not match any known one
            # -- return 0 by default
            return 0

    def estimate_speed_for_pathfinding (self, terrain):
        """ similar to estimate_speed but now the result is going to
            be used in all the pathfinding searchs. Therefore adding
            special cases here can mold the behaviour of a NPC.
            For example a noble NPC won't cross a swamp unless he is under attack
            @param terrain the type of terrain we want to interact with
        """
        # -- Convert terrain to lowercase
        terrain = terrain.lower()

        # -- Verify for special cases
        if (terrain == "swamp") or (terrain == "water"):
            return -1000 # -- Do not cross

        # -- Return the normal values
        return self.estimate_speed(terrain)
