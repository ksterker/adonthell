#
#  $Id: convert_quests.py,v 1.1 2005/10/09 07:41:07 ksterker Exp $
#
#  (C) Copyright 2005 Kai Sterker <kaisterker@linuxgames.com>
#  Part of the Adonthell Project http://adonthell.linuxgames.com
#
#  Adonthell is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  Adonthell is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with Adonthell; if not, write to the Free Software 
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#

import xml.parsers.expat
import adonthell.rpg
import sys

""" states of the sax parser """
class states (object):
	UNDEF			= -1
	ROOT			= 0
	QUEST			= 1
	CODE			= 2
	LOG				= 3
	PARTS			= 4
	LOG_ENTRY		= 5
	TOPIC			= 6
	TEXT			= 7
	ID				= 8

""" Contents of a log index entry """
class log_context (object):
	""" Topic of log entry """
	Topic = ""
	""" Text of log entry """
	Text = ""
	""" Id of log entry """
	Id = ""

""" the parser implementation """
class quest_parser (object):
	""" current state of parser """
	State = states.UNDEF
	""" the expat parser instance """
	Parser = None
	""" quest stack """
	Quest = []
	""" a log entry """
	LogEntry = None
	
	""" initialize expat parser """
	def __init__(self):
		self.Parser = xml.parsers.expat.ParserCreate()

		self.Parser.StartElementHandler = self.start_element
		self.Parser.EndElementHandler = self.end_element
		self.Parser.CharacterDataHandler = self.char_data	

	""" called on start of an xml element """
	def start_element (self, name, attrs):
		# -- root node
		if self.State == states.UNDEF and name == "quests":
			self.State = states.ROOT
			
		# -- a quest node
		elif self.State == states.ROOT and name == "quest":
			self.State = states.QUEST

			quest = str (attrs["name"])
			self.Quest.append (adonthell.rpg.quest_part (quest, None))
			self.Quest[-1].thisown = 0
		
		# -- quest parts
		elif self.State == states.QUEST and name == "parts":
			self.State = states.PARTS
		
		# -- a child quest node
		elif self.State == states.PARTS and name == "quest":
			self.State = states.QUEST
			
			quest = str (attrs["name"])
			parent = self.Quest[-1]

			self.Quest.append (adonthell.rpg.quest_part (quest, parent))
			self.Quest[-1].thisown = 0

			print " + Adding part '%s'" % self.Quest[-1].full_name ()
		
		# -- a quest completion rule 
		elif self.State == states.QUEST and name == "code":
			self.State = states.CODE
		
		# -- log entries
		elif self.State == states.QUEST and name == "log":
			self.State = states.LOG
		
		# -- start or end log entry
		elif self.State == states.LOG and (name == "on_start" or name == "on_completion"):
			self.State = states.LOG_ENTRY
			self.LogEntry = log_context ()
		
		# -- log entry topic
		elif self.State == states.LOG_ENTRY and name == "topic":
			self.State = states.TOPIC
		
		# -- log entry text
		elif self.State == states.LOG_ENTRY and name == "text":
			self.State = states.TEXT

		# -- log entry id
		elif self.State == states.LOG_ENTRY and name == "id":
			self.State = states.ID

		# -- error
		else:
			print " ! Error: Unknown tag '%s' encountered" % (name)
		
	""" called on end of an xml element """
	def end_element (self, name):
		# -- below log entry level
		if self.State == states.TOPIC or self.State == states.TEXT or self.State == states.ID:
			self.State = states.LOG_ENTRY
	
		# -- below log level
		elif self.State == states.LOG_ENTRY:
			self.State = states.LOG
			
			# -- do not add empty log entries
			if self.LogEntry.Id != "":
				log = adonthell.rpg.log_entry (self.LogEntry.Topic, self.LogEntry.Text, self.LogEntry.Id)
				log.thisown = 0
				
				if name == "on_start":
					self.Quest[-1].set_start_entry (log)
				else:
					self.Quest[-1].set_completion_entry (log)
	
		# -- below quest level
		elif self.State == states.CODE or self.State == states.LOG or self.State == states.PARTS:
			self.State = states.QUEST
		
		# -- root or parts level
		elif self.State == states.QUEST:
			quest = self.Quest.pop()
			if len (self.Quest) > 0:
				self.State = states.PARTS
			else:
				self.State = states.ROOT
				print " + Adding quest '%s'" % quest.id ()
				adonthell.rpg.quest_add (quest)
			
		# -- end of parsing
		elif self.State == states.ROOT:
			self.State = states.UNDEF
			
		# -- error
		else:
			print " ! Error: Unknown state transition"

	""" called when character data is encountered """
	def char_data (self, data):
		if self.State == states.CODE:
			self.Quest[-1].set_code (str (data))
		elif self.State == states.TOPIC:
			self.LogEntry.Topic = str (data)
		elif self.State == states.TEXT:
			self.LogEntry.Text = str (data)
		elif self.State == states.ID:
			self.LogEntry.Id = str (data)

	""" parse given xml file """
	def parse (self, filename):
		print " > Parsing '%s' ..." % filename
		self.Parser.ParseFile (file (filename, "r"))

		print " > Finished parsing, now saving to '%s/data/quest.data' ... " % sys.path[0]
		adonthell.rpg.quest.put_state (sys.path[0])
		
# -- the 'main program'
if __name__ == '__main__':
	if len (sys.argv) != 2:
		print "usage: python convert_quests.py <filename>"
		sys.exit(1)
            
	q = quest_parser()
	q.parse (sys.argv[1])
