#
# $Id: mapview.py,v 1.5 2009/04/19 16:46:12 ksterker Exp $
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

class mapview (object):
    """
     This is the class containing different map view schedules.
     They will determine which part of a map is visible and
     control camera movement on the map.
    """

    def focus_on_character (self, view, char_name):
        """
         This schedule keeps the focus centered on the given
         character and will follow its movement across the map.
        """
        area = world.area_manager.get_map ()
        char = area.get_character (char_name)
        view.center_on (char.x() + char.length()/2, char.y() + char.width()/2)
        z = char.ground_pos() + char.height()/2;
        if z != view.get_z():
            view.scroll_to_z (z, 2)
