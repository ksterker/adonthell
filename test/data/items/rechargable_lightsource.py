#
# $Id: rechargable_lightsource.py,v 1.1 2004/07/03 05:59:00 ksterker Exp $
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

import lightsource

#
# A lightsource that can be recharged, such as a lamp. Using it will turn it
# on or off respectively, depending on its  current state. Combining it with
# "Lampfuel" will restore charges.
#
class rechargable_lightsource (lightsource.lightsource):

    # -- mark item as rechargable
    IsRechargable = 1
    
    # -- recharge item with "Lampfuel"
    def combine (self, item):
        fuel = item.get_instance ()
        print "Recharging Lamp with %s ..." % item.name ()
        if not fuel.is_a ("Lampfuel"): return

        # -- recharge item
        self.recharge (fuel)
        
        # -- update consume event with new Charge
        ConsumeLstnr = self.Events. get_listener ("consume")
        if ConsumeLstnr != None: 
            ConsumeEvent = ConsumeLstnr.get_event ()
            ConsumeEvent.set_repeat (self.Charge)

        return self.this
