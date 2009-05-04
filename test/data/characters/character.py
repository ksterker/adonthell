#
# $Id: character.py,v 1.4 2009/05/04 19:40:40 ksterker Exp $
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

from adonthell import input, gui
from schedules.char import actions 

class action_talk (object):
    """
     Implementation for a conversation between two characters
    """
    def __init__ (self, initiator, other):
        self.initiator = initiator
        self.other = other
        
    def perform (self):
        # -- stop character schedules during conversation
        self.initiator.get_schedule().set_active(0)
        self.other.avatar.get_schedule().set_active(0)
        # -- stop initiator, in case he has been walking
        self.initiator.set_direction(0)
        # -- look in the direction of the character that adressed us
        actions.face_character(self.other.avatar, self.initiator)
        # -- create conversation widget ...
        self.dlg_wnd = gui.conversation (self.other.this, 600, 300, self.on_finished)
        # -- ... and add it to the gui manager
        gui.window_manager.add (self.dlg_wnd)
    
    def on_finished (self):
        """
         called when a conversation has ended.
        """
        # -- close window
        gui.window_manager.remove (self.dlg_wnd)
        
        # -- resume schedules
        self.initiator.get_schedule().set_active(1)
        self.other.avatar.get_schedule().set_active(1)
        
        # -- cleanup
        del self.dlg_wnd
        del self
    

class character (object):
    """
     This is the python counterpart to rpg/character.cc. It contains an
     character's most basic attributes. Any actual character should inherit
     from this class.
    """

    def __init__ (self):
        """ctor"""
        # -- reference to the underlying rpg.character instance
        self.this = None
        # -- reference to the underlying world.character instance
        self.avatar = None

    def destroy (self):
        """properly delete a character"""
        if self.this != None:
            self.this.destroy ()
            
    def get_area_of_effect (self, action):
        """
         get the area that is affected when this character
         performs an action. Returns an arc and a radius.
        """
        return (120, 30)

    def perform_action (self, action, initiator):
        """
         perform an action.
         @param action the type of action to perform
         @param initiator world character instance of the character 
                triggering the action
        """
        # -- trigger a conversation
        if action == actions.ACTION_NORMAL:
            self.the_action = action_talk (initiator, self)
            self.the_action.perform ()
            
    def put_state (self, record):
        """
         save character to disk
         record needs to be of type base.flat
        """
        pass
        
    def get_state (self, record):
        """
         load character from disk
         record needs to be of type base.flat
        """
        pass
