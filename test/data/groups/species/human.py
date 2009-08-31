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
import specie

class human (specie.specie):
    """
     This is an example implementation of the Human Specie
    """ 
    
    def __init__ (self):
        """ctor"""
        specie.specie.__init__(self)
        

    
    def estimate_speed (self, terrain):
        """ returns a percentage depending on how nicely this group
            and the terrain interact.
            @param terrain the type of terrain we want to interact with
        """
        # -- Convert terrain to lowercase
        terrain = terrain.lower()
        
        # -- Dictionary with the terrains and their respective impact
        dic = { "wood"  :  15 ,
                "swamp" : -45 ,
                "grass" : -10 ,
                "water" : -50
              }
        
        try:
            # -- correspondence found
            return dic[terrain]
        except:
            # -- the terrain passed does not match any known one
            # -- return 0 by default
            return 0
