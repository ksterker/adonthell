#
# $Id: player.py,v 1.1 2009/04/16 21:06:10 ksterker Exp $
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

from adonthell import input, world

"""
 Implements the PC "schedule", which is being controlled
 by the player.
"""
class player (object):
    
    """
     ctor
    """
    def __init__ (self, schedule):
        # -- the map for this schedule
        self.schedule = schedule
        # -- Whether PC is always running
        self.always_run = 0
        # -- The PC map character instance
        self.pc = schedule.get_map().get_character ("Player")
    
    """
     Called once to start the schedule manager.
    """
    def run (self):
        # -- set the actual schedule script
        self.schedule.set_schedule ("player")
        self.schedule.set_running (1)

    """
     Called when the schedule is first assigned
    """
    def start (self):
        # -- Create our input_listener 
        self.il = input.listener()

        # -- Add the listener to the manager
        input.manager.add(self.il)
        
        # -- connect the callback to handle key/control events
        self.il.connect_control_function(self.handle_controls)
        self.il.connect_keyboard_function(self.handle_keys)
    
    """
     called whenever the character needs to freeze
    """
    def pause (self):
        # -- stop listening to events while paused
        self.il.disconnect_control_function ()
        self.il.disconnect_keyboard_function ()
        
    """
     Called when the schedule is first assigned
    """
    def resume (self):
        # -- start listening to key/control events again
        self.il.connect_control_function(self.handle_controls)
        self.il.connect_keyboard_function(self.handle_keys)

    """ 
     called before the schedule is removed
    """
    def stop (self):
        # -- clean up
        input.manager.remove (self.il)
        self.il.disconnect_control_function ()
        self.il.disconnect_keyboard_function ()
        self.il = none
        
    """
     Called whenever a regular key has bee pushed.
    """
    def handle_keys (self, kev):
        # -- caps lock toggles running on or off
        if kev.key() == input.keyboard_event.CAPSLOCK_KEY:
            if kev.type() == input.keyboard_event.KEY_PUSHED:
                self.always_run = 1
                self.pc.run ()
            else:
                self.always_run = 0
                self.pc.walk ()
                
        return 1

    """
     Called whenever a control key has bee pushed.
    """
    def handle_controls (self, cev):
        btn = cev.button()
        if cev.type() == input.control_event.BUTTON_PUSHED:
            if btn == input.control_event.A_BUTTON:
                if not self.always_run: self.pc.run ();
                else: self.pc.walk ();
                
            elif btn == input.control_event.B_BUTTON:
                self.pc.jump();
                
            elif btn == input.control_event.LEFT_BUTTON:
                self.pc.add_direction(self.pc.WEST);

            elif btn == input.control_event.RIGHT_BUTTON:
                self.pc.add_direction(self.pc.EAST);

            elif btn == input.control_event.UP_BUTTON:
                self.pc.add_direction(self.pc.NORTH);
            
            elif btn == input.control_event.DOWN_BUTTON:
                self.pc.add_direction(self.pc.SOUTH);
         
        else:
            if btn == input.control_event.A_BUTTON:
                if not self.always_run: self.pc.walk();
                else: self.pc.run ();
            
            elif btn == input.control_event.LEFT_BUTTON:
                self.pc.remove_direction(self.pc.WEST);
	            
            elif btn == input.control_event.RIGHT_BUTTON:
                self.pc.remove_direction(self.pc.EAST);
	           
            elif btn == input.control_event.UP_BUTTON:
                self.pc.remove_direction(self.pc.NORTH);
	          
            elif btn == input.control_event.DOWN_BUTTON:
                self.pc.remove_direction(self.pc.SOUTH);

        return 1
