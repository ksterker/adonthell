#
# $Id: lightsource.py,v 1.1 2004/07/03 05:59:00 ksterker Exp $
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

from adonthell import event
from items import item

#
# A basic lightsource, such as a torch or candle. Using it will turn it
# on or off respectively, depending on its  current state. One all charges
# are used up, it will be deleted. (use rechargable_lightsource if that
# behaviour isn't desired).
#
class lightsource (item.item):

    def __init__(self):
        # -- init base class
        item.item.__init__(self)
        
        # -- we need an event factory for storing time events
        self.Events = event.factory ()

        # -- duration of a charge (as gametime format string)
        self.Duration = "5m"
        
        # -- flag to indicate whether the torch is on or off
        self.IsBurning = 0

    def __del__(self):
        self.Events.clear ()
        
    def use (self, user):
		# -- turn it on
        if self.IsBurning == 0:

            # -- does it have enough charge?
            if self.Charge == 0: return
            
            # -- resume event
            self.Events.resume ()

            # -- get listener associated with the torch
            ConsumeLstnr = self.Events.get_listener ("consume")
            
            if ConsumeLstnr == None:
                # -- register time event so it is consumed while burning
                #    a charge will last 5 minutes
                ConsumeEvent = event.time_event (self.Duration)

                # -- repeat event as often as item has charges
                ConsumeEvent.set_repeat (self.Duration, self.Charge)

                # -- register consume event with event_list
                ConsumeLstnr = self.Events.add (ConsumeEvent)

                # -- callback to execute whenever a charge has been used
                #    up.
                ConsumeLstnr.connect_callback ("item_events", "item_events", "on_charge_used", self)

                # -- id to retrieve event later on
                ConsumeLstnr.set_id ("consume")
                
            
            self.IsBurning = 1

            # -- while it's burning, make it a lightsource
            self.add_category ("Lightsource")

            # -- start any graphical effects, etc
            # ...
  
        # -- switch it off
        else:
            self.IsBurning = 0
            
            # -- make sure event is not repeated
            self.Events.pause ()
            
            # -- not burning, so no lightsource
            self.remove_category ("Lightsource")

            # -- remove any graphical effects, etc
            # ...
            
    def put_state (self, record):
        # -- save state of parent class
        item.item.put_state (self, record)

        # -- save event list
        self.Events.put_state (record)

        # -- save own parameters
        record.put_string ("lsd", self.Duration)
        record.put_bool ("lsb", self.IsBurning)

    def get_state (self, record):
        # -- load state of parent class
        item.item.get_state (self, record)

        # -- load event list
        self.Events.get_state (record)

        # -- load own parameters
        self.Duration = record.get_string ("lsd")
        self.IsBurning = record.get_bool ("lsb")
