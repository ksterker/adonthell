#
# $Id: transformable_item.py,v 1.1 2004/07/03 05:59:00 ksterker Exp $
#   
# Copyright (C) 2003/2004 Kai Sterker <kaisterker@linuxgames.com>
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

from items import item
from adonthell.rpg import item_storage

#
# A basic item that can be transformed into a different item
#
class transformable_item (item.item):

    def __init__ (self):
        # -- init base class
        item.item.__init__(self)

        # -- item required for the transformation
        self.Agent = ""

        # -- result of the transformation
        self.Result = ""

        
    def combine (self, item):
        instance = item.get_instance ()
        
        # -- is item the required agent?
        if instance.Name == self.Agent or instance.is_a (self.Agent):
            return item_storage.get (self.Result)
        
        return None
        
    
    def put_state (self, record):
        # -- save state of parent class
        item.item.put_state (self, record)

        # -- save attributes
        record.put_string ("tia", self.Agent)
        record.put_string ("tir", self.Result)
        
        
    def get_state (self, record):
        # -- load state of parent class
        item.item.get_state (self, record)
        
        # -- save attributes
        self.Agent = record.get_string ("tia")
        self.Result = record.get_string ("tir")
        
