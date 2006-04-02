#
# $Id: audio_events.py,v 1.1 2006/04/02 03:31:16 Mithander Exp $
#   
# Copyright (C) 2006 Tyler Nielsen <tyler.nielsen@gmail.com>
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

import random

#
# This class encapsulates callbacks used by audio. They are written
# as generic as possible, 
#
class event_script (object):
    #Right now when timer expires randomly pick a new song.
    def timer (self, lst, evt, music):
        print "Got timer event"
        if(music.sound):
            music.sound.fadeout(5)
        #Pick a new sound at random
        oldsound = music.sound
        while oldsound == music.sound:
            #make sure it isn't the same as what we were playing
            music.sound = random.choice(music.sounds)
        print "Playing:", music.sound.getfilename()
        music.sound.fadein(5)

    #When the song ends just repeat it (You can make one song repeat over and over using play)
    #So normally you will want to pick something different here but this is just an example
    def endsong (self, lst, evt, music):
        print "End of the song, repeating:", music.sound.getfilename()
        if(music.sound):
            music.sound.play()


