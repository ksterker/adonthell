#
# $Id: mapview.py,v 1.2 2008/05/26 21:17:17 ksterker Exp $
#   
# Copyright (C) 2008 Kai Sterker <kaisterker@linuxgames.com>
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

from adonthell import world

"""
 This is the class containing different map view schedules.
 They will determine which part of a map is visible and
 control camera movement on the map.
"""
class mapview (object):

    """
     This schedule keeps the focus centered on the given
     character and will follow its movement across the map.
    """
    def focus_on_character (self, view, character):
        view.center_on (character.x(), character.y(), character.ox(), character.oy())
        if character.ground_pos() != view.get_z():
            view.scroll_to_z (character.ground_pos(), 2) # abs(int(character.vz())))
