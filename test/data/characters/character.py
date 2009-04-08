#
# $Id: character.py,v 1.1 2009/04/08 19:36:03 ksterker Exp $
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

from adonthell.base import flat

"""
 This is the python counterpart to rpg/character.cc. It contains an
 character's most basic attributes. Any actual character should inherit
 from this class.
"""
class character (object):

    """ctor"""
    def __init__ (self):
        # -- pointer to the underlying rpg.character instance
        self.this = None


    """properly delete a character"""
    def destroy (self):
        if self.this != None:
            self.this.destroy ()
            
    
    """
     save character to disk
     record needs to be of type base.flat
    """
    def put_state (self, record):
        pass
        
    """
     load character from disk
     record needs to be of type base.flat
    """
    def get_state (self, record):
        pass
