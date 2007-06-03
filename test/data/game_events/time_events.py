#
# $Id: time_events.py,v 1.1 2007/06/03 21:09:16 ksterker Exp $
#   
# Copyright (C) 2007 Kai Sterker <kaisterker@linuxgames.com>
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

from adonthell import base

#
# This class implements test callbacks for time events. 
#
class event_script (object):
    def __init__ (self):
        self.counter = 0
        
    def callback_1 (self, lst, evt, str, num):
        print str, num, evt.time (), base.Timer.uptime (), lst.get_event ().repeat ()
        lst.connect_callback ("time_events", "event_script", "callback_1", (str, num+1))
        self.counter += 1
        
    def callback_2 (self, lst, evt):
        print "#2", self.counter, evt.time (), base.Timer.uptime (), lst.get_event ().repeat ()

        # -- switch callback
        if self.counter == 4:
            print "#2 Switching to callback_3 / Increasing repeat count to 4"
            lst.connect_callback ("time_events", "event_script", "callback_3", ("#2",))
            lst.get_event ().set_repeat (4)
            
    def callback_3 (self, lst, evt, num):
        print num, evt.time (), base.Timer.uptime (), lst.get_event ().repeat ()
        if lst.get_event ().repeat () == 2:
            # -- destroy
            print "#2 Destroying at repeat count '2'"
            lst.destroy ()
