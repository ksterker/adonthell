#
# $Id: weapon.py,v 1.1 2004/07/03 05:59:00 ksterker Exp $
#   
# Copyright (C) 2003 Kai Sterker <kaisterker@linuxgames.com>
# Part of the Adonthell Project http://adonthell.linuxgames.com
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License.
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY.
#
# See the COPYING file for more details.
#

import item

#
# This is the template for simple weapons.
#
class weapon (item.item):

    def __init__ (self):
        item.item.__init__ (self)
        self.add_category ("Weapon")
        
    def can_equip (self, character):
        return 1
        
    def equipped (self):
        print "%s has been equipped in slot %s" % (self.Name, self.this.get_slot ().id ())
    
    def unequipped (self):
        print "%s has been removed from slot %s" % (self.Name, self.this.get_slot ().id ())
    
