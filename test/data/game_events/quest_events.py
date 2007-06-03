#
# $Id: quest_events.py,v 1.1 2007/06/03 21:09:16 ksterker Exp $
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

#
# This class implements test callbacks used by the quest system. 
#
class event_callbacks (object):
    def capture_all (self, listener, event):
        quest = event.part ()
        if quest.is_started ():
            print "[1] Quest '%s' has been started" % quest.full_name ()
        if quest.in_progress ():
            print "[1] Quest '%s' is in progress" % quest.full_name ()            
        if quest.is_completed ():
            print "[1] Quest '%s' has been completed" % quest.full_name ()

    def quest_1 (self, listener, event):
        print "[2] State of Quest_1 changed"
