from adonthell import event, main, rpg
import sys

# -- test event callbacks
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

# -- test class
class App (main.AdonthellApp):
	def __init__ (self):
		main.AdonthellApp.__init__ (self)

	def run (self):
		# -- load quest tree
		rpg.quest.get_state ()
		
		# -- setup some quest events
		factory = event.factory ()

		# -- one that detects all changes to the children of Quest_1
		capture_all_event = rpg.quest_event ("Quest_1.>")
		listener = factory.add (capture_all_event)
		listener.connect_callback ("questtest", "event_callbacks", "capture_all")

		# -- one that detects changes of Quest_1
		quest_1_event = rpg.quest_event ("Quest_1")
		listener = factory.add (quest_1_event)
		listener.connect_callback ("questtest", "event_callbacks", "quest_1")

		# -- start completing quest steps
		rpg.quest.set_completed ("Quest_1.Part_1.Step_3")
		rpg.quest.set_completed ("Quest_1.Step_1")
		
		# -- TODO: save and reload, then continue ...

		# -- complete remaining quest steps		
		rpg.quest.set_completed ("Quest_1.Part_1.Step_5")
		rpg.quest.set_completed ("Quest_1.Step_2")
		rpg.quest.set_completed ("Quest_1.Part_1.Step_4")

# -- main
if __name__ == '__main__':
    theApp = App ()
    theApp.init (theApp.run)
