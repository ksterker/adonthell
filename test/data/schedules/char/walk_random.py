#
# $Id: walk_random.py,v 1.1 2009/04/18 21:54:59 ksterker Exp $
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

"""
 Implements a NPC "schedule", that lets the character walk
 to random places on the map at random intervals.
"""
class walk_random (object):
    
    """
     ctor
    """
    def __init__ (self, schedule):
        # -- the map for this schedule
        self.schedule = schedule
        # -- The NPC map character instance ... 
        #    FIXME: shouldn't be hardcoded
        self.chr = schedule.get_map().get_character ("NPC")
    
    """
     Called once to start the schedule manager.
    """
    def run (self):
        # -- set the actual schedule script
        self.schedule.set_schedule ("walk_random")
        # -- create a gametime between 5 and 12 minutes
        tm = "%im" % (random.randint (5, 12))
        # -- set timer until next run of manager
        self.schedule.set_alarm (tm)

    """
     Called when the schedule is first assigned
    """
    def start (self):
        # -- get map assigned to this schedule
        map = self.schedule.get_map ()
        # -- get random x coordinate
        x = random.randint (10, map.length () - 10)
        # -- get random y coordinate
        y = random.randint (10, map.height () - 10)
        # -- create target vector
        target = world.vector3i (x, y, 0)
        # -- start path finding task
        self.task = world.pathfinding_manager.add_task (self.chr, target)
    
    """
     called whenever the character needs to freeze
    """
    def pause (self):
        # -- stop following path while paused
        world.pathfinding_manager.pause_task (self.task)
        
    """
     Called when the schedule is first assigned
    """
    def resume (self):
        # -- continue following path
        world.pathfinding_manager.resume_task (self.task)

    """ 
     called before the schedule is removed
    """
    def stop (self):
        # -- clean up
        world.pathfinding_manager.delete_task (self.task)
