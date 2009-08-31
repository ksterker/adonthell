#
# Copyright (C) 2009 Frederico Cerveira <frederico.cerveira@gmail.com>
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
import groups.group

class faction (groups.group.group):
    """
     This is the base class for any faction. Every faction should
     inherit from this one
    """ 
    
    def __init__ (self):
        """ctor"""
        groups.group.group.__init__(self)
        
    def requirements (self):
        """ 
            this function verifies if the C++ class calling this template fulfills
            the requirements of this social_class.
            @return 0 if any requirement fails, 1 if everything is OK
        """
        return 1
