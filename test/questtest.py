from adonthell import event, main, rpg
import sys

# -- test event callbacks

# -- test class
class App (main.AdonthellApp):

	def run (self):
		# -- add data directory to python search path
		sys.path.insert (0, "data")
		
		# -- load quest tree
		rpg.quest.load ()
		
		# -- setup some quest events
		factory = event.factory ()

		# -- one that detects all changes to the children of Quest_1
		capture_all_event = rpg.quest_event ("Quest_1.>")
		listener = factory.add (capture_all_event)
		listener.connect_callback ("quest_events", "event_callbacks", "capture_all")

		# -- one that detects changes of Quest_1
		quest_1_event = rpg.quest_event ("Quest_1")
		listener = factory.add (quest_1_event)
		listener.connect_callback ("quest_events", "event_callbacks", "quest_1")

		# -- start completing quest steps
		rpg.quest.set_completed ("Quest_1.Part_1.Step_3")
		rpg.quest.set_completed ("Quest_1.Step_1")
		
		# -- TODO: save and reload, then continue ...

		# -- complete remaining quest steps		
		rpg.quest.set_completed ("Quest_1.Part_1.Step_5")
		rpg.quest.set_completed ("Quest_1.Step_2")
		rpg.quest.set_completed ("Quest_1.Part_1.Step_4")

		return 0
	
# -- main
if __name__ == '__main__':
    theApp = App ()
    theApp.init (theApp.run)
