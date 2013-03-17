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

from adonthell import base, input, world, event
import actions
import ui.savegame

class player (object):
    """
     Implements the PC "schedule", which is being controlled
     by the player.
    """
    
    def __init__ (self, schedule):
        """
         ctor
        """
        # -- the map for this schedule
        self.schedule = schedule
        # -- Whether PC is always running
        self.always_run = 0
        # -- The owner of the schedule
        self.pc = self.schedule.get_owner()
        # -- The game "mode"
        self.state = actions.ACTION_NORMAL
    
    def run (self):
        """
         Called once to start the schedule manager.
        """
        # -- set the actual schedule script
        self.schedule.set_schedule ("player")

    def start (self):
        """
         Called when the schedule is first assigned
        """
        # -- Create our input_listener 
        self.il = input.listener()

        # -- Add the listener to the manager
        input.manager.add(self.il)
        
        # -- connect the callback to handle key/control events
        self.il.connect_control_function(self.handle_controls)
        self.il.connect_keyboard_function(self.handle_keys)
    
    def pause (self):
        """
         called whenever the character needs to freeze
        """
        # -- stop listening to events while paused
        self.il.disconnect_control_function ()
        self.il.disconnect_keyboard_function ()
        
    def resume (self):
        """
         Called when the schedule is first assigned
        """
        # -- start listening to key/control events again
        self.il.connect_control_function(self.handle_controls)
        self.il.connect_keyboard_function(self.handle_keys)

    def stop (self):
        """ 
         called before the schedule is removed
        """
        # -- clean up
        input.manager.remove (self.il)
        self.il.disconnect_control_function ()
        self.il.disconnect_keyboard_function ()
        self.il = None
        
    def handle_keys (self, kev):
        """
         Called whenever a regular key has bee pushed.
        """
        # -- caps lock toggles running on or off
        if kev.key() == input.keyboard_event.CAPSLOCK_KEY:
            if kev.type() == input.keyboard_event.KEY_PUSHED:
                self.always_run = 1
                self.pc.run ()
            else:
                self.always_run = 0
                self.pc.walk ()
            # -- consume key event
            return 1
            
        if kev.type() == input.keyboard_event.KEY_PUSHED:
            # -- F5: quick save
            if kev.key() == input.keyboard_event.F5_KEY:
                game_mgr = base.savegame()
                game_mgr.save (base.savegame.QUICK_SAVE, "Quicksave", event.date.time())
                
                game_ui = ui.savegame.savegame()
                game_ui.save_screenshot (game_mgr.get (base.savegame.QUICK_SAVE).directory())
                
                game_mgr = None
                return 1
                
            # -- F6: quick load
            elif kev.key() == input.keyboard_event.F6_KEY:
                game_mgr = base.savegame()
                game_mgr.load (base.savegame.QUICK_SAVE)
                game_mgr = None                
                return 1
        return 0

    def handle_controls (self, cev):
        """
         Called whenever a control key has bee pushed.
        """
        btn = cev.button()
        if cev.type() == input.control_event.BUTTON_PUSHED:
            if btn == input.control_event.A_BUTTON:
                if not self.always_run: self.pc.run ();
                else: self.pc.walk ();
                
            elif btn == input.control_event.B_BUTTON:
                actions.perform_action (self.pc, self.state)
                
            elif btn == input.control_event.C_BUTTON:
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
