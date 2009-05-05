#include "conversation.h"

#define LINEHEIGHT 20 
namespace gui
{
	conversation::conversation(rpg::character & c, int w, int h, ::base::functor_0*e)
	:widget(w, h),ct(w-30, h/3),dlg(c),speaker(w-30, LINEHEIGHT),end(e)
	{
        font f (NULL, LINEHEIGHT - 1);
        ct.setFont(f);
        speaker.setFont(f);
        
        bg = gfx::surfaces->get_surface_only("data/gfx/gui/conversation.png");//FIXME
		line = dlg.run(-1);
		ct.multiline(true);
		objs.addchild(speaker, 15, 10);
		objs.addchild(ct, 15, LINEHEIGHT+10);
		opty = ct.getHeight() + 40;
		optcount = 0;
		update();
		color = c.color(); 
		ct.setColor(color);
	}
	void conversation::selectopt(bool down, void* arg)
	{
		answer* self = (answer*)arg;
		self->obj->line = self->obj->dlg.run(self->which);
		self->obj->update();
	}
	void conversation::update()
	{
		while (optcount)
		{
			optcount--;
			objs.removechild(*options[optcount]);
			delete options[optcount];
		}
		optcount = 0;
		if (!line) //this is null if we hit the end of the conversation
		{
			if (end) {
				(*end)();
				delete end;
				end = NULL;
			}
			//speaker.setString(string(""));
			//ct.setString("(End)");
			return;
		}
		speaker.setString(string(line->speaker()) + ":");
		ct.setString(line->text());
		//*
		int i;
		int y = opty;
		optcount = line->num_answers() > MAX_OPTS ? MAX_OPTS : line->num_answers();
         
        font f (NULL, LINEHEIGHT - 1);
        f.setColor(rpg::character::get_player()->color());

		for (i = 0; i < optcount; i++)
		{
			answers[i].which = i;
			answers[i].obj = this;
			options[i] = new button(w-40, LINEHEIGHT, ::base::make_functor(*this, &conversation::selectopt), (void*)&answers[i], 2);
			options[i]->setFont(f);
			options[i]->multiline(true);
			char tmp[16];
			snprintf(tmp, 16, "%i)", i+1);
			options[i]->setString(string(tmp)+line->answer(i));
			options[i]->reheight();
			options[i]->centerV(false);
            objs.addchild(*options[i], 20, y);
			y += options[i]->getHeight() +5;
		}
		if (optcount == 0) 
		{
			optcount = 1;
			answers[0].which = -1;
			answers[0].obj = this;
			options[0] = new button(w-40, LINEHEIGHT, ::base::make_functor(*this, &conversation::selectopt), (void*)&answers[i],2);
			options[0]->setFont(f);
			options[0]->multiline(true);
			options[0]->setString("1) (continue)");
			options[0]->reheight();
			options[0]->centerV(false);
			objs.addchild(*options[0], 20, y);
			y += options[0]->getHeight() +5;
		}
		objs.focus();
		// */
	}
	bool conversation::keyup(input::keyboard_event &k) 
	{
		switch (k.key())
		{
		case input::keyboard_event::N1_KEY:
		case input::keyboard_event::KP1_KEY:
			selectopt(true, &answers[0]);
			break;
		case input::keyboard_event::N2_KEY:
		case input::keyboard_event::KP2_KEY:
			if (optcount > 0) selectopt(true, &answers[1]);
			break;
		case input::keyboard_event::N3_KEY:
		case input::keyboard_event::KP3_KEY:
			if (optcount > 1) selectopt(true, &answers[2]);
			break;
		case input::keyboard_event::N4_KEY:
		case input::keyboard_event::KP4_KEY:
			if (optcount > 2) selectopt(true, &answers[3]);
			break;
		case input::keyboard_event::N5_KEY:
		case input::keyboard_event::KP5_KEY:
			if (optcount > 3) selectopt(true, &answers[4]);
			break;
		case input::keyboard_event::N6_KEY:
		case input::keyboard_event::KP6_KEY:
			if (optcount > 4) selectopt(true, &answers[5]);
			break;
		case input::keyboard_event::N7_KEY:
		case input::keyboard_event::KP7_KEY:
			if (optcount > 5) selectopt(true, &answers[6]);
			break;
		case input::keyboard_event::N8_KEY:
		case input::keyboard_event::KP8_KEY:
			if (optcount > 6) selectopt(true, &answers[7]);
			break;
		case input::keyboard_event::N9_KEY:
		case input::keyboard_event::KP9_KEY:
			if (optcount > 7) selectopt(true, &answers[8]);
			break;
		default:
			return objs.keyup(k);
		}
		return true;
	}
	bool conversation::keydown(input::keyboard_event &k) 
	{
		return objs.keydown(k);
	}
};
