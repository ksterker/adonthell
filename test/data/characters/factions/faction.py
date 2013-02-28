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

from adonthell.base import flat
from adonthell.rpg import faction

class faction (object):
    """
     This is the base class for any faction. Every faction should
     inherit from this one
    """
    
    def __init__ (self):
        """ctor"""
        # -- reference to the underlying rpg.faction instance
        self.this = None

        # -- Dictionary with relation between the terrain and respective
        # -- imapct in speed
        self.Dic = {}

    def destroy (self):
        """properly delete a group"""
        if self.this != None:
            self.this.destroy ()

    def requirements (self):
        """ 
            this function verifies if the C++ class calling this template fulfills
            the requirements of this social_class.
            @return 0 if any requirement fails, 1 if everything is OK
        """
        return 1
        
    def estimate_speed (self, terrain):
        try:
            # gc.collect()
            return self.Dic[terrain]
        except: return 0

    # -- save item to disk
    #    record needs to be of type base.flat
    def put_state (self, record):

        terrains = flat()
        for terrain in self.Dic:
            terrains.put_sint8(terrain, self.Dic[terrain])

        record.put_flat ("gTerr", terrains)

    # -- load item from disk
    #    record needs to be of type base.flat
    def get_state (self, record):

        terrains = record.get_flat ("gTerr")

        type, value, unused, key = terrains.next ()
        while type == flat.T_SINT8:
            self.Dic[key] = value
            type, value, unused, key = terrains.next ()
