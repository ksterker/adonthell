#
# $Id: projectile.py,v 1.1 2004/07/03 05:59:00 ksterker Exp $
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
from adonthell import rpg

#
# Class for basic projectile weapons.
#
class projectile (item.item):
    def can_equip (self, character):
        return 1
        
    def equipped (self):
        print "%s has been equipped in slot %s" % (self.Name, self.this.get_slot ().id ())
    
    def unequipped (self):
        print "%s has been removed from slot %s" % (self.Name, self.this.get_slot ().id ())

    def combine (self, item):
        instance = item.get_instance ()
        
        # -- projectiles can be poisoned
        if instance.is_a ("Poison") and not self.is_a ("poisoned"):
        
            # -- create a (new) immutable item from projectile and poison
            name = self.Name + " of " + instance.Name
            
            # -- see whether this combination already exists
            result = rpg.item_storage.get (name)
            
            # -- otherwise create it
            if result == None:
                result = rpg.item (0)
                result.create_instance ("items.projectile", "projectile")
                result.set_max_stack (self.this.max_stack ())
                result.thisown = 0
                
                pyres = result.get_instance ()
                for category in self.Categories:
                    pyres.add_category (category)
                pyres.add_category ("poisoned")
                pyres.Name = name
                pyres.Value = self.Value + instance.Value / instance.MaxCharge
                
                # -- add it to item storage
                rpg.item_storage.add (result)
                
            # -- remove poison charge
            instance.Charge = instance.Charge - 1
            
            # -- destroy if no charges left
            if instance.Charge == 0: instance.destroy ()
            
            return result
        
        # -- combination failed
        return None
