#
# $Id: item.py,v 1.1 2004/07/03 05:59:00 ksterker Exp $
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

from adonthell.base import flat

#
# This is the python counterpart to item_base.cc. It contains an
# item's most basic attributes. Any actual item should inherit
# from this class.
#
class item (object):

    def __init__ (self):
        # -- pointer to the underlying rpg.item instance
        self.this = None
        
        # -- The item's name
        self.Name = ""

        # -- List of types the item belongs to
        self.Categories = []

        # -- Number of uses the item has left
        self.Charge = 0

        # -- Maximum number of uses the item can have
        self.MaxCharge = 0

        # -- The item's objective worth
        self.Value = 0

        # -- the item's basic weight
        self.Weight = 0.0

        # -- additional weight per charge
        self.WeightPerCharge = 0.0


    # -- properly delete an item
    def destroy (self):
        if self.this != None:
            self.this.destroy ()
            
                                        
    # -- add item to another category
    def add_category (self, category):
        if category not in self.Categories:
            self.Categories.append (category)
    
    
    # -- remove category
    def remove_category (self, category):
        if category in self.Categories:
            self.Categories.remove (category)
            
    
    # -- check whether item belongs to given category
    #    returns 1 if this is the case, 0 otherwise.
    def is_a (self, category):
        return category in self.Categories
                
                
    # -- get item's weight
    def weight (self):
        return self.Weight + self.WeightPerCharge * self.Charge

    
    # -- recharge this item with charges of the given item
    def recharge (self, item):
        # -- check how much charge goes into the item
        charge_needed = self.MaxCharge - self.Charge
        
        # -- recharge
        if item.Charge > charge_needed:
            item.Charge = item.Charge - charge_needed
            self.Charge = self.MaxCharge
        else:
            self.Charge = self.Charge + item.Charge
            item.destroy ()
    
    
    # -- save item to disk
    #    record needs to be of type base.flat
    def put_state (self, record):
        record.put_string ("inm", self.Name)
        
        categories = flat ()
        for category in self.Categories: categories.put_string ("", category)
        record.put_flat ("ict", categories)
        
        record.put_uint16 ("icg", self.Charge)
        record.put_uint16 ("imc", self.MaxCharge)
        record.put_uint32 ("ivl", self.Value)
        record.put_float ("iwt", self.Weight)
        record.put_float ("iwc", self.WeightPerCharge)
        
        
    # -- load item from disk
    #    record needs to be of type base.flat
    def get_state (self, record):
        self.Name = record.get_string ("inm")
        categories = record.get_flat ("ict")

        type, value, unused, unused = categories.next ()
        while type == flat.T_STRING:
            self.add_category (value)
        
        self.Charge = record.get_uint16 ("icg")
        self.MaxCharge = record.get_uint16 ("imc")
        self.Value = record.get_uint32 ("ivl")
        self.Weight = record.get_float ("iwt")
        self.WeightPerCharge = record.get_float ("iwc")
