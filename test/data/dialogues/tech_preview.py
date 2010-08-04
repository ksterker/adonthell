import dialogue
from adonthell import rpg# -- pygettext support
def _(message): return message

class tech_preview (dialogue.base):
	text = [None,\
		_("Welcome to the Adonthell 0.4 Tech Preview. There isn't much to see yet, but as new features become available, they'll be demonstrated here. Until then, maybe we can satisfy your curiosity by answering a few questions.   "),\
		_("Nah, I've heard everything I need to know."),\
		_("Yeah, I've got lots of questions."),\
		_("I've been waiting ages for new features... could you speed things up a little?"),\
		_("Remember that we're doing this for fun in our spare time. There's only so much of that available. So to really speed things up, we'd need more people to help out. People like *cough* you, perhaps? "),\
		_("No interest, thanks! I just want to play."),\
		_("Well, care to know anything else then?"),\
		_("Me? You must be kidding! I'm not much of a developer."),\
		_("I'd love to, but time... you know."),\
		_("Yeah, we know: time is always in short supply, and you're probably involved in a handful of other projects already. We're not asking much of you however. A few hours per week on average would be enough."),\
		_("Really? You mean I can work together with you guys?"),\
		_("Maybe you've got other talents then? We're looking for artists and writers too."),\
		_("What kind of art do you need?"),\
		_("What's involved with writing?"),\
		_("No, I better leave that to somebody else."),\
		_("If anything at all, I'd like to code."),\
		_("Sure. A lot of different people contributed to Adonthell over the past years. We tried our best to make it easy for newcomers to pick up a task. And there's more besides programming: graphics and writing also have to be done."),\
		_("Plot, characters, and background are pretty much done, so it boils down mostly to dialogues like this--and we'll need lots of them. "),\
		_("We'll need more detailed designs of the different locations described on our Wiki. Then we'll need tile sets for each of those. Another area where not much has been done yet are characters and creatures. We do have all NPC portraits, but none of the sprites to go with them."),\
		_("Okay, but what could be done in such little time?"),\
		_("Then you should probably start by browsing our Wiki. Have a look at the Engine Architecture documentation and the Getting Started Guide. Then check out the Roadmap and Open Tasks. Of course, just playing around with this demo might give you an idea what to do."),\
		_("In any case, if you would like to help, you should subscribe to our development mailing list and come by the IRC channel #adonthell on irc.freenode.net from time to time."),\
		_("Practically anything, with a bit of patience. There are tasks for programmers, artists, and writers that could be tackled over the course of a couple months. Or just patch code up here and there, or paint a few graphics whenever you feel like it. Every contribution counts!"),\
		_("You'll probably find some of them answered on our Wiki, but since you're here already..."),\
		_("What is taking you guys so long? With Waste's Edge you already had a fairly complete game!"),\
		_("This doesn't look much different from the first v0.4 alpha. Have you done anything at all in the past year?"),\
		_("So what's this all about, then?"),\
		_("Actually, I'd like to know something else."),\
		_("Well, if you're that curious, why not get in touch with us and ask your questions directly. You can find us on the IRC channel #adonthell on irc.freenode.net."),\
		_("It's a long story, and developer's egos are part of it. Waste's Edge had shown some severe flaws in our design -- the way we were interfacing with Python, for example. We've also grown more ambitious after seeing what we had done already."),\
		_("Unfortunately, after laying down the new design and refactoring a lot of the existing code, some of the core developers no longer found time to continue their work, so important bits like the map engine or user interface were suddenly on hold."),\
		_("At that point, we spent a lot of time documenting existing code and future tasks to make life easier for new developers. That alone didn't help much. Without much working code to show, there was little interest in helping out."),\
		_("Hey! I could lend a hand, perhaps?"),\
		_("So that's what this strange demo is all about?"),\
		_("Looks like you're doing better now, although I still don't see much progress."),\
		_("In the past few months alone we've made some considerable improvements. We have the basics of the GUI working (as you can see). And we've got pathfinding. Outside this demo, we've also started work on the map editor."),\
		_("Of course, all of these need some more work, but they are the key to integrate more of the existing code. There's the whole item code lying unused for years because we didn't have an inventory GUI. That will change now."),\
		_("And as the map editor becomes usable, the looks of this demo will change for the better as well -- provided we get the help of an artist."),\
		_("An artist, you say? What kind of art do you need, exactly?"),\
		_("So there's a lot more to expect in the future, eh?"),\
		_("Good luck to you then. I gotta go now."),\
		_("Certainly. Right now we have 3 programmers working on the engine and editors, although we could use another one to work on the rules implementation. In any case, we're working on bringing everything to the point where we can run Waste's Edge with it."),\
		_("Waste's Edge?! Why bother with the old crap? Didn't you have a new game planned?"),\
		_("Game rules? Sounds interesting. Do you have more information about that?"),\
		_("We're already spending time keeping Waste's Edge running as its dependencies evolve. In the future we might have to port the old engine to Python 3 and SDL 1.3. By porting Waste's Edge to the new engine instead, we save some work."),\
		_("Besides, it's a nice test case. Moreover, the next game -- Dun Barethsol -- will include Waste's Edge as a location, so recreating it for the new engine will be necessary anyway."),\
		_("Of course. See the Role Playing System section on the Wiki. That's what has to be implemented (mostly in Python). The Scripting Guide (also on the Wiki) may have more details about how to do that."),\
		_("There we can discuss what needs to be done, pass around design ideas, and answer all questions that may come up. "),\
		_("Sounds good. Maybe I'll see you there."),\
		_("Well, apart from generating interest in the project, this is also the testbed for all the new features that we implement. As such, it'll slowly evolve into the next demo game, Dun Barethsol."),\
		_("Any estimate when it will be ready?"),\
		_("There is still a lot of work ahead. Without further help, it will probably take years still. "),\
		_("Tell me more about this \"Dun Barethsol\"."),\
		_("I assume that you've heard of, or even played, Adonthell -- Waste's Edge. What you have here is going to be the successor to that. A role playing game with a detailed background, well-balanced combat, puzzles, and character interaction elements."),\
		_("With Waste's Edge, we have demonstrated character interaction and a few puzzles. With the next demo game that we have planned -- Dun Barethsol -- we will add combat, magic, and items on top of that. Overall, it will be much larger too."),\
		_("Wait a moment. Can you elaborate a bit on your ideas about RPG design?"),\
		_("If you are curious, you can find out more about that on our Wiki. See the section about Plot Design Guidelines near the bottom of the main page. For the combat stuff refer to the Role Playing System section."),\
		_("But Waste's Edge looks much more complete than this here. How come?"),\
		_("We don't really want to spoil it for you. Suffice it to say that it will be a complete RPG with everything you'd expect from one. It won't be a full length game, but we hope to get 5 to 10 hours of playtime into it."),\
		_("Also, it will have multiple paths to the conclusion of the story (we've been calling them \"The Thief's Path\", \"The Warrior's Path\", and \"The Mystic Path\"), so you could play it a couple times and still uncover new pieces of the story. There'll be multiple endings too. "),\
		_("So I can play as a Warrior, Rogue or Mage? "),\
		_("No. Everyone will start with the same character, an orphaned human girl named Marcella. But depending on the choices you make during the game, you'll be evolving into a specific direction... or combination thereof."),\
		_("We think that this is more \"true\" roleplaying, as your actions will define what you become. In most other RPGs you start with a defined role, but then you are free to behave however you like anyway."),\
		_("Sounds like the plot and everything is already complete?"),\
		_("It pretty much is on a fairly generic level. We've got the characters, quests, and side-quests covered, but not in every little detail. We also just began working on the dialogue."),\
		_("Writing, huh? That sounds like something I could help with?"),\
		_("We could use all the help we can get with this. Even the short Waste's Edge demo had over 1000 lines of dialogue. For Dun Barethsol we expect at least 10 times as much.")]

	# -- (speaker, code, ((text, operation, condition), ...))
	dlg = [\
		(None, -1, ((1, 0, -1), )),\
		("Narrator", -1, ((3, 0, -1), (4, 0, -1), (2, 0, -1), )),\
		(None, -1, ()),\
		(None, -1, ((24, 0, -1), )),\
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
		("Narrator", -1, ((48, 0, -1), )),\
		("Narrator", -1, ((16, 0, -1), (13, 0, -1), (14, 0, -1), )),\
		("Narrator", -1, ((27, 0, -1), (25, 0, -1), (26, 0, -1), (28, 0, -1), )),\
		(None, -1, ((30, 0, -1), )),\
		(None, -1, ((36, 0, -1), )),\
		(None, -1, ((54, 0, -1), )),\
		(None, -1, ((29, 0, -1), )),\
		("Narrator", -1, ((49, 0, -1), )),\
		("Narrator", -1, ((31, 0, -1), )),\
		("Narrator", -1, ((32, 0, -1), )),\
		("Narrator", -1, ((33, 0, -1), (34, 0, -1), (35, 0, -1), )),\
		(None, -1, ((17, 0, -1), )),\
		(None, -1, ((50, 0, -1), )),\
		(None, -1, ((36, 0, -1), )),\
		("Narrator", -1, ((37, 0, -1), )),\
		("Narrator", -1, ((38, 0, -1), )),\
		("Narrator", -1, ((40, 0, -1), (39, 0, -1), (41, 0, -1), )),\
		(None, -1, ((19, 0, -1), )),\
		(None, -1, ((42, 0, -1), )),\
		(None, -1, ()),\
		("Narrator", -1, ((43, 0, -1), (44, 0, -1), (41, 0, -1), )),\
		(None, -1, ((45, 0, -1), )),\
		(None, -1, ((47, 0, -1), )),\
		("Narrator", -1, ((46, 0, -1), )),\
		("Narrator", -1, ((53, 0, -1), (41, 0, -1), )),\
		("Narrator", -1, ((22, 0, -1), )),\
		("Narrator", -1, ((49, 0, -1), )),\
		(None, -1, ()),\
		("Narrator", -1, ((53, 0, -1), (51, 0, -1), )),\
		(None, -1, ((52, 0, -1), )),\
		("Narrator", -1, ((53, 0, -1), (33, 0, -1), (41, 0, -1), )),\
		(None, -1, ((59, 0, -1), )),\
		("Narrator", -1, ((55, 0, -1), )),\
		("Narrator", -1, ((56, 0, -1), (53, 0, -1), (58, 0, -1), )),\
		(None, -1, ((57, 0, -1), )),\
		("Narrator", -1, ((53, 0, -1), )),\
		(None, -1, ((30, 0, -1), )),\
		("Narrator", -1, ((60, 0, -1), )),\
		("Narrator", -1, ((61, 0, -1), (64, 0, -1), (51, 0, -1), )),\
		(None, -1, ((62, 0, -1), )),\
		("Narrator", -1, ((63, 0, -1), )),\
		("Narrator", -1, ((64, 0, -1), (51, 0, -1), )),\
		(None, -1, ((65, 0, -1), )),\
		("Narrator", -1, ((61, 0, -1), (51, 0, -1), (66, 0, -1), )),\
		(None, -1, ((67, 0, -1), )),\
		("Narrator", -1, ((22, 0, -1), ))]


	def __init__(self, p, n):
		self.namespace = globals ()
		self.the_player = p
		self.the_npc = n
