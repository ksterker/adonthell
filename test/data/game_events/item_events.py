#
# $Id: item_events.py,v 1.1 2004/07/03 05:59:00 ksterker Exp $
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

#
# This class encapsulates callbacks used by items. They are written
# as generic as possible, but some might require items with a special
# interface to work properly.
#
class item_events:
    #
    # Decrease charge of an item by 1. Delete item if charge reaches 0.
    # Items containing the variable 'IsRechargable' with a value != 0
    # can avoid this.
    #
    def on_charge_used (self, item):
        # -- decrease item's charge
        item.Charge = item.Charge - 1

        # -- if no charges left, 'turn off' item
        if item.Charge == 0:
            # -- the 'use' method will do that for us
            item.use (None)
            
            # -- items that are not rechargable will be
            #    deleted when their charge is used up
            try:
                if item.IsRechargable == 0: del item 
            catch:
                del item
        