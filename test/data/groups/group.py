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
        
        # -- Group Name
        self.Name = ""
        
        # -- Dictionary with relation between the terrain and respective
        # -- imapct in speed
        self.Dic = {}


    def destroy (self):
        """properly delete a group"""
        if self.this != None:
            self.this.destroy ()
    
    def estimate_speed (self, terrain):
        try:
            return self.Dic[terrain]
        except: return 0
        
    # -- save item to disk
    #    record needs to be of type base.flat
    def put_state (self, record):
        record.put_string("pgpn", self.Name) 
        
        terrains = flat()
        for terrain in self.Dic: 
            terrains.put_sint8(terrain, self.Dic[terrain])
            
        record.put_flat ("gTerr", terrains)
        
    # -- load item from disk
    #    record needs to be of type base.flat
    def get_state (self, record):
        self.Name = record.get_string("pgpn")
        
        terrains = record.get_flat ("gTerr")

        type, value, unused, key = terrains.next ()
        while type == flat.T_SINT8:
            self.Dic[key] = value
            type, value, unused, key = terrains.next ()
        
        
