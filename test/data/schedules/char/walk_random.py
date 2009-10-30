#
# $Id: walk_random.py,v 1.2 2009/04/19 16:46:12 ksterker Exp $
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
import random

class walk_random (object):
    """
     Implements a NPC "schedule", that lets the character walk
     to random places on the map at random intervals.
    """
    
    def __init__ (self, schedule):
        """
         ctor
        """
        # -- the map for this schedule
        self.schedule = schedule
        # -- The NPC map character instance ... 
        self.chr = schedule.get_owner()
    
    def run (self):
        """
         Called once to start the schedule manager.
        """
        # -- set the actual schedule script
        self.schedule.set_schedule ("walk_random")
        # -- create a gametime between 5 and 12 minutes
        tm = "%im" % (random.randint (5, 12))
        # -- set timer until next run of manager
        self.schedule.set_alarm (tm)

    def start (self):
        """
         Called when the schedule is first assigned
        """
        # -- get map assigned to this schedule
        map = self.chr.map ()
        # -- get random x coordinate
        x = random.randint (20, map.length () - 20)
        # -- get random y coordinate
        y = random.randint (20, map.height () - 20)
        # -- create target vector
        target = world.vector3i (x, y, 0)
        # -- start path finding task
        self.task = world.pathfinding_manager.add_task (self.chr, target)
        # -- get notification when goal has been reached
        world.pathfinding_manager.set_callback (self.task, self.on_arrived)
    
    def pause (self):
        """
         called whenever the character needs to freeze
        """
        # -- stop following path while paused
        world.pathfinding_manager.pause_task (self.task)
        
    def resume (self):
        """
         Called when the schedule is first assigned
        """
        # -- continue following path
        world.pathfinding_manager.resume_task (self.task)

    def stop (self):
        """ 
         called before the schedule is removed
        """
        # -- clean up
        world.pathfinding_manager.delete_task (self.task)

    def on_arrived (self, goal_reached):
        """ 
         callback notifying that path finding is done
        """
        if goal_reached == 1:
            # -- hooray!
            self.chr.jump()
            self.chr.walk()
        else:
            # -- now I need to hurry
            self.chr.run()
        