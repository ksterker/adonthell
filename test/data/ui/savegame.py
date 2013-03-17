#
# Copyright (C) 2013 Kai Sterker <kai.sterker@gmail.com>
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

from adonthell import gfx
from adonthell.world import mapview
from adonthell.world import area_manager

class savegame (object):
    """
     User interface and helpers for saving & loading your game
    """
    
    def save_screenshot (self, directory):
        # -- create a standard-sized view to capture a screenshot
        mv = mapview (640, 480)
        # -- get the real map view to obtain the location for our view 
        actual_view = area_manager.get_mapview()
        # -- this makes sure that the view is at proper Z coordinate
        mv.set_position (actual_view.get_x(), actual_view.get_y(), actual_view.get_z())
        # -- now actually ensure that the x and y are in the center of the view
        mv.center_on (actual_view.get_x(), actual_view.get_y())
        
        # -- create a surface onto which to render the map
        surf = gfx.create_surface()
        surf.resize (640, 480)

        # -- render the view onto the surface
        mv.draw (0, 0, None, surf)

        # -- save the surface to disk
        surf.save_png(directory + "/screenshot.png")
        
        # -- cleanup
        surf = None
        mv = None
