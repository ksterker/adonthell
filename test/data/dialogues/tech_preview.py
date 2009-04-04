import dialogue
from adonthell import rpg# -- pygettext support
#import adonthell
def _(message): return message

class tech_preview (dialogue.base):
	text = [None,\
		_("Welcome to the Adonthell 0.4 Tech Preview. There isn't much to see yet, but as new features become available, they'll be demo'd here. Until then, maybe we can satisfy your curiosity by answering a few questions.   "),\
		_("Nah, I've heard everything I need to know."),\
		_("Yeah, I've got lots of questions."),\
		_("I've been waiting ages for new features ... isn't there a way to speed things up a little?"),\
		_("Remember that we're doing this for fun in our spare time. There's only so much of that available. So to really speed things up, we'd need more people to help out. People like *cough* you, perhaps? "),\
		_("No interest, thanks! I just want to play."),\
		_("Well, care to know anything else then?"),\
		_("Me? You must be kidding! I'm not much of a developer."),\
		_("I'd love to, but time ... you know."),\
		_("Yeah, we know: time's always in short supply and you're probably involved in a handful other projects already. We're not asking much of you however. A few hours per week on avarage would be enough."),\
		_("Really? You mean I can work together with you guys?"),\
		_("Maybe you've got other talents then? We're looking for artists and writers too."),\
		_("What kind of art do you need?"),\
		_("What's involved with writing?"),\
		_("No, I better leave that to somebody else."),\
		_("If anything at all, I'd like to code."),\
		_("Sure. A lot of different people contributed to Adonthell over the past years. Seasoned developers leave and newcomers pick up what remains unfinished. And there's more besides programming too. We also lack graphic artists and writers."),\
		_("Plot, characters and background are pretty much done, so it'll boild down mostly to dialogues like this. And we'll probably need lots of them. "),\
		_("We'll be needing more detailed designs of the different locations described on our Wiki. Then we'll need tile sets for each of those. Another area where not much has been done yet are characters and creatures. We do have all character portraits, but none of the sprites to go with them."),\
		_("Okay. But what could be done in such little time?"),\
		_("Then you should probably start by reading the appropriate parts of the Wiki. The engine architecture description in particular. You might also find the Getting Started Guide helpful. Then see what tasks are still open. Just playing around with this demo might also give you some ideas for improvements or additional features."),\
		_("In any case, you should subscribe to our development mailing list and come by the IRC channel #adonthell on irc.freenode.net from time to time. Those are the places to get in touch with other developers, announce what you'd like to work on, exchange ideas or get help if you get stuck with something."),\
		_("Practically anything, with a bit of patience. There are tasks for programmers, artists and writers that could be tackled over the course of a couple months. Or just patch code up here or there, or paint a few graphics whenever you feel like it. Every small contribution counts!")]

	# -- (speaker, code, ((text, operation, condition), ...))
	dlg = [\
		(None, -1, ((1, 0, -1), )),\
		("Narrator", -1, ((3, 0, -1), (4, 0, -1), (2, 0, -1), )),\
		(None, -1, ()),\
		(None, -1, ()),\
		(None, -1, ((5, 0, -1), )),\
		("Narrator", -1, ((11, 0, -1), (9, 0, -1), (8, 0, -1), (6, 0, -1), )),\
		(None, -1, ((7, 0, -1), )),\
		("Narrator", -1, ((3, 0, -1), (4, 0, -1), (2, 0, -1), )),\
		(None, -1, ((12, 0, -1), )),\
		(None, -1, ((10, 0, -1), )),\
		("Narrator", -1, ((20, 0, -1), (15, 0, -1), )),\
		(None, -1, ((17, 0, -1), )),\
		("Narrator", -1, ((16, 0, -1), (13, 0, -1), (14, 0, -1), (15, 0, -1), )),\
		(None, -1, ((19, 0, -1), )),\
		(None, -1, ((18, 0, -1), )),\
		(None, -1, ((7, 0, -1), )),\
		(None, -1, ((21, 0, -1), )),\
		("Narrator", -1, ((16, 0, -1), (13, 0, -1), (14, 0, -1), (15, 0, -1), )),\
		("Narrator", -1, ((22, 0, -1), )),\
		("Narrator", -1, ((22, 0, -1), )),\
		(None, -1, ((23, 0, -1), )),\
		("Narrator", -1, ((22, 0, -1), )),\
		("Narrator", -1, ()),\
		("Narrator", -1, ((16, 0, -1), (13, 0, -1), (14, 0, -1), ))]


	def __init__(self, p, n):
		self.namespace = globals ()
		self.the_player = p
		self.the_npc = n
