import dialogue
import adonthell

# -- pygettext support
def _(message): return message

class demo_intro_1 (dialogue.base):
	text = [None,\
		_("Your employer, eh? Who might that be?"),\
		_("The Lady Silverhair, and she must have great need of me, so let me pass!"),\
		_("Nay, if you are Silverhair's man, then I shall not let you pass. That one is the source of all this mess, and I'll be switched if I let her accomplice in to free her. Be on your way!"),\
		_("What sort of mess, man? If my Lady is in trouble, I must know!"),\
		_("Don't be foolish! I must see my Lady at once. Open this gate!"),\
		_("The sort of trouble we have no wish to put up with, that's certain. The Lady is being held for theft, and a grievous theft it was."),\
		_("Theft? But that cannot be, the Lady Silverhair is a wealthy noble in her own right. She need not stoop to theft."),\
		_("You speak madness, man! There can have been no theft. Allow me in, and I shall prove to you the Lady is innocent."),\
		_("But I must! If my Lady is in need of me I cannot simply walk away! Please, I beg you to open the gate so I may aid her."),\
		_("That is none of your concern. All you need know is that she is within your walls and will need me close if there is trouble at hand."),\
		_("She, eh? Your employer wouldn't be an elven lady named Silverhair, would she?"),\
		_("Of course she is, you fool! Who else would it be? Are you going to open this gate or not?"),\
		_("Er, no. No, it is not. In fact, my employer is no lady at all."),\
		_("Huh! You won't get in these doors by lying to me. You said your employer was a lady and there's only one in here who'd employ one of your sort."),\
		_("Very well then, my employer is Lady Silverhair. What of it?"),\
		_("Your Lady is being held for theft, and a grievous theft it was! And you'll certainly not be coming in here to make the situation even worse."),\
		_("That's what you are saying. But fact is, she first argued with the victim over his goods and the other morning they were gone. Now he demands justice and Silverhair is known to need them badly. So what would you say the truth is?"),\
		_("I cannot tell you the truth unless you let me see for myself."),\
		_("You'll have a damned hard time doing that. The lady is guilty and has been seen to be guilty. The truth is that she is the thief and that is all there is to it."),\
		_("I tell you again, you shall not come in."),\
		_("I tell you, I cannot. My orders..."),\
		_("I am sick of arguing with you. Are you about to let me in, or will I be forced to take action? And believe me, you would not enjoy it."),\
		_("The guard hesitates for a moment, and even from where you stand you can see he is uncertain."),\
		_("You'll be in more trouble for not opening that gate than you will be for letting me in, I assure you of that. Now do as I say!"),\
		_("But finally he seems to reach a conclusion, and after carefully scanning the surroundings, he beckons you in."),\
		_("Aid her? That is why you can't be allowed in. If you aid her too much, she might escape."),\
		_("I promise, please. I am becoming sick with worry. Can you not help me?"),\
		_("How could she escape from a guarded camp, even if I did help? Now let me in!"),\
		_("I wish I could, friend. But If I were to open this gate, I would be in terrible trouble."),\
		_("I am beginning to lose my patience, man. I don't wish you trouble, but I must be allowed in."),\
		_("I tell you I can't allow you in.")]

	code = [\
		"self.the_npc.set_dialogue (\"dialogues.talan_start\")\n"]

	# -- (speaker, code, ((text, operation, condition), ...))
	dlg = [\
		(None, -1, ((1, 0, -1), )),\
		("Default", -1, ((2, 0, -1), (10, 0, -1), )),\
		(None, -1, ((3, 0, -1), )),\
		("Default", -1, ((4, 0, -1), )),\
		(None, -1, ((6, 0, -1), )),\
		(None, -1, ((21, 0, -1), )),\
		("Default", -1, ((7, 0, -1), (8, 0, -1), )),\
		(None, -1, ((17, 0, -1), )),\
		(None, -1, ((19, 0, -1), )),\
		(None, -1, ((26, 0, -1), )),\
		(None, -1, ((11, 0, -1), )),\
		("Default", -1, ((12, 0, -1), (13, 0, -1), )),\
		(None, -1, ((3, 0, -1), )),\
		(None, -1, ((14, 0, -1), )),\
		("Default", -1, ((15, 0, -1), )),\
		(None, -1, ((16, 0, -1), )),\
		("Default", -1, ((7, 0, -1), (8, 0, -1), )),\
		("Default", -1, ((18, 0, -1), )),\
		(None, -1, ((20, 0, -1), )),\
		("Default", -1, ((18, 0, -1), )),\
		("Default", -1, ((9, 0, -1), )),\
		("Default", -1, ((22, 0, -1), (24, 0, -1), )),\
		(None, -1, ((23, 0, -1), )),\
		("Narrator", -1, ((25, 0, -1), )),\
		(None, -1, ((23, 0, -1), )),\
		("Narrator", 0, ()),\
		("Default", -1, ((27, 0, -1), (28, 0, -1), (5, 0, -1), )),\
		(None, -1, ((29, 0, -1), )),\
		(None, -1, ((29, 0, -1), )),\
		("Default", -1, ((30, 0, -1), (5, 0, -1), )),\
		(None, -1, ((31, 0, -1), )),\
		("Default", -1, ((22, 0, -1), (24, 0, -1), ))]


	def __init__(self, p, n):
		self.namespace = globals ()
		self.the_player = p
		self.the_npc = n

	def __del__(self):
		self.the_player.do_stuff ("enter_inn")

